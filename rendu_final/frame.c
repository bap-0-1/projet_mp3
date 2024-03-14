#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "frame.h"
#include "define.h"
#define MAX_TRAM_SIZE 1000


void createInitFrame(s_song mySong, char* frame){
	//Vider le contenu de frame
	strncpy(frame, "", sizeof(frame));
	int i;
	char buffer[4] = {0};
	int myXorCheckSum = 0;
	//Création du contenu de la tram 
	frame[0] = '#';
	//Ajout du titre dans la frame
	strcat(frame, mySong.title);
	strcat(frame, ",");
	//Ajout du tpm 
	sprintf(buffer, "%d", mySong.tpm);//Fonction pour convertir une valeure numrique en char* %d pour preciser sous forme d'entier
	strcat(frame, buffer);
	strcat(frame, ",");
	sprintf(buffer, "%d", mySong.nTicks);
	strcat(frame, buffer);
	//Creation de la checksum
	for ( i =1; i< strlen(frame); i++){
		//Ajout de chaque caractère des données de la trame en utilisant l'operation xor sur la variable
		myXorCheckSum ^= frame[i];
	}
	strcat(frame, "*");
	//Ecriture de la checkSum dans la trame
	sprintf(buffer, "%.2x", myXorCheckSum);// %.2x utilisé pour preciser 2 chiffres et sous forme hexadecimale
	strcat(frame, buffer);
	// CR CL
	strcat(frame, "\r\n\0");

}

void createTickFrame(s_tick myTick, char* frame){
	strncpy(frame, "", sizeof(frame));
	int i;
	char buffer[4] = {0};
	int myXorCheckSum = 0;
	//Ecriture du debut de la trame
	frame[0] = '#';
	//Mode
	strcat(frame, "0");
	strcat(frame, ",");
	//Accent
	sprintf(buffer, "%d", myTick.accent);
	strcat(frame, buffer);
	//Ecriture des notes associées au tick
	for (int i = 0; i < 4; i++){
	
		strcat(frame, ",");
		sprintf(buffer, "%.2d", myTick.note[i]);
		strcat(frame, buffer);

	}
	//Creation de la checkSum
	for (i = 1;i < strlen(frame);i++){
		myXorCheckSum ^= frame[i];
	}
	strcat(frame, "*");
	sprintf(buffer, "%.2x", myXorCheckSum);
	strcat(frame, buffer);
	strcat(frame, "\r\n\0");
}
