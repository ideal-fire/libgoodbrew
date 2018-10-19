#include <stdio.h>
#include <goodbrew/config.h>
#include <goodbrew/base.h>
#include <goodbrew/paths.h>

char* dataFolder=NULL;

// Passed string should be freed already
void generateDefaultDataDirectory(char** _dataDirPointer){
	#if GBPLAT == GB_ANDROID
		*_dataDirPointer = malloc(strlen("/data/data//")+strlen(ANDROIDPACKAGENAME)+1);
		strcpy(*_dataDirPointer,"/data/data/");
		strcat(*_dataDirPointer,ANDROIDPACKAGENAME);
		strcat(*_dataDirPointer,"/");
	#elif GBPLAT == GB_LINUX
		*_dataDirPointer = malloc(strlen("./")+1);
		strcpy(*_dataDirPointer,"./");
	#elif GBPLAT == GB_WINDOWS
		char _buffer[1000];
		if (GetModuleFileName(NULL,_buffer,sizeof(_buffer))==0){
			printf("Failed to get exe location.");
			strcpy(_buffer,"./");
		}else{
			int i;
			for (i=strlen(_buffer)-2;i>=0;--i){
				if (_buffer[i]=='\\'){
					_buffer[i+1]='\0';
					break;
				}
			}
		}
		*_dataDirPointer = strdup(_buffer);
	#elif GBPLAT == GB_VITA
		// Use uma0 data directory if it exists, otherwise fallback on ux0
		*_dataDirPointer = malloc(strlen("uma0:data//")+strlen(VITAAPPID)+1);
		strcpy(*_dataDirPointer,"uma0:data/");
		strcat(*_dataDirPointer,VITAAPPID);
		strcat(*_dataDirPointer,"/");
		if (!directoryExists(*_dataDirPointer)){
			free(*_dataDirPointer);
			*_dataDirPointer = malloc(strlen("ux0:data//")+strlen(VITAAPPID)+1);
			strcpy(*_dataDirPointer,"ux0:data/");
			strcat(*_dataDirPointer,VITAAPPID);
			strcat(*_dataDirPointer,"/");
		}
	#elif GBPLAT == GB_3DS
		*_dataDirPointer = malloc(strlen("/3ds/data/")+strlen(VITAAPPID)+2);
		strcpy(*_dataDirPointer,"/3ds/data/");
		strcat(*_dataDirPointer,VITAAPPID);
		strcat(*_dataDirPointer,"/");
	#elif GBPLAT == GB_SWITCH
		*_dataDirPointer = malloc(strlen("/switch/")+strlen(VITAAPPID)+2);
		strcpy(*_dataDirPointer,"/switch/");
		strcat(*_dataDirPointer,VITAAPPID);
		strcat(*_dataDirPointer,"/");
	#endif
}

char* getFixPathString(fileLocationType type){
	if (dataFolder==NULL){
		generateDefaultDataDirectory(&dataFolder);
	}
	if (type==TYPE_DATA){
		return dataFolder;
	}else if (type==TYPE_EMBEDDED){
		#if GBPLAT == GB_ANDROID
			return "";
		#elif GBPLAT == GB_WINDOWS
			return dataFolder;
		#elif GBPLAT == GB_LINUX
			return "./";
		#elif GBPLAT == GB_VITA
			return "app0:";
		#elif GBPLAT == GB_3DS
			if (cacheIsCiaBuild==-1){
				cacheIsCiaBuild = getIsCiaBuild();
			}
			if (cacheIsCiaBuild){
				return "romfs:/";
			}else{
				return dataFolder;
			}
		#elif GBPLAT == GB_SWITCH
			return "romfs:/";
		#else
			printf("Not yet implemented to getFixPathString.\n");
			return "";
		#endif
	}else{
		printf("Unknown type.\n");
		return "";
	}
}

void fixPath(char* filename,char _buffer[], fileLocationType type){
	strcpy((char*)_buffer,getFixPathString(type));
	strcat((char*)_buffer,filename);
}

char* fixPathAlloc(char* filename, fileLocationType type){
	char* _foundPrefix = getFixPathString(type);
	char* _returnBuff = malloc(strlen(filename)+strlen(_foundPrefix)+1);
	strcpy(_returnBuff,_foundPrefix);
	strcat(_returnBuff,filename);
	return _returnBuff;
}

void makeDataDirectory(){
	char* _foundDataDir = getFixPathString(TYPE_DATA);
	if (directoryExists(_foundDataDir)==0){
		createDirectory(_foundDataDir);
	}
}