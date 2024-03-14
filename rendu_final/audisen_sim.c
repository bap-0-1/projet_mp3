#include "define.h"
#include "frame.h"
#include "ams.h"
#include "amp.h"


int main(){
	//Définition des variables locales
	char playListName[] = "fichiers_musicaux/Playlist.amp";
	char frmFileName[] = "fichiers_musicaux/Playlist.frm";
	FILE* myFrmFile;
	FILE* myPlayListFile = initAMP(playListName);
	char songFileName[MAX_SIZE_TITLE] = {0};
	myFrmFile = fopen(frmFileName, "r");
	//Si le fichier Playlist.frm existe, on demande a l'utilisateur de le supprimer et on sort du programme
	if (myFrmFile != NULL){
			fclose(myFrmFile);
			printf("Erreur, veuillez-supprimer le fichier %s", frmFileName);
			return 1;
	}
	//Ouverture du fichier frm
	myFrmFile = fopen(frmFileName, "w");
	//Lecture du fichier Playlist.frm ligne par ligne a l'aide de readAMP
	while(!feof(myPlayListFile)){
		FILE * amsFile;
		char amsFileNameWithFolder[MAX_SIZE_TITLE+18] = "fichiers_musicaux/";// Laisser vide si pas de dossier
		char line[MAX_SIZE_LINE] = {0};
		s_song mySong;
		//Utilisation de readAMP pour recupérer le nom des fichier ams associé aux morceaux de musique 
		readAMP(myPlayListFile, songFileName);
		strcat(amsFileNameWithFolder, songFileName);
		amsFile = fopen(amsFileNameWithFolder, "r");
		//Si le fichier mas n'existe pas, recherche d'un fichier texte
		if (amsFile == NULL){
			FILE * txtFile;
			char txtFileName[MAX_SIZE_TITLE];
			//Creation du nom du fichier texte
			strcpy(txtFileName, amsFileNameWithFolder);
			txtFileName[strlen(txtFileName)-1] = 't';
			txtFileName[strlen(txtFileName)-2] = 'x';
			txtFileName[strlen(txtFileName)-3] = 't';
			txtFile = fopen(txtFileName, "r");
			//Si on ne trouve pas de fichier texte, sortie de programme
			if (txtFile == NULL){
				printf("Erreur, mauvais chemin de fichier");
				return 1;
			}
			fclose(txtFile);
			//Utilisation de createAMS pour créer un fichier ams a partir du fichier texte
			createAMS(txtFileName, amsFileNameWithFolder);

		}
		//Utilisation de readAMS pour lire le fichier ams et le representer sous forme de structure de données (voir define.h)
		mySong = readAMS(amsFileNameWithFolder);
		fclose(amsFile);
		// Creation de la trame associée au morceau de musique
		createInitFrame(mySong, line);
		// Ecriture de la trame dans Playlist.frm
		fputs(line, myFrmFile);
		// Lecture des ticks afin d'ecrire leurs trame
		for (int i = 0; i < mySong.nTicks;i++){
			strcpy(line, "");
			// Creation de la tram associée à un tick
			createTickFrame(mySong.tickTab[i], line);
			fputs(line, myFrmFile);
		}

	}
	fclose(myFrmFile);
	closeAMP(myPlayListFile);
	return 0;
}
