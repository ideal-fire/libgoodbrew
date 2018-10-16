#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "goodbrew/config.h"
#include "base.h"


int main(int argc, char const *argv[]){
	generalGoodInit();

	// Test wait and time functions
	printf("%d\n",getMilli());
	wait(500);
	printf("%d\n",getMilli());

	// Test file existance
	if (checkFileExist("./testbase.c")){
		printf("./testbase.c exist\n");
	}

	// Test directory listing
	printf("Files here:\n");
	crossDir dir;
	crossDirStorage lastStorage;
	dir = openDirectory ("./");
	char* foundFiles[256];
	if (dirOpenWorked(dir)==0){
		printf("Couldn't open");
	}
	int i;
	for (i=0;i<256;i++){
		if (directoryRead(&dir,&lastStorage) == 0){
			break;
		}
		foundFiles[i] = malloc(256);
		memcpy((foundFiles[i]),getDirectoryResultName(&lastStorage),strlen(getDirectoryResultName(&lastStorage))+1);
	}
	directoryClose (dir);
	int j;
	for (j=0;j<i;++j){
		printf("%s\n",foundFiles[j]);
		free(foundFiles[j]);
	}

	// Test file
	crossFile myFile = crossfopen("./testbase.c","rb");
	printf("the first 3 chars in any order: %c %c %c\n",crossgetc(myFile),crossgetc(myFile),crossgetc(myFile));
	printf("3: %d\n",crossftell(myFile));
	crossfseek(myFile,-1,CROSSFILE_CUR);
	printf("2: %d\n",crossftell(myFile));
	crossfseek(myFile,0,CROSSFILE_START);
	printf("0: %d\n",crossftell(myFile));
	crossfseek(myFile,0,CROSSFILE_END);
	printf("filesize: %d\n",crossftell(myFile));
	crossfclose(myFile);

	generalGoodQuit();
	return 0;
}