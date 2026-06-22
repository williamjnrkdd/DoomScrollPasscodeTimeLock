#include <curl/curl.h>

struct Memory {
    char *response;
    size_t size;
};

size_t write_callback(void *data, size_t size, size_t nmemb, void *userp){
    size_t total = size * nmemb;
    struct Memory *mem = (struct Memory *)userp;

    char *ptr = realloc(mem->response,  mem->size + total + 1);
    if (!ptr) {
        return 0;
    }

    mem->response = ptr;

    memcpy(&(mem->response[mem->size]), data, total);
    mem->size += total;

    mem->response[mem->size] = '\0';

    return total;
}

int getCurrentUnixTimestamp(){
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();

    struct Memory curlData = {
        .response = malloc(1),
        .size = 0
    };

    if(curl){
        curl_easy_setopt(curl, CURLOPT_URL, "https://unixdate.com/api/now");
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &curlData);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        res = curl_easy_perform(curl);

        if(res != CURLE_OK){
            fprint("Request to NTP server failed, reason: %s", curl_easy_strerror(res));
            return 0;
        }

    }

    curl_easy_cleanup(curl);
    free(res);

}