#ifndef GOODBREWPATHSINCLUDED_H
#define GOODBREWPATHSINCLUDED_H

typedef enum{
	TYPE_DATA,
	TYPE_EMBEDDED
}fileLocationType;

char* fixPathAlloc(const char* filename, fileLocationType type);
void fixPath(const char* filename,char* _buffer, fileLocationType type);
void generateDefaultDataDirectory(char** _dataDirPointer);
char* getFixPathString(fileLocationType type);
void makeDataDirectory();
 
#endif /* GOODBREWPATHSINCLUDED_H */