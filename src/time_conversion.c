#include <time.h>
#include <stdint.h>
#include <stdio.h>

uint64_t parse_datetime_string_to_unix_timestamp(char *datetime_string){
    struct tm time_info= {0};
    int num_of_parts = sscanf(datetime_string, "%d-%d-%dT%d:%d", &time_info.tm_year, &time_info.tm_mon, &time_info.tm_mday, \
    &time_info.tm_hour, &time_info.tm_min);

    getchar(); //get trainling \n
    
    if (num_of_parts != 5){
        return 0;
    }

    time_info.tm_year -= 1900;
    time_info.tm_mon -= 1;
    time_info.tm_sec = 0;
    
    time_t datetime = mktime(&time_info);
    return (uint64_t) datetime;
}