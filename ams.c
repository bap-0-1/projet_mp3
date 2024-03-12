#include "define.h"
#include "ams.h"
#include <string.h>

s_song readAMS(char* fileName){
	//Définir les variables locales
        s_song mySong;
        int charRead;
        s_tick myCurrentTick;
        int tickNumber = 0;//Conteur du nombre de tick
        int noteNumber = 0;//Conteur du nombre de note dans le tick 
        int accordNumber = 0;//Conteur du nimbre de note jouée dans le tick
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
		if (charRead == -1 || charRead == '%'){
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

int findNoteFromSymbol(char symbol[4]){
	for (int i=0;i < 60;i++){
		if(strcmp(convertSymbolesNotes[i].symbole, symbol) == 0){
			return convertSymbolesNotes[i].note;
		}
	}
	return 0;
}

void createAMS(char* txtFileName, char* amsFileName){
	int sizeLine;//Variable inutile
	int len;//Taille de la ligne lue
	int tickCounter = 0;//Conteur du nombre de tick (1 tick = 1 demi-temps)
	int partition[MAX_NUMBER_TICKS][60] = {0}; // 1 => croix; 2 => chapo; 0 => pas de notes 
	int noteAccentBuffer[60] = {0};//Conteur du nombre de note prolongée en fonction de l'accentuation
	char* amsFileContent = NULL; // Zone d'ecriture pour le contenu du fichier ams
	char title[MAX_SIZE_TITLE] = {0};//Variable de stockage du titre
	char bpm[4] = {0};//Variable de stockage des bpm
	char* line = NULL;//Variable de stockage des lignes
	FILE* txtFile = fopen(txtFileName, "r");//Pointeur de fichier
	FILE* amsFile = fopen(amsFileName, "w");//Pointeur de fichier
	//Récupérer le titre
	getline(&line, &sizeLine, txtFile);
	strcpy(title, line);
	//Récupérer le bpm
	getline(&line, &sizeLine, txtFile);
	strncpy(bpm, line, 3);
	//Saut de ligne
	getline(&line, &sizeLine, txtFile);
	//Boucle de lecture des ticks (lecture ligne par ligne)
	while((len = getline(&line, &sizeLine, txtFile)) != -1){
		int charRead;
		int myNote;//Stockage de la note sous forme numérique
		char symbol[4] = {0};//Stockage de la note sous forme de caractères
		int symbolCounter = 0;//Conteur du nombre de caractère pour les notes
		int readingSymbol = 1; //Booleen de lecture de note ou d'accentutation; 1=> lecture note et 0 => lecture accentuation
		//Ajout d'une note si une note sur plus d'un demi-temps (Blanche, Noire, Ronde) a été jouée
		for (int i =0; i< 60; i++){
			if (noteAccentBuffer[i] != 0){
				noteAccentBuffer[i]--;
				partition[tickCounter][i] = 1;
			}
		}
		//Lire les notes (lecture caractère par caractères)
		for (int i = 0; i< len;i++){
			charRead = line[i];
			//Si on lit une note
			if (readingSymbol == 1 && charRead != ',' && charRead != ' '){
				symbol[symbolCounter] = charRead;
				symbolCounter++;
			}
			//Si on a finit de lire une note
			else if (readingSymbol == 1 && charRead == ' '){
				readingSymbol = 0;
				symbolCounter = 0;
				myNote = findNoteFromSymbol(symbol);
				partition[tickCounter][myNote-1] = 2;
				strncpy(symbol, "", 3);
			}
			//Si on lit une accentuation (note sur plus d'un demi-temps), ajout de la note sur les ticks suivant
			else if (readingSymbol == 0 && charRead != ',' && charRead != ' '){
				if (charRead == 'R'){
					noteAccentBuffer[myNote-1] = 7;
				}
				else if (charRead == 'B'){
					noteAccentBuffer[myNote-1] = 3;
				}
				else if (charRead == 'N'){
					noteAccentBuffer[myNote-1] = 1;
				}
			}
			//Sinon si on lit un espace en mode lecture d'accentuation => c'est lespace juste avant la prochaine note, on passe donc en mode lecture de note
			else if (readingSymbol == 0 && charRead == ' '){
				readingSymbol = 1;
			}
		}
		tickCounter++;//Passage au prochain tick
	}
	fclose(txtFile);
	free(line);
	//Ecrire le fichier AMS
	
	amsFileContent = (char *)malloc(MAX_SIZE_LINE*(tickCounter+3));
	//Vider le contenu de amsFileContent
	strncpy(amsFileContent, "\0", MAX_SIZE_LINE*(tickCounter+3));
	//Ecrire le titre
	strcat(amsFileContent, title);
	//Ecrire les bpm
	strcat(amsFileContent, bpm);
	strcat(amsFileContent, "\n\n    ");
	//Ecrire la ligne d'en-tête du fichier ams
	for (int i=1; i <=60;i++){
		char toConcatenate[3] = {0};
		if (i < 10){
			toConcatenate[0] = 0x30;
			toConcatenate[1] = i+0x30;
		}
		else{
			sprintf(toConcatenate, "%d", i);
		}
		toConcatenate[2] = 0x20;
		strcat(amsFileContent, toConcatenate);
	}
	strcat(amsFileContent, "\n");
	//Ecrire les lignes correspondants à chaques demi-temps (demi-temps = 1 tick)
	for (int i= 1; i <= tickCounter; i++){
		char currentLine[MAX_SIZE_LINE] = {0};
		char buffer[4] = {0};
		//Ajout des 0 de débuts de ligne en fonction du numéro du tick
		if (i < 100){
			if (i < 10){
				strcat(currentLine, "0");
			}
			strcat(currentLine, "0");
		}
		sprintf(buffer, "%d", i);
		strcat(currentLine, buffer);
		strcat(currentLine, "|");
		//Ecrire les notes de chaque tick en fonction des informations stockes dans la partition
		for (int j = 0; j < 60; j++){
			if (partition[i-1][j] == 1){
				strcat(currentLine, "x |");
			}
			else if(partition[i-1][j] == 2){
				strcat(currentLine, "^ |");
			}
			else if (partition[i-1][j] == 0){
				strcat(currentLine, "  |");
			}
		}
		strcat(currentLine, "\n");
		strcat(amsFileContent, currentLine); 
	}
	printf("%s", amsFileContent);
	free(amsFileContent);
	fclose(amsFile);

}
