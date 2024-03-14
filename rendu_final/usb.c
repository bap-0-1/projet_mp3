//
// Created by ISEN on 13/02/2024.
//

#include "ftd2xx.h"
#include <string.h>

FT_HANDLE initUSB(){
    FT_HANDLE ftHandle;//Handle asssoicé à la connection usb
    FT_STATUS ftStatus;//Handle pour obtenir le status des operations dur le flux usb
    //Ouverture du flux usb
    ftStatus = FT_Open(0, &ftHandle);
    if(ftStatus != FT_OK){
    	return NULL;
    }
    //Mettre la vitesse de transmission de données
    ftStatus = FT_SetBaudRate(ftHandle, FT_BAUD_9600);
    if(ftStatus != FT_OK){
    	    FT_Close(ftHandle);
	    return NULL;
    }
    //Mettre les informations concernant le protocole UART (nombre de bits utilisés, bit de stop, bit de parité)
    ftStatus = FT_SetDataCharacteristics(ftHandle, FT_BITS_8, FT_STOP_BITS_1, FT_PARITY_NONE);
    if (ftStatus != FT_OK){
    	FT_Close(ftHandle);
	return NULL;
    }
    //Ne pas mettre de flow control
    ftStatus = FT_SetFlowControl(ftHandle, FT_FLOW_NONE, 0, 0);
    if (ftStatus != FT_OK){
    	FT_Close(ftHandle);
	return NULL;
    }
    return ftHandle;

}
void closeUSB(FT_HANDLE ftHandle){
	FT_Close(ftHandle);
}


void writeUSB(char* frame, FT_HANDLE ftHandle){
     int nbBytesWritten;
     //Ecriture de données dans le flux usb
     FT_Write(ftHandle, frame, strlen(frame), &nbBytesWritten);
     //Detection d'erreur si les bits n'ont pas tous été envoyés
     if(nbBytesWritten != strlen(frame)){
     	printf("Erreur lors de l'envoi de la chaine %s\n%d/%d bits envoyés", frame, nbBytesWritten, strlen(frame));
     }

}
