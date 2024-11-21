#include "stdes.h"
#include <stdlib.h>
#include <stdio.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    FICHIER *f1;
    FICHIER *f2;
    char buffer[BUFFER_SIZE];
    size_t bytesRead;

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source_file> <destination_file>\n", argv[0]);
        exit(-1);
    }

    f1 = ouvrir(argv[1], 'R');
    if (f1 == NULL) {
        perror("Error opening source file");
        exit(-1);
    }

    f2 = ouvrir(argv[2], 'W');
    if (f2 == NULL) {
        perror("Error opening destination file");
        fermer(f1); // Close the source file before exiting
        exit(-1);
    }

    while ((bytesRead = lire(buffer, 1, BUFFER_SIZE, f1)) > 0) {
        ecrire(buffer, 1, bytesRead, stdout);
        ecrire(buffer, 1, bytesRead, f2);
    }

    // Flush the output buffer to ensure everything is printed
    fflush(stdout);

    fermer(f1);
    fermer(f2);

    return 0;
}
