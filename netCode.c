#include <stdlib.h>
#include <string.h>
#include <goodbrew/config.h>
#include <goodbrew/netCode.h>

#if GBNET == GBNET_CURL
	typedef struct{
		char* memory;
		size_t size;
	}MemoryStruct;
	CURL* curl_handle;
	char curlFollowRedirects=0;

	size_t curlWriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp){
		size_t realsize = size * nmemb;
		MemoryStruct* mem = (MemoryStruct*) userp;

		char* _newBuffer = realloc(mem->memory, mem->size + realsize + 1);
		if(_newBuffer == NULL) {
			free(mem->memory);
			printf("not enough memory (realloc returned NULL)\n");
			return 0;
		}
		mem->memory = _newBuffer;
	
		memcpy(&(mem->memory[mem->size]), contents, realsize);
		mem->size += realsize;
		mem->memory[mem->size] = 0;
	
		return realsize;
	}
	size_t curlWriteDataFile(void *ptr, size_t size, size_t nmemb, void *stream){
		size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
		return written;
	}
#endif

void gbNetDeinit(){
	#if GBNET == GBNET_CURL
		curl_easy_cleanup(curl_handle);
	#endif
}

void gbNetDownloadToFile(const char* _passedUrl, const char* _passedFilename){
	#if GBNET == GBNET_CURL
		FILE* fp;
		curl_easy_setopt(curl_handle, CURLOPT_URL, _passedUrl);
		fp = fopen(_passedFilename, "wb");
		if(fp) {
			curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, curlWriteDataFile);
			curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, fp);
			curl_easy_perform(curl_handle);
			fclose(fp);
		}
	#endif
}
void gbNetDownloadToMem(const char* _passedUrl, char** _toStoreWebpageData, size_t* _toStoreSize){
	#if GBNET == GBNET_CURL
		CURLcode res;
		MemoryStruct chunkToDownloadTo;
		chunkToDownloadTo.memory = malloc(1);
		chunkToDownloadTo.size = 0;
		curl_easy_setopt(curl_handle, CURLOPT_URL, _passedUrl);
		curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, curlWriteMemoryCallback);
		curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void*)&chunkToDownloadTo);
		curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, curlFollowRedirects);
		res = curl_easy_perform(curl_handle);
		if(res != CURLE_OK) {
			printf("Failed, the world is over. %d\n",res);
		}
		*_toStoreWebpageData = chunkToDownloadTo.memory;
		if (_toStoreSize!=NULL){
			*_toStoreSize = chunkToDownloadTo.size;
		}
	#else
		*_toStoreWebpageData=NULL;
		*_toStoreSize=0;
	#endif
}
void gbNetDisableSSL(){
	#if GBNET == GBNET_CURL
		curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYPEER, 0);
	#endif
}
void gbNetSetReferer(const char* _newReferer){
	#if GBNET == GBNET_CURL
		curl_easy_setopt(curl_handle, CURLOPT_REFERER, _newReferer);
	#endif
}
void gbNetSetAgent(const char* _agentName){
	#if GBNET == GBNET_CURL
		curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, _agentName);
	#endif
}
void gbNetEnableDebug(){
	#if GBNET == GBNET_CURL
		curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1L);
	#endif
}
void gbNetFollowRedirects(char _isEnabled){
	#if GBNET == GBNET_CURL
		curlFollowRedirects = _isEnabled;
	#endif
}
char* gbNetGetLastRedirect(){
	#if GBNET == GBNET_CURL
		if (curlFollowRedirects==0){
			char* _returnLocation;
			CURLcode res;
			res = curl_easy_getinfo(curl_handle, CURLINFO_REDIRECT_URL, &_returnLocation);
			if((res == CURLE_OK) && _returnLocation) {
				return _returnLocation;
			}else{
				printf("Erorr in get redirect url");
				return NULL;
			}
		}else{
			printf("Can't work, you're already following redirects");
			return NULL;
		}
	#else
		return NULL;
	#endif
}
char gbNetInit(const char* _certLocation){
	#if GBNET == GBNET_CURL
		if (curl_global_init(CURL_GLOBAL_ALL)!=0){
			return 1;
		}
		curl_handle = curl_easy_init();
		if (curl_handle==NULL){
			return 1;
		}
		//curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1L);
		//curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1L);
		
		// Needed for https
		if (_certLocation!=NULL){
			curl_easy_setopt(curl_handle, CURLOPT_CAINFO, _certLocation);
		}else{
			curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYPEER, 0);
		}
		curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
	#else
		return 1;
	#endif
}
