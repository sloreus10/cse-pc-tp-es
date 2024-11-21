# Compilateur utilisé
CC = gcc

# Options de compilation
CFLAGS = -Wall -Wextra -std=c11

# Nom de l'exécutable
TARGETS = test test-lecture test-ecriture test-copier-coller

# Fichiers source du projet
SRCS = stdes.c test.c test-lecture.c test-ecriture.c test-copier-coller.c

# Fichiers objets générés à partir des fichiers sources
OBJS = $(SRCS:.c=.o)

# Cible par défaut : compilation complète
all: $(TARGETS)

# Règle pour créer l'exécutable "test" à partir des fichiers objets
test: stdes.o test.o
	$(CC) $(CFLAGS) -o $@ stdes.o test.o

# Règle pour créer l'exécutable "test-lecture" à partir des fichiers objets
test-lecture: stdes.o test-lecture.o
	$(CC) $(CFLAGS) -o $@ stdes.o test-lecture.o

# Règle pour créer l'exécutable "test-ecriture" à partir des fichiers objets
test-ecriture: stdes.o test-ecriture.o
	$(CC) $(CFLAGS) -o $@ stdes.o test-ecriture.o

# Règle pour créer l'exécutable "test-copier-coller" à partir des fichiers objets
test-copier-coller: stdes.o test-copier-coller.o
	$(CC) $(CFLAGS) -o $@ stdes.o test-copier-coller.o

# Règle pour compiler les fichiers sources en fichiers objets
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Règle pour supprimer les fichiers intermédiaires et l'exécutable
clean:
	@echo "Suppression des fichiers intermédiaires et de l'exécutable..."
	rm -f $(OBJS) $(TARGETS)

# Règle pour tout reconstruire depuis zéro
rebuild: clean all

# Indique que ces cibles ne correspondent pas à des fichiers réels
.PHONY: all clean rebuild
