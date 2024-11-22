#include "stdes.h"
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>  // Pour gérer les erreurs système

// Flux globaux
FICHIER *my_stdout = NULL;
FICHIER *my_stderr = NULL;

// Fonction pour initialiser les flux globaux
void initialiser_flux(void) {
    my_stdout = (FICHIER *)malloc(sizeof(FICHIER));
    if (!my_stdout) {
        perror("Erreur : allocation de my_stdout");
        exit(EXIT_FAILURE);
    }
    my_stdout->fd = STDOUT_FILENO; // Flux standard pour la console
    my_stdout->buf_pos_read = 0;
    my_stdout->buf_pos_write = 0;
    my_stdout->buf_size = 0;
    my_stdout->mode = 'W';
    my_stdout->eof = 0;

    my_stderr = (FICHIER *)malloc(sizeof(FICHIER));
    if (!my_stderr) {
        perror("Erreur : allocation de my_stderr");
        exit(EXIT_FAILURE);
    }
    my_stderr->fd = STDERR_FILENO; // Flux d'erreur standard
    my_stderr->buf_pos_read = 0;
    my_stderr->buf_pos_write = 0;
    my_stderr->buf_size = 0;
    my_stderr->mode = 'W';
    my_stderr->eof = 0;
}

// 1 - Fonction pour ouvrir un fichier en mode lecture ('R') ou écriture ('W')
FICHIER *ouvrir(const char *nom, char mode) {
    if (!nom || (mode != 'R' && mode != 'W')) {
        return NULL;
    }

    FICHIER *f = (FICHIER *)malloc(sizeof(FICHIER));
    if (!f) {
        return NULL;
    }

    int flags = (mode == 'R') ? O_RDONLY : (O_WRONLY | O_CREAT | O_TRUNC);
    f->fd = open(nom, flags, 0644);
    if (f->fd == -1) {
        perror("Erreur lors de l'ouverture du fichier");
        free(f);
        return NULL;
    }

    memset(f->tampon, 0, BUFFER_SIZE);
    f->buf_size = 0;
    f->buf_pos_read = 0;
    f->buf_pos_write = 0;
    f->mode = mode;
    f->eof = 0;

    return f;
}

// 2 - Fonction pour fermer un fichier
int fermer(FICHIER *f) {
    if (!f) {
        return -1;
    }

    if (f->mode == 'W' && f->buf_pos_write > 0) {
        if (vider(f) == -1) {
            close(f->fd);
            free(f);
            return -1;
        }
    }

    if (close(f->fd) == -1) {
        perror("Erreur lors de la fermeture du fichier");
        free(f);
        return -1;
    }

    free(f);
    return 0;
}

/**
 * Fonction pour lire des données d'un fichier.
 * 
 * Cette fonction lit un nombre d'éléments spécifié depuis un fichier en utilisant un tampon intermédiaire.
 * Elle garantit que la lecture passe toujours par le tampon et retourne le nombre d'éléments effectivement lus.
 */
int lire(void *p, unsigned int taille, unsigned int nbelem, FICHIER *f) {
    if (!f || f->mode != 'R' || !p || taille == 0 || nbelem == 0) {
        return -1;
    }

    size_t total_bytes = taille * nbelem;
    size_t bytes_read = 0;

    // Lire les données disponibles dans le tampon
    if (f->buf_pos_read < f->buf_size) {
        size_t available = f->buf_size - f->buf_pos_read;
        size_t to_copy = (total_bytes < available) ? total_bytes : available;

        memcpy(p, f->tampon + f->buf_pos_read, to_copy);
        f->buf_pos_read += to_copy;
        bytes_read += to_copy;
    }

    // Lire les données depuis le fichier
    while (bytes_read < total_bytes) {
        ssize_t n = read(f->fd, f->tampon, BUFFER_SIZE);
        if (n == 0) {
            f->eof = 1;
            break;
        }
        if (n < 0) {
            perror("Erreur lors de la lecture");
            return -1;
        }

        f->buf_size = n;
        f->buf_pos_read = 0;

        size_t to_copy = (total_bytes - bytes_read < (size_t)n) ? (total_bytes - bytes_read) : (size_t)n;
        memcpy((char *)p + bytes_read, f->tampon, to_copy);
        f->buf_pos_read += to_copy;
        bytes_read += to_copy;
    }

    return bytes_read / taille;
}

// 4 - Fonction pour écrire des données dans un fichier
int ecrire(const void *p, unsigned int taille, unsigned int nbelem, FICHIER *f) {
    if (!f || f->mode != 'W' || !p || taille == 0 || nbelem == 0) {
        return -1;
    }

    size_t total_bytes = taille * nbelem;
    size_t bytes_written = 0;

    while (bytes_written < total_bytes) {
        if (f->buf_pos_write == BUFFER_SIZE) {
            if (vider(f) == -1) {
                return -1;
            }
        }

        size_t to_copy = (total_bytes - bytes_written < BUFFER_SIZE - f->buf_pos_write)
                         ? total_bytes - bytes_written
                         : BUFFER_SIZE - f->buf_pos_write;

        memcpy(f->tampon + f->buf_pos_write, (char *)p + bytes_written, to_copy);
        f->buf_pos_write += to_copy;
        bytes_written += to_copy;
    }

    return bytes_written / taille;
}

// 5 - Fonction pour vider le tampon d'écriture
int vider(FICHIER *f) {
    if (!f || f->mode != 'W') {
        return -1;
    }

    ssize_t bytes_written = write(f->fd, f->tampon, f->buf_pos_write);
    if (bytes_written == -1) {
        perror("Erreur lors de l'écriture dans le fichier");
        return -1;
    }

    f->buf_pos_write = 0;
    return 0;
}



// 6 - Fonction pour écrire dans un fichier avec un formatage
int fecriref(FICHIER *f, const char *format, ...) {
    if (!f || f->mode != 'W' || !format) {
        return -1;
    }

    va_list args;
    va_start(args, format);

    int result = vfprintf(fdopen(f->fd, "w"), format, args);

    va_end(args);
    return result;
}


// 7 - Fonction pour écrire directement dans la console avec un formatage
int ecriref(const char *format, ...) {
    if (!format) {
        return -1;
    }

    va_list args;
    va_start(args, format);

    int result = vfprintf(stdout, format, args);

    va_end(args);
    return result;
}


// 8 - Fonction pour lire depuis un fichier avec un formatage
int fliref(FICHIER *f, const char *format, ...) {
    if (!f || f->mode != 'R' || !format) {
        return -1;
    }

    va_list args;
    va_start(args, format);

    int result = vfscanf(fdopen(f->fd, "r"), format, args);

    va_end(args);
    return result;
}