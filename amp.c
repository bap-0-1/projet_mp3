#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "amp.h"

#define MAX_SIZE_TITLE 40

// Tentative d'ouverture du fichier playlist.amp
FILE* initAMP(char* filename) {
    FILE* pf = fopen(filename, "r"); // Ouvre le fichier en lecture
    if (pf == NULL) {
        return NULL; // Échec de l'ouverture, renvoie NULL
    }
    return pf; // Succès, renvoie le pointeur vers le fichier
}

// Fonction d'aide pour formater le nom du morceau
void formatSongName(char* input, char* output) {
    int i, j = 0;
    char lastChar = ' ';
    for (i = 0; input[i] != '\0' && i < MAXSIZE_TITLE; i++) {
        char c = input[i];
        // Convertit chaque caractère en minuscule
        c = tolower(c);
        // Remplace les caractères non alphanumériques par ''
        if (!isalnum(c)) {
            if (lastChar != '') { // Évite les répétitions d'underscores
                output[j++] = '';
                lastChar = '';
            }
        } else {
            output[j++] = c;
            lastChar = c;
        }
    }
    if (output[j-1] == '') j--; // Supprime le caractère '_' à la fin si présent
    output[j] = '\0'; // Termine la chaîne de caractères proprement
}

// Implémentation de readAMP
void readAMP(FILE* pf, char* song_name) {
    char buffer[1001]; // Taille arbitraire, peut être ajustée
    if (fgets(buffer, MAX_SIZE_TITLE, pf) == NULL) {
        *song_name = '\0'; // Si fin de fichier, song_name est vide
        return;
    }
    buffer[strcspn(buffer, "\n")] = '\0'; // Supprime le retour à la ligne
    formatSongName(buffer, song_name); // Formate le nom selon les critères
}


void closeAMP(FILE* pf) {
    if (pf != NULL) { // Vérifie que le fichier est ouvert
        fclose(pf); // Ferme le fichier
    }
}

/*int main() {
    char filename[] = "playlist.amp";
    FILE* pf = initAMP(filename);
    if (pf == NULL) {
        printf("Erreur lors de l'ouverture du fichier %s.\n", filename);
        return EXIT_FAILURE;
    }

    char song_name[MAX_SIZE_TITLE + 1]; // Taille conforme à celle utilisée dans readAMP
    while (1) {
        readAMP(pf, song_name);
        if (song_name[0] == '\0') break; // Si readAMP renvoie une chaîne vide, fin de la playlist
        printf("%s.ams\n", song_name); // Affiche le nom du fichier .ams correspondant
    }

    closeAMP(pf); // Ferme le fichier
    return EXIT_SUCCESS;
}*/
