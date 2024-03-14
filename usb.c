//
// Created by ISEN on 13/02/2024.
//

#include "ftd2xx.h"
#include <string.h>

FT_HANDLE initUSB(){
    FT_HANDLE ftHandle;
    FT_STATUS ftStatus;
    ftStatus = FT_Open(0, &ftHandle);
    if(ftStatus != FT_OK){
    	return NULL;
    }
    ftStatus = FT_SetBaudRate(ftHandle, FT_BAUD_9600);
    if(ftStatus != FT_OK){
    	    FT_Close(ftHandle);
	    return NULL;
    }
    ftStatus = FT_SetDataCharacteristics(ftHandle, FT_BITS_8, FT_STOP_BITS_1, FT_PARITY_NONE);
    if (ftStatus != FT_OK){
    	FT_Close(ftHandle);
	return NULL;
    }
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
     FT_Write(ftHandle, frame, strlen(frame), &nbBytesWritten);
     if(nbBytesWritten != strlen(frame)){
     	printf("Erreur lors de l'envoi de la chaine %s\n%d/%d bits envoyés", frame, nbBytesWritten, strlen(frame));
     }

}
