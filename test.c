#include "stdes.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>  // Pour write()

int main(int argc, char *argv[]) {
    // Initialiser les flux globaux
    initialiser_flux();

    FICHIER *f1;
    FICHIER *f2;

    // Vérifier les arguments en ligne de commande
    if (argc != 3) {
        fprintf(stderr, "Usage: %s input_file output_file\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Ouvrir le fichier source en lecture
    f1 = ouvrir(argv[1], 'R');
    if (f1 == NULL) {
        perror("Erreur lors de l'ouverture de f1");
        exit(EXIT_FAILURE);
    }

    // Ouvrir le fichier destination en écriture
    f2 = ouvrir(argv[2], 'W');
    if (f2 == NULL) {
        perror("Erreur lors de l'ouverture de f2");
        fermer(f1);
        exit(EXIT_FAILURE);
    }

    // Définir la taille des éléments (par exemple 2 octets)
    size_t element_size = 2;  // Exemple: Taille de chaque élément (2 octets)
    size_t element_count = 4;  // Exemple: Nombre d'éléments à lire/écrire par itération

    // Allocation du tampon utilisateur pour stocker les éléments lus
    char buffer[element_size * element_count];  // Le tampon contient 4 éléments de 2 octets chacun

    ssize_t bytesRead;  // Variable pour stocker le nombre d'éléments lus
    ssize_t taille_total_read_write = 0;  // Compteur pour le nombre total d'éléments lus et écrits

    // Lire et écrire les données en blocs
    while ((bytesRead = lire(buffer, element_size, element_count, f1)) > 0) {
        // Afficher le nombre d'éléments lus
        printf("\nNombre d'éléments lus : %ld\n", bytesRead);

        // Afficher les éléments lus sur stdout (écrire dans le terminal)
        write(STDOUT_FILENO, buffer, bytesRead * element_size);  // Affiche les éléments lus (en octets)

        // Écrire les éléments dans le fichier destination
        ssize_t bytesWritten = ecrire(buffer, element_size, bytesRead, f2);

        // Afficher le nombre d'éléments écrits
        printf("\nNombre d'éléments écrits : %ld\n", bytesWritten);

        // Mettre à jour le compteur total
        taille_total_read_write += bytesWritten;
    }

    // Vérifier si la fin du fichier a été atteinte
    if (bytesRead == 0) {
        printf("\nFin du fichier atteinte.\n");
        printf("\nLe nombre total des éléments lus et écrits est : %ld\n", taille_total_read_write);
    } else {
        perror("Erreur de lecture");
    }

    // Fermer les fichiers
    fermer(f1);
    fermer(f2);

    return 0;
}
