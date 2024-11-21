#include <stdio.h>
#include <stdlib.h>

void lire_fichier(const char *nom_fichier) {
    FILE *fichier = fopen(nom_fichier, "r"); // Ouvre le fichier en mode lecture
    if (fichier == NULL) {
        perror("Erreur d'ouverture du fichier");
        return;
    }

    char ch;
    while ((ch = fgetc(fichier)) != EOF) { // Lire caractère par caractère
        putchar(ch); // Afficher chaque caractère
    }

    fclose(fichier); // Fermer le fichier
}

int main() {
    char nom_fichier[256];
    printf("Entrez le nom du fichier à lire: ");
    scanf("%s", nom_fichier);

    lire_fichier(nom_fichier);

    return 0;
}
