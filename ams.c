#include "define.h"
#include "ams.h"
#include <string.h>

notes convertSymbolesNotes[] = {{"C1", 1}, {"C1#", 2}, {"D1", 3}, {"D#1", 4}, {"E1", 5}, {"F1", 6}, {"F1#", 7}, {"G1", 8}, {"G1#", 9}, {"A1", 10}, {"A1#", 11}, {"B1", 12}, {"C2", 13}, {"C2#", 14}, {"D2", 15}, {"D2#", 16}, {"E2", 17}, {"F2", 18}, {"F2#", 19}, {"G2", 20}, {"G2#", 21}, {"A2", 22}, {"A2#", 23}, {"B2", 24}, {"C3", 25}, {"C3#", 26}, {"D3", 27}, {"D3#", 28}, {"E3", 29}, {"F3", 30}, {"F3#", 31}, {"G3", 32}, {"G3#", 33}, {"A3", 34}, {"A3#", 35}, {"B3", 36}, {"C4", 37}, {"C4#", 38}, {"D4", 39}, {"D4#", 40}, {"E4", 41}, {"F4", 42}, {"F4#", 43}, {"G4", 44}, {"G4#", 45}, {"A4", 46}, {"A4#", 47}, {"B4", 48}, {"C5", 49}, {"C5#", 50}, {"D5", 51}, {"D5#", 52}, {"E5", 53}, {"F5", 54}, {"F5#", 55}, {"G5", 56}, {"G5#", 57}, {"A5", 58}, {"A5#", 59}, {"B5", 60}};

#ifdef _WIN32
size_t getline(char **lineptr, size_t *n, FILE *stream) {
    char *bufptr = NULL;
    char *p = bufptr;
    size_t size;
    int c;

    if (lineptr == NULL) {
        return -1;
    }
    if (stream == NULL) {
        return -1;
    }
    if (n == NULL) {
        return -1;
    }
    bufptr = *lineptr;
    size = *n;

    c = fgetc(stream);
    if (c == EOF) {
        return -1;
    }
    if (bufptr == NULL) {
        bufptr = malloc(128);
        if (bufptr == NULL) {
            return -1;
        }
        size = 128;
    }
    p = bufptr;
    while(c != EOF) {
        if ((p - bufptr) > (size - 1)) {
            size = size + 128;
            bufptr = realloc(bufptr, size);
            if (bufptr == NULL) {
                return -1;
            }
        }
        *p++ = c;
        if (c == '\n') {
            break;
        }
        c = fgetc(stream);
    }

    *p++ = '\0';
    *lineptr = bufptr;
    *n = size;

    return p - bufptr - 1;
}
#endif

char* replace_char(char* str, char find, char replace){
    char *current_pos = strchr(str,find);
    while (current_pos) {
        *current_pos = replace;
        current_pos = strchr(current_pos,find);
    }
    return str;
}

s_song readAMS(char* fileName){
        //Définir les variables locales
        s_song mySong;
        //mySong = (s_song)malloc(sizeof(s_song));//Il faut liberer la memoire à l'adresse retournée en fin de prgrm
        int charRead;
        s_tick myCurrentTick;
        int tickNumber = 0;//Conteur du nombre de tick
        int noteNumber = 0;//Conteur du nombre de note dans le tick 
        int accordNumber = 0;//Conteur du nombre de note jouée dans le tick
        int sizeLine = 0;
        char* line = NULL;
        FILE* myFile = fopen(fileName, "r");
        if (myFile == NULL){
                mySong.tpm = 0;
                strncpy(mySong.title, "", MAX_SIZE_TITLE);
                for (int i = 0; i<MAX_NUMBER_TICKS;i++){
                        mySong.tickTab[i].accent = 0;
                        for (int j=0; j <4;j++){
                                mySong.tickTab[i].note[j] = 0;
                        }
                }
                return mySong;
        }
        //Lire le titre de la musique
        getline(&line, &sizeLine, myFile);
        strcpy(line, replace_char(line, '\n', '\0'));
        strcpy(line, replace_char(line, '\r', '\0'));
        strcpy(mySong.title, line);
        //Lire le nombre de ticks par minutes de la musique
        getline(&line, &sizeLine, myFile);
        mySong.tpm = 2*atoi(line);
        //Ligne vide
        getline(&line, &sizeLine, myFile);
        //Header du tableau
        getline(&line, &sizeLine, myFile);
	//Première ligne
	//getline(&line, &sizeLine, myFile);
        //Lecture des ticks, lecture charactère par caractère
        while((charRead = fgetc(myFile)) != 'EOF'){
                // Enumération des cas en fonction du caractère lu

		//Ligne vide 
		if (strcmp(replace_char(replace_char(line, '\n', '\0'), '\r', '\0'), "") == 0){
			continue;
		}
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
	tickNumber--;
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
        strcpy(line, replace_char(line, '\r', '\0'));
        strcpy(line, replace_char(line, '\n', '\0'));
	strcpy(title, line);
        //Récupérer le bpm
        getline(&line, &sizeLine, txtFile);
        strcpy(line, replace_char(line, '\n', '\0'));
	strcpy(line, replace_char(line, '\r', '\0'));
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
		
		strcpy(line, replace_char(line, '\n', '\0'));
		strcpy(line, replace_char(line, '\r', '\0'));
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
	strcat(amsFileContent, "\r\n");
        //Ecrire les bpm
        strcat(amsFileContent, bpm);
        
	strcat(amsFileContent, "\r\n\r\n    ");
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
        strcat(amsFileContent, "\r\n");
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
        //printf("%s", amsFileContent);
        fputs(amsFileContent, amsFile);
        free(amsFileContent);
        fclose(amsFile);

}
