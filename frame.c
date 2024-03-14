#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "frame.h"
#include "define.h"
#define MAX_TRAM_SIZE 1000


void createInitFrame(s_song mySong, char* frame){
	//char myTram[MAX_TRAM_SIZE] = {0};
	strcpy(frame, "");
	int i;
	char buffer[4] = {0};
	int myXorCheckSum = 0;
	//Création du contenu de la tram 
	frame[0] = '#';
	strcat(frame, mySong.title);
	strcat(frame, ",");
	sprintf(buffer, "%d", mySong.tpm);
	strcat(frame, buffer);
	strcat(frame, ",");
	sprintf(buffer, "%d", mySong.nTicks);
	strcat(frame, buffer);
	//Opération du xor
	for ( i =1; i< strlen(frame); i++){
		myXorCheckSum ^= frame[i];
	}
	strcat(frame, "*");
	sprintf(buffer, "%.2x", myXorCheckSum);
	strcat(frame, buffer);
	strcat(frame, "\r\n");

}

void createTickFrame(s_tick myTick, char* frame){

}
