#ifndef _STDES_H
#define _STDES_H

#define BUFFER_SIZE 1024 // 1KB la taille du buffer

// Définition complète de la structure FICHIER
struct _ES_FICHIER {
    int fd;                     // Descripteur de fichier
    char tampon[BUFFER_SIZE];    // Tampon pour les données
    unsigned int buf_size;      // Taille actuelle des données dans le tampon
    unsigned int buf_pos_read;       // Position actuelle dans le tampon de lecteure
    unsigned int buf_pos_write;       // Position actuelle dans le tampon d'écriture
    char mode;                  // Mode du fichier ('R' pour lecture, 'W' pour écriture)
    int eof;                    // Indicateur de fin de fichier
};

// Définition du type FICHIER
typedef struct _ES_FICHIER FICHIER;

extern FICHIER *my_stdout;
extern FICHIER *my_stderr;

// Initialisation des flux
void initialiser_flux(void);

/* mode: 'L' = lecture, 'E' = écriture */
FICHIER *ouvrir(const char *nom, char mode);
int fermer(FICHIER*f);
int lire(void *p, unsigned int taille, unsigned int nbelem, FICHIER *f);
int ecrire(const void *p, unsigned int taille, unsigned int nbelem, FICHIER *f);
int vider(FICHIER *f);

int fecriref (FICHIER *f, const char *format, ...);
/* directly in stdout */
int ecriref (const char *format, ...);
int fliref (FICHIER *f, const char *format, ...);

#endif
