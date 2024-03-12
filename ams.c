#include "define.h"
#include "ams.h"
#include <string.h>

s_song readAMS(char* fileName){
	//Définir les variables locales
        s_song mySong;
        int charRead;
        s_tick myCurrentTick;
        int tickNumber = 0;
        int noteNumber = 0;
        int accordNumber = 0;
	int sizeLine = 0;
        FILE* myFile = fopen(fileName, "r");
	char* line = NULL;
	//Lire le titre de la musique
	getline(&line, &sizeLine, myFile);
	strcpy(mySong.title, line);
	//Lire le nombre de ticks par minutes de la musique
	getline(&line, &sizeLine, myFile);
	mySong.tpm = 2*atoi(line);
	//Ligne vide
	getline(&line, &sizeLine, myFile);
	//Header du tableau
	getline(&line, &sizeLine, myFile);
	//Lecture des ticks, lecture charactère par caractère
	while((charRead = fgetc(myFile)) != 'EOF'){
                // Enumération des cas en fonction du caractère lu
		// Caractère de fin
		if (charRead == -1){
			mySong.tickTab[tickNumber] = myCurrentTick;
			tickNumber++;
			break;
		}
		// Saut de ligne => fin du tick en cours
		if (charRead == '\n'){
                        mySong.tickTab[tickNumber] = myCurrentTick;
                        myCurrentTick.accent = 0;
                        for (int i=0;i< 4;i++){
				myCurrentTick.note[i] = 0;
			}
                        accordNumber = 0;
                        noteNumber = 0;
			tickNumber++;
                }
		// Changement de note
                else if (charRead == '|'){
                        noteNumber++;
                }
		// Note jouée dans le tick
                else if (charRead == '^' || charRead == 'X' || charRead == 'x'){
                        if (charRead == '^'){
                                myCurrentTick.accent = 1;
                        }
                        myCurrentTick.note[accordNumber] = noteNumber;
                        accordNumber++;
                }
        }
	free(line);
	fclose(myFile);
	//Ajout d'un tick null (tick "de fin")
	myCurrentTick.accent = 0;
	for (int i=0;i<4;i++){
		myCurrentTick.note[i] = 0;
	}
	mySong.tickTab[tickNumber] = myCurrentTick;
	mySong.nTicks = tickNumber;
        return mySong;


}

int findNoteFromSymbol(char* symbol){
	for (int i=0;i < sizeof(convertSymbolesNotes)/sizeof(notes);i++){
		if(strcmp(convertSymbolesNotes[i].symbole, symbol)){
			return convertSymbolesNotes[i].note;
		}
	}
}

void createAMS(char* txtFileName, char* amsFileName){
	char title[MAX_SIZE_TITLE] = {0};
	char bpm[3];
	char* line = NULL;
	FILE* txtFile = fopen(txtFileName, "r");
	FILE* amsFile = fopen(amsFileName, "w");
	int noteRondeCounter = 0;
	char noteRondeBuffer[3] = {0};
	int noteAccentBuffer[60] = {0};
	int croixOuChapo[MAX_NUMBER_TICKS][60] = {0}; // 1 => croix; 2 => chapo 
	int sizeLine;
	int tickCounter = 0;
	int  myTicksList[MAX_NUMBER_TICKS][4] = {0};
	//Récupérer le titre
	getline(&line, &sizeLine, txtFile);
	strcpy(title, line);
	//Récupérer le bpm
	getline(&line, &sizeLine, txtFile);
	strcpy(bpm, line);
	//Saut de ligne
	getline(&line, &sizeLine, txtFile);
	//Lire les tick
	int len;
	while((len = getline(&line, &sizeLine, txtFile)) != -1){
		int myTick[4] = {0};
		char symbol[3];
		int accent;
		int charRead;
		int symbolCounter = 0;
		int readingSymbol = 1;
		int noteCounter = 0;
		for (int i =0; i< 60; i++){
			if (noteAccentBuffer[i] != 0){
				myTick[noteCounter] = i+1;
				myTicksList[tickCounter][noteCounter] = i+1;
				noteCounter++;
				noteAccentBuffer[i]--;
				croixOuChapo[tickCounter][i] = 1;
			}
		}
		//Lire les notes
		for (int i = 0; i< len;i++){
			charRead = line[i];
			if (readingSymbol == 1 && charRead != ',' && charRead != ' '){
				symbol[symbolCounter] = charRead;
				symbolCounter++;
			}
			else if (readingSymbol == 1 && charRead == ' '){
				readingSymbol = 0;
				symbolCounter = 0;
				myTick[noteCounter] = findNoteFromSymbol(symbol);
				myTicksList[tickCounter][noteCounter] = myTick[noteCounter];
				croixOuChapo[tickCounter][myTick[noteCounter]-1] = 2;
				noteCounter++;
			}
			else if (readingSymbol == 0 && charRead != ',' && charRead != ' '){
				if (charRead == 'R'){
					noteAccentBuffer[myTick[noteCounter-1]-1] = 7;
				}
				else if (charRead == 'B'){
					noteAccentBuffer[myTick[noteCounter-1]-1] = 3;
				}
				else if (charRead == 'N'){
					noteAccentBuffer[myTick[noteCounter-1]-1] = 1;
				}
			}
			else if (readingSymbol == 0 && charRead == ' '){
				readingSymbol = 1;
			}
		}
		//strncpy(myTicksList[tickCounter], myTick, 12);
		tickCounter++;
	}
	fclose(txtFile);
	fclose(amsFile);
	free(line);

}
//trm
int main(){
        createAMS("fichiers_musicaux/bohemian_rhapsody.txt", "fichiers_musicaux/bohemian_rhapsody_moi.ams");
        return 0;
}
