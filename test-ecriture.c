#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stdes.h" // Assurez-vous que ce fichier contient vos définitions de types et de fonctions

void ecrire_fichier(const char *nom_fichier) {
    // Ouvrir le fichier en mode écriture ('W')
    FICHIER *fichier = ouvrir(nom_fichier, 'W');
    if (fichier == NULL) {
        perror("Erreur d'ouverture du fichier");
        return;
    }

    char ligne[256];
    printf("Entrez du texte à écrire dans le fichier (Ctrl+D pour terminer) :\n");

    // Lire et écrire ligne par ligne
    while (fgets(ligne, sizeof(ligne), stdin)) {
        // Écrire la ligne dans le fichier
        if (ecrire(ligne, sizeof(char), strlen(ligne), fichier) == -1) {
            perror("Erreur lors de l'écriture dans le fichier");
            fermer(fichier);
            return;
        }
    }

    // Fermer le fichier
    if (fermer(fichier) == -1) {
        perror("Erreur lors de la fermeture du fichier");
    }
}

int main() {
    char nom_fichier[256];
    printf("Entrez le nom du fichier à écrire: ");
    scanf("%s", nom_fichier);
    getchar(); // Consomme le '\n' laissé par scanf

    ecrire_fichier(nom_fichier);

    return 0;
}
