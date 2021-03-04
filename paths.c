#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <goodbrew/config.h>
#include <goodbrew/base.h>
#include <goodbrew/paths.h>
#include <goodbrew/useful.h>

#if GBPLAT == GB_WINDOWS
	#include <windows.h> // for getting the data directory
#endif
#if GBPLAT == GB_LINUX
#include <errno.h>
#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>
#endif

char* gbDataFolder=NULL;
static char* gbAppdataFolder=NULL;
signed char gbDataFolderUsing=-2;

#if GBPLAT == GB_LINUX
// may or may not end in a slash
static char* gnulinuxgethome(){
	char* _homevar=getenv("HOME");
	if (!_homevar){
		errno=0;
		struct passwd* pw = getpwuid(getuid());
		if (pw){
			_homevar=pw->pw_dir;
		}else{
			perror(NULL);
			return NULL;
		}
	}
	return strdup(_homevar);
}
// xdg type list.
static char* gnulinuxtrydirlist(const char* _list, const char* _subdirname){
	int _nextcheckpos=0;
	while(1){
		int _thislen;
		const char* _nextcolon;
		if ((_nextcolon=strchr(_list+_nextcheckpos,':'))){
			_thislen=_nextcolon-(_list+_nextcheckpos);
		}else{
			_thislen=strlen(_list+_nextcheckpos);
		}
		char* _potential=malloc(_thislen+strlen(_subdirname)+3);
		memcpy(_potential,_list+_nextcheckpos,_thislen);
		strcpy(_potential+_thislen,"/");
		strcat(_potential,_subdirname);
		strcat(_potential,"/");
		if (directoryExists(_potential)){
			return _potential;
		}
		free(_potential);
		if (_nextcolon){
			_nextcheckpos+=_thislen+1;
		}else{
			return NULL;
		}
	}
}
#endif
// Passed string should be freed already
signed char generateDefaultDataDirectory(char** _dataDirPointer, signed char _dataDirPreference){
	#if GBPLAT == GB_ANDROID
		*_dataDirPointer = easySprintf("/data/data/%s/",androidPackageName);
	#elif GBPLAT == GB_LINUX
		char* _dirpref = getenv("XDG_DATA_HOME");
		if (!_dirpref){
			char* _homedir=gnulinuxgethome();
			if (_homedir){
				char* _localdir = easyCombineStrings(4,_homedir,"/.local/share/",vitaAppId,"/");
				free(_homedir);
				if (directoryExists(_localdir)){
					*_dataDirPointer=_localdir;
					return 0;
				}else{
					free(_localdir);
				}
			}
		}else{
			char* _localdir = easyCombineStrings(4,_dirpref,"/",vitaAppId,"/");
			if (directoryExists(_localdir)){
				*_dataDirPointer=_localdir;
				return 0;
			}
			free(_localdir);
		}
		*_dataDirPointer = easyCombineStrings(3,"./",vitaAppId,"/");
		fprintf(stderr,"falling back on %s for data directory\n",*_dataDirPointer);
		return 0;
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
		if (_dataDirPreference==-1){
			generateDefaultDataDirectory(_dataDirPointer,1);
			if (!directoryExists(*_dataDirPointer)){
				free(*_dataDirPointer);
				return generateDefaultDataDirectory(_dataDirPointer,0);
			}
			return 1;
		}
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
	return _dataDirPreference==-1 ? 0 : _dataDirPreference;
}
static void generateDefaultEmbeddedDirectory(){
	if (gbDataFolder==NULL){ // needed for windows
		generateDefaultDataDirectory(&gbDataFolder, -1);
	}
	#if GBPLAT == GB_ANDROID
		gbAppdataFolder="";
	#elif GBPLAT == GB_WINDOWS
		gbAppdataFolder=gbDataFolder;
	#elif GBPLAT == GB_LINUX
		char* _dirList = getenv("XDG_DATA_DIRS");
		if (!_dirList){
			_dirList="/usr/local/share/:/usr/share/";
		}
		gbAppdataFolder=gnulinuxtrydirlist(_dirList, vitaAppId);
		if (gbAppdataFolder){
			return;
		}
		fprintf(stderr,"falling back on ./ for embedded data directory\n");
		gbAppdataFolder = strdup("./");
	#elif GBPLAT == GB_VITA
		gbAppdataFolder="app0:";
	#elif GBPLAT == GB_3DS
		if (cacheIsCiaBuild==-1){
			cacheIsCiaBuild = getIsCiaBuild();
		}
		gbAppdataFolder = cacheIsCiaBuild ? "romfs:/" : gbDataFolder;
	#elif GBPLAT == GB_SWITCH
		gbAppdataFolder="romfs:/";
	#else
		printf("Not yet implemented to getFixPathString.\n");
		gbAppdataFolder="";
	#endif
}
char* getFixPathString(fileLocationType type){
	if (gbDataFolder==NULL){
		generateDefaultDataDirectory(&gbDataFolder, -1);
	}
	if (gbAppdataFolder==NULL){
		generateDefaultEmbeddedDirectory();
	}
	return type==TYPE_DATA ? gbDataFolder : gbAppdataFolder;
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
