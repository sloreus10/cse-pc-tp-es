#ifndef _STDES_H
#define _STDES_H

#define BUFFER_SIZE 1024 // 1KB la taille du buffer

struct _ES_FICHIER;
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
