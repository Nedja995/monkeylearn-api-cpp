#include <stdio.h>
#include <curl/curl.h>

static char* responseBuffer;
static int writer(char *data, size_t size, size_t nmemb, char* buffer_in)
{
	unsigned int memSize = size * nmemb;
	responseBuffer = (char*)malloc(memSize);
	strcpy(responseBuffer, data);
	return memSize;
}
int main(void)
{
	const char* text = "{\"text_list\": [\"This is a text to test the API.\"]}";
	CURL *curl;
	CURLcode res;
	struct curl_slist *headers = NULL;

	/* In windows, this will init the winsock stuff */
	res = curl_global_init(CURL_GLOBAL_DEFAULT);
	if (res != CURLE_OK) {
		fprintf(stderr, "curl_global_init() failed: %s\n",
			curl_easy_strerror(res));
		return 1;
	}
	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, "https://api.monkeylearn.com/v2/classifiers/cl_KFXhoTdt/classify/?sandbox=1");
		curl_easy_setopt(curl, CURLOPT_POST, 1L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, text);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
		headers = curl_slist_append(headers, "Expect:");
		headers = curl_slist_append(headers, "Content-Type: application/json");
		headers = curl_slist_append(headers, "Authorization:Token fec5b02f8ca3037b3644f4092c2a0835b83b493e");
		headers = curl_slist_append(headers, "Transfer-Encoding: chunked");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
//		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
		res = curl_easy_perform(curl);
		if (res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
				curl_easy_strerror(res));
		//cleanup always
		curl_easy_cleanup(curl);
	}
	curl_global_cleanup();
	return 0;
}
