#ifndef GOODBREWPATHSINCLUDED_H
#define GOODBREWPATHSINCLUDED_H
#ifdef __cplusplus
extern "C" {
#endif


typedef enum{
	TYPE_DATA,
	TYPE_EMBEDDED
}fileLocationType;

extern char* gbDataFolder;
// -2 by default. Its values correspond to those passed to generateDefaultDataDirectory
extern signed char gbDataFolderUsing;

char* fixPathAlloc(const char* filename, fileLocationType type);
void fixPath(const char* filename,char* _buffer, fileLocationType type);
// to set the internal one used by fixPath, use gbDataFolder.
// _dataDirPreference:
// -1 - Use alternate if exists, otherwise primary
// 0 - Must use primary
// 1 - Must use alternate
// returns _dataDirPreference value used, useful if you pass -1
signed char generateDefaultDataDirectory(char** _dataDirPointer, signed char _dataDirPreference);
char* getFixPathString(fileLocationType type);
void makeDataDirectory();

#ifdef __cplusplus
}
#endif
#endif /* GOODBREWPATHSINCLUDED_H */
