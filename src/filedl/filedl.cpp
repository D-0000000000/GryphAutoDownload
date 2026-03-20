#include "filedl.hpp"
#include "common/common_typedefs.hpp"

#include <curl/curl.h>

#include <cstdint>
#include <cstring>
#include <memory>
#include <string>

int filedl_init()
{
	CURLcode result = curl_global_init(CURL_GLOBAL_ALL);
	return (int)result;
}

int filedl_deinit()
{
	curl_global_cleanup();
	return 0;
}

size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp)
{
	size_t realsize = size * nmemb;
	MemChunk *mem = (MemChunk *)userp;

	char *ptr = (char *)realloc(mem->data, mem->size + realsize + 1);
	if (!ptr)
	{
		/* out of memory! */
		printf("not enough memory (realloc returned NULL)\n");
		return 0;
	}

	mem->data = ptr;
	memcpy(&(mem->data[mem->size]), contents, realsize);
	mem->size += realsize;
	mem->data[mem->size] = 0;
	return realsize;
}

int filedl_download(std::string url, MemChunk &chunk)
{
	CURL *curl;
	CURLcode result = (CURLcode)-1;

	curl = curl_easy_init();
	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_CA_CACHE_TIMEOUT, 86400L);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

		result = curl_easy_perform(curl);

		if (result != CURLE_OK)
		{
			curl_easy_cleanup(curl);
			return (int)result;
		}

		curl_easy_cleanup(curl);
	}
	return (int)result;
}
