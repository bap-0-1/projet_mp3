#include "autotests.h"
#include "frame.h"
#include "ams.h"
#include "amp.h"
#include "usb.h"

int main(){
	testReadAMS();
	testCreateAMS();
	testReadAMP();
	/*char frame[1000] = {0};
	s_song mySong;
	mySong = readAMS((char*)"fichiers_musicaux/bohemian_rhapsody.ams");
	createInitFrame(mySong, frame);
	printf("%s", frame);
	//testReadAMS();*/
	return 0;
}
