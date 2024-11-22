#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stdes.h" // Assurez-vous que ce fichier contient vos définitions de types et de fonctions

void lire_fichier(const char *nom_fichier) {
    // Ouvrir le fichier en mode lecture ('R')
    FICHIER *fichier = ouvrir(nom_fichier, 'R');
    if (fichier == NULL) {
        perror("Erreur d'ouverture du fichier");
        return;
    }

    char ch;
    while (lire(&ch, sizeof(char), 1, fichier) > 0) { // Lire caractère par caractère
        putchar(ch); // Afficher chaque caractère
    }

    // Fermer le fichier
    if (fermer(fichier) == -1) {
        perror("Erreur lors de la fermeture du fichier");
    }
}

int main() {
    char nom_fichier[256];
    printf("Entrez le nom du fichier à lire: ");
    scanf("%s", nom_fichier);

    lire_fichier(nom_fichier);

    return 0;
}
