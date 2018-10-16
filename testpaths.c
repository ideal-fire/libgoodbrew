#include <stdio.h>
#include <stdlib.h>
#include "goodbrew/config.h"
#include "base.h"
#include "paths.h"

int main(int argc, char const *argv[]){
	generalGoodInit();

	char* _dataFile = fixPathAlloc("somedatafile",TYPE_DATA);
	char* _embeddedFile = fixPathAlloc("someembeddedfile",TYPE_EMBEDDED);
	printf("%s\n%s\n",_dataFile,_embeddedFile);
	free(_dataFile);
	free(_embeddedFile);

	generalGoodQuit();
	return 0;
}