#include "stdes.h"
#include <stdlib.h>
//#include <stdio.h>

#define BUFFER_SIZE 1024  // Taille du tampon

int main(int argc, char *argv[])
{
    // Initialiser les flux globaux
    initialiser_flux();

    FICHIER *f1;
    FICHIER *f2;
    char buffer[BUFFER_SIZE];
    int nbelem_lus;

    if (argc != 3) {
        //fprintf(stderr, "Usage : %s <fichier source> <fichier destination>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Ouvrir les fichiers source et destination
    f1 = ouvrir(argv[1], 'R');
    if (f1 == NULL) {
        //perror("Erreur lors de l'ouverture du fichier source");
        exit(EXIT_FAILURE);
    }

    f2 = ouvrir(argv[2], 'W');
    if (f2 == NULL) {
        //perror("Erreur lors de l'ouverture du fichier destination");
        fermer(f1);
        exit(EXIT_FAILURE);
    }

    // Lire et écrire en utilisant un tampon
    while ((nbelem_lus = lire(buffer, sizeof(char), BUFFER_SIZE, f1)) > 0) {
        // Écrire sur la console
        ecrire(buffer, sizeof(char), nbelem_lus, my_stdout);
        //fflush(stdout);  // S'assurer que les données sont affichées immédiatement

        // Écrire dans le fichier de destination
        if (ecrire(buffer, sizeof(char), nbelem_lus, f2) != nbelem_lus) {
            //perror("Erreur lors de l'écriture dans le fichier destination");
            fermer(f1);
            fermer(f2);
            exit(EXIT_FAILURE);
        }
    }

    if (nbelem_lus < 0) {
        //perror("Erreur lors de la lecture du fichier source");
    }

    // Fermer les fichiers
    fermer(f1);
    fermer(f2);

    return EXIT_SUCCESS;
}
