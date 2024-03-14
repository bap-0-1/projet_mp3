#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "define.h"

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
    for (i = 0; input[i] != '\0' && i < MAX_SIZE_TITLE; i++) {
	while(input[i] == ' ' &&(i == 0 || lastChar == ' ')){
		lastChar = input[i];
		i++;
	}
        char c = input[i];
        // Convertit chaque caractère en minuscule
        c = tolower(c);
        // Remplace les caractères non alphanumériques par '_'
        if (!isalnum(c)) {
            if (lastChar != '_') { // Évite les répétitions d'underscores
                output[j++] = '_';
                lastChar = '_';
            }
        } else {
            output[j++] = c;
            lastChar = c;
        }
    }
    if (output[j-1] == '_') j--; // Supprime le caractère '_' à la fin si présent
    output[j++] = '.';
    output[j++] = 'a';
    output[j++] = 'm';
    output[j++] = 's';
    output[j] = '\0'; // Termine la chaîne de caractères proprement
}

// Implémentation de readAMP
void readAMP(FILE* pf, char* song_name) {
    char buffer[1001]; // Taille arbitraire, peut être ajustée
    if (fgets(buffer, 1000, pf) == NULL) {
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
