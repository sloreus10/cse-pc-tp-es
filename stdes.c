#include "stdes.h"
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>  // Pour gérer les erreurs système

// Taille par défaut du tampon de lecture/écriture
#define BUFFER_SIZE 64

// Structure représentant un fichier
struct _ES_FICHIER {
    int fd;                     // Descripteur de fichier
    char tampon[BUFFER_SIZE];    // Tampon pour les données
    unsigned int buf_size;      // Taille actuelle des données dans le tampon
    unsigned int buf_pos;       // Position actuelle dans le tampon
    char mode;                  // Mode du fichier ('R' pour lecture, 'W' pour écriture)
    int eof;                    // Indicateur de fin de fichier
};

// 1 - Fonction pour ouvrir un fichier en mode lecture ('R') ou écriture ('W')
FICHIER *ouvrir(const char *nom, char mode) {
    if (!nom || (mode != 'R' && mode != 'W')) {
        // Nom invalide ou mode incorrect
        return NULL;
    }

    // Allouer de la mémoire pour la structure FICHIER
    FICHIER *f = (FICHIER *)malloc(sizeof(FICHIER));
    if (!f) {
        // Erreur d'allocation mémoire
        return NULL;
    }

    // Déterminer les flags pour l'ouverture du fichier
    int flags = (mode == 'R') ? O_RDONLY : (O_WRONLY | O_CREAT | O_TRUNC);

    // Ouvrir le fichier
    f->fd = open(nom, flags, 0644);
    if (f->fd == -1) {
        // Échec de l'ouverture du fichier, libération de la mémoire
        free(f);
        return NULL;
    }

    // Initialiser les champs de la structure
    memset(f->tampon, 0, BUFFER_SIZE);
    f->buf_size = 0;
    f->buf_pos = 0;
    f->mode = mode;
    f->eof = 0;

    return f;
}


// 2 - Fonction pour fermer un fichier
int fermer(FICHIER *f) {
    if (!f) {
        // Pointeur invalide
        return -1;
    }

    // Si le fichier est en mode écriture, vider le tampon
    if (f->mode == 'W' && f->buf_pos > 0) {
        if (vider(f) == -1) {
            // Erreur lors du vidage
            close(f->fd);
            free(f);
            return -1;
        }
    }

    // Fermer le fichier
    if (close(f->fd) == -1) {
        // Échec de la fermeture du fichier
        free(f);
        return -1;
    }

    // Libérer la mémoire allouée
    free(f);
    return 0;
}


/**
 * Fonction pour lire des données d'un fichier.
 * 
 * Cette fonction lit un nombre d'éléments spécifié depuis un fichier en utilisant un tampon intermédiaire.
 * Elle garantit que la lecture passe toujours par le tampon et retourne le nombre d'éléments effectivement lus.
 * 
 * @param p       Pointeur vers la zone mémoire où les données lues seront stockées.
 * @param taille  Taille (en octets) d'un élément à lire.
 * @param nbelem  Nombre d'éléments à lire.
 * @param f       Pointeur vers la structure FICHIER représentant le fichier à lire.
 * @return        Nombre d'éléments effectivement lus, ou -1 en cas d'erreur.
 */
int lire(void *p, unsigned int taille, unsigned int nbelem, FICHIER *f) {
    if (!f || f->mode != 'R' || !p || taille == 0 || nbelem == 0) {
        // Paramètres invalides ou fichier non ouvert en lecture
        return -1;
    }

    size_t total_bytes = taille * nbelem;
    size_t bytes_read = 0;
    size_t bytes_left = total_bytes;

    // Lire les données disponibles dans le tampon
    if (f->buf_pos < f->buf_size) {
        size_t buffer_bytes = f->buf_size - f->buf_pos;
        size_t to_copy = (bytes_left < buffer_bytes) ? bytes_left : buffer_bytes;

        memcpy(p, f->tampon + f->buf_pos, to_copy);
        f->buf_pos += to_copy;
        bytes_read += to_copy;
        bytes_left -= to_copy;

        if (bytes_left == 0) {
            return bytes_read / taille;
        }
    }

    // Lire les données depuis le fichier
    while (bytes_left > 0) {
        ssize_t n = read(f->fd, f->tampon, BUFFER_SIZE);
        if (n == 0) {
            // Fin de fichier
            f->eof = 1;
            break;
        }
        if (n < 0) {
            // Erreur de lecture
            return -1;
        }

        f->buf_size = n;
        f->buf_pos = 0;

        size_t to_copy = (bytes_left < (size_t)n) ? bytes_left : (size_t)n;
        memcpy((char *)p + bytes_read, f->tampon, to_copy);

        bytes_read += to_copy;
        bytes_left -= to_copy;
    }

    return bytes_read / taille;
}



// 4 - Fonction pour écrire des données dans un fichier
int ecrire(const void *p, unsigned int taille, unsigned int nbelem, FICHIER *f) {
    if (!f || f->mode != 'W' || !p || taille == 0 || nbelem == 0) {
        // Paramètres invalides ou fichier non ouvert en écriture
        return -1;
    }

    size_t total_bytes = taille * nbelem;
    size_t bytes_written = 0;

    while (bytes_written < total_bytes) {
        if (f->buf_pos == BUFFER_SIZE) {
            // Si le tampon est plein, le vider
            if (vider(f) == -1) {
                return -1;
            }
        }

        size_t to_copy = (total_bytes - bytes_written < BUFFER_SIZE - f->buf_pos) 
                         ? total_bytes - bytes_written 
                         : BUFFER_SIZE - f->buf_pos;

        memcpy(f->tampon + f->buf_pos, (char *)p + bytes_written, to_copy);
        f->buf_pos += to_copy;
        bytes_written += to_copy;
    }

    return bytes_written / taille;
}


// 5 - Fonction pour vider le tampon d'écriture
int vider(FICHIER *f) {
    if (!f || f->mode != 'W') {
        // Fichier invalide ou mode incorrect
        return -1;
    }

    ssize_t bytes_written = write(f->fd, f->tampon, f->buf_pos);
    if (bytes_written == -1) {
        // Erreur lors de l'écriture
        return -1;
    }

    f->buf_pos = 0; // Réinitialiser le tampon après l'écriture
    return 0;
}
