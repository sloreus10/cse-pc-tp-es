#include "stdes.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>  // Pour write()

int main(int argc, char *argv[]) {
    FICHIER *f1;
    FICHIER *f2;
    char buffer[16];  // Tampon pour stocker les éléments lus
    unsigned int element_size = 2;  // Taille de chaque élément en octets
    unsigned int element_count = 4; // Nombre d'éléments à lire/écrire par itération
    ssize_t bytesRead = 0;          // Nombre d'éléments lus
    ssize_t bytesWritten = 0;       // Nombre d'éléments écrits
    ssize_t total_elements_read = 0;    // Total des éléments lus
    ssize_t total_elements_written = 0; // Total des éléments écrits

    // Vérifier que le nombre d'arguments est correct
    if (argc != 3) {
        fprintf(stderr, "Usage: %s input_file output_file\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Ouvrir le fichier source en lecture
    f1 = ouvrir(argv[1], 'R');
    if (f1 == NULL) {
        perror("Erreur lors de l'ouverture du fichier source");
        exit(EXIT_FAILURE);
    }

    // Ouvrir le fichier destination en écriture
    f2 = ouvrir(argv[2], 'W');
    if (f2 == NULL) {
        perror("Erreur lors de l'ouverture du fichier destination");
        fermer(f1);
        exit(EXIT_FAILURE);
    }

    // Lire et écrire les données en utilisant le tampon
    while (1) {
        // Lire plusieurs éléments dans le tampon (jusqu'à la taille du tampon ou la fin du fichier)
        bytesRead = lire(buffer, element_size, element_count, f1);

        if (bytesRead > 0) {
            // Afficher les éléments lus
            printf("Nombre d'éléments lus : %ld\n", bytesRead);
            for (ssize_t i = 0; i < bytesRead; i++) {
                fwrite(buffer + i * element_size, element_size, 1, stdout);  // Afficher l'élément
            }
            printf("\n");

            // Écrire dans le fichier destination
            bytesWritten = ecrire(buffer, element_size, bytesRead, f2);

            // Vérification de l'écriture
            if (bytesWritten == -1) {
                perror("Erreur lors de l'écriture dans le fichier destination");
                fermer(f1);
                fermer(f2);
                exit(EXIT_FAILURE);
            }

            // Afficher le nombre d'éléments écrits
            printf("Nombre d'éléments écrits : %ld\n", bytesWritten);

            // Mettre à jour les totaux
            total_elements_read += bytesRead;
            total_elements_written += bytesWritten;
        } else {
            break; // Si aucun élément n'a été lu, on sort de la boucle
        }

        // Si le tampon d'écriture est plein, il faut le vider
        if (f2->buf_pos_write == BUFFER_SIZE) {
            if (vider(f2) == -1) {
                perror("Erreur lors du vidage du tampon dans f2");
                fermer(f1);
                fermer(f2);
                exit(EXIT_FAILURE);
            }
        }
    }

    // Vérifier si la fin du fichier a été atteinte
    if (bytesRead == 0) {
        printf("\nFin du fichier atteinte.\n");
        printf("Le nombre total des éléments lus est : %ld\n", total_elements_read);
        printf("Le nombre total des éléments écrits est : %ld\n", total_elements_written);
    } else {
        perror("Erreur de lecture");
    }

    // Vider tout ce qui reste dans le tampon de f2
    if (f2->buf_pos_write > 0) {
        if (vider(f2) == -1) {
            perror("Erreur lors du vidage final du tampon dans f2");
            fermer(f1);
            fermer(f2);
            exit(EXIT_FAILURE);
        }
    }

    // Fermer les fichiers
    fermer(f1);
    fermer(f2);

    return 0;
}
