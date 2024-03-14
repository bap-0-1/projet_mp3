#include "define.h"
#include "frame.h"
#include "ams.h"
#include "amp.h"
#include "usb.h"

int main(){
	char playListName[] = "fichiers_musicaux/Playlist.amp";
	char frmFileName[] = "fichiers_musicaux/Playlist.frm";
	FILE* myPlayListFile = initAMP(playListName);
	char songFileName[MAX_SIZE_TITLE] = {0};
	//Ouverture du flux usb
	FT_HANDLE ftHandle = initUSB();
	while(!feof(myPlayListFile)){
		FILE * amsFile;
		char amsFileNameWithFolder[MAX_SIZE_TITLE+18] = "fichiers_musicaux/";
		char line[MAX_SIZE_LINE] = {0};
		s_song mySong;
		readAMP(myPlayListFile, songFileName);
		strcat(amsFileNameWithFolder, songFileName);
		amsFile = fopen(amsFileNameWithFolder, "r");
		if (amsFile == NULL){
			FILE * txtFile;
			char txtFileName[MAX_SIZE_TITLE];
			strcpy(txtFileName, amsFileNameWithFolder);
			txtFileName[strlen(txtFileName)-1] = 't';
			txtFileName[strlen(txtFileName)-2] = 'x';
			txtFileName[strlen(txtFileName)-3] = 't';
			txtFile = fopen(txtFileName, "r");
			if (txtFile == NULL){
				printf("Erreur, mauvais chemin de fichier");
				return 1;
			}
			fclose(txtFile);
			createAMS(txtFileName, amsFileNameWithFolder);

		}
		mySong = readAMS(amsFileNameWithFolder);
		fclose(amsFile);
		createInitFrame(mySong, line);
		//Ecriture de la trame dans le flux de données usb
		writeUSB(line, ftHandle);
		for (int i = 0; i < mySong.nTicks;i++){
			strcpy(line, "");
			createTickFrame(mySong.tickTab[i], line);
			writeUSB(line, ftHandle);
		}

	}
	closeAMP(myPlayListFile);
	closeUSB(ftHandle);
	return 0;
}
