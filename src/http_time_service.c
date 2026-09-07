#include <curl/curl.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "cJSON/cJSON.h"
#include "logger.h"

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

uint64_t get_timestamp_from_response(struct Memory jsonMemory){
    uint64_t return_value = 0;
    cJSON *json = cJSON_Parse(jsonMemory.response);
    if (json == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            printf("Error parsing json: %s; received: %s\n", error_ptr, jsonMemory.response);
        }
        goto end;
    }

    cJSON *unixObject = cJSON_GetObjectItemCaseSensitive(json, "unix");
    if (unixObject == NULL || !cJSON_IsObject(unixObject)){
        log("Got unexpected type at json key 'unix': %s", unixObject);
    }
    cJSON *secondsItem = cJSON_GetObjectItemCaseSensitive(unixObject, "seconds");
    if (secondsItem == NULL || !cJSON_IsNumber(secondsItem)){
        log("Got unexpected type at json key 'unix.seconds': %s", secondsItem);
    }
    return_value = cJSON_GetNumberValue(secondsItem);


    end:
        cJSON_Delete(json);
        return return_value;
}

uint64_t get_current_unix_timestamp(){
    uint64_t return_value = 0;
    
    struct Memory curlData = {
        .response = malloc(1),
        .size = 0
    };

    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();

    if(!curl){
        log("Failed to initialize curl");
        goto end;
    }
    curl_easy_setopt(curl, CURLOPT_URL, "https://unixdate.com/api/now");
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &curlData);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    res = curl_easy_perform(curl);

    if(res != CURLE_OK){
        log("Request to NTP server failed, reason: %s\n", curl_easy_strerror(res));
        goto end;
    }

    return_value = get_timestamp_from_response(curlData);

    end:
        curl_easy_cleanup(curl);
        return return_value;
        free(curlData.response);
}