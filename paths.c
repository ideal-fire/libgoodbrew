#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <goodbrew/config.h>
#include <goodbrew/base.h>
#include <goodbrew/paths.h>
#include <goodbrew/useful.h>

char* gbDataFolder=NULL;

// Passed string should be freed already
signed char generateDefaultDataDirectory(char** _dataDirPointer, signed char _dataDirPreference){
	if (_dataDirPreference==-1){
		generateDefaultDataDirectory(_dataDirPointer,1);
		if (!directoryExists(*_dataDirPointer)){
			free(*_dataDirPointer);
			return generateDefaultDataDirectory(_dataDirPointer,0);
		}
		return 1;
	}
	#if GBPLAT == GB_ANDROID
		*_dataDirPointer = easySprintf("/data/data/%s/",androidPackageName);
	#elif GBPLAT == GB_LINUX
		*_dataDirPointer = strdup("./");
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
		const char* _format;
		if (_dataDirPreference==0){
			_format="ux0:data/%s/";
		}else if (_dataDirPreference==1){
			_format="uma0:data/%s/";
		}else{
			printf("Invalid _dataDirPreference %d\n",_dataDirPreference);
		}
		*_dataDirPointer = easySprintf(_format,vitaAppId);
	#elif GBPLAT == GB_3DS
		*_dataDirPointer = easySprintf("/3ds/data/%s/",vitaAppId);
	#elif GBPLAT == GB_SWITCH
		*_dataDirPointer = easySprintf("/switch/%s/",vitaAppId);
	#endif
	return _dataDirPreference;
}

char* getFixPathString(fileLocationType type){
	if (gbDataFolder==NULL){
		generateDefaultDataDirectory(&gbDataFolder,-1);
	}
	if (type==TYPE_DATA){
		return gbDataFolder;
	}else if (type==TYPE_EMBEDDED){
		#if GBPLAT == GB_ANDROID
			return "";
		#elif GBPLAT == GB_WINDOWS
			return gbDataFolder;
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
				return gbDataFolder;
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

void fixPath(const char* filename,char* _buffer, fileLocationType type){
	strcpy(_buffer,getFixPathString(type));
	strcat(_buffer,filename);
}

char* fixPathAlloc(const char* filename, fileLocationType type){
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