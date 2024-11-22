#include "stdes.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>  // Pour write()

int main(int argc, char *argv[]) {
    FICHIER *f1;
    FICHIER *f2;
    char c;  // Variable pour stocker un caractère

    // Initialiser les flux globaux
    initialiser_flux();  // Initialisation de my_stdout

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

    ssize_t bytesRead;  // Nombre d'octets effectivement lus
    ssize_t taille_total_read_write = 0;  // Compteur pour le nombre total d'éléments lus et écrits

    // Lire et écrire les données caractère par caractère
    while ((bytesRead = lire(&c, 1, 1, f1)) == 1) {  // Lire un élément (1 octet) à la fois
        // Afficher le caractère lu
        printf("Élement lu : %c\n", c);

        // Écrire les éléments dans le fichier destination
        ssize_t bytesWritten = ecrire(&c, 1, 1, f2);

        // Vérification d'écriture
        if (bytesWritten == -1) {
            perror("Erreur lors de l'écriture dans le fichier destination");
            fermer(f1);
            fermer(f2);
            exit(EXIT_FAILURE);
        }

        // Afficher le caractère écrit
        printf("Élement écrit : %c\n", c);

        // Mettre à jour le compteur total
        taille_total_read_write += bytesWritten;

        // Si le tampon est plein, il faut le vider
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
        printf("\nLe nombre total des éléments lus et écrits est : %ld\n", taille_total_read_write);
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