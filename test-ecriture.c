#include <stdio.h>
#include <stdlib.h>

void ecrire_fichier(const char *nom_fichier) {
    FILE *fichier = fopen(nom_fichier, "w"); // Ouvre le fichier en mode écriture
    if (fichier == NULL) {
        perror("Erreur d'ouverture du fichier");
        return;
    }

    char ligne[256];
    printf("Entrez du texte à écrire dans le fichier (Ctrl+D pour terminer) :\n");

    // Lire et écrire ligne par ligne
    while (fgets(ligne, sizeof(ligne), stdin)) {
        fputs(ligne, fichier); // Écrire la ligne dans le fichier
    }

    fclose(fichier); // Fermer le fichier
}

int main() {
    char nom_fichier[256];
    printf("Entrez le nom du fichier à écrire: ");
    scanf("%s", nom_fichier);
    getchar(); // Consomme le '\n' laissé par scanf

    ecrire_fichier(nom_fichier);

    return 0;
}
