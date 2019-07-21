#ifndef GOODBREWNETINCLUDED_H
#define GOODBREWNETINCLUDED_H
#ifdef __cplusplus
extern "C" {
#endif
#if GBNET == GBNET_CURL
	#include <curl/curl.h>
#endif

void gbNetDeinit();
void gbNetDownloadToFile(const char *_passedUrl,const char *_passedFilename);
void gbNetDownloadToMem(const char *_passedUrl,char **_toStoreWebpageData,size_t *_toStoreSize);
void gbNetDisableSSL();
void gbNetSetReferer(const char *_newReferer);
void gbNetSetAgent(const char *_agentName);
void gbNetEnableDebug();
void gbNetFollowRedirects(char _isEnabled);
char *gbNetGetLastRedirect();
char gbNetInit(const char *_certLocation);

#ifdef __cplusplus
}
#endif 
#endif
