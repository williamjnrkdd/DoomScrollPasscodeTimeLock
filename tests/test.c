#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>


uint64_t getCurrentUnixTimestamp();

int main(){
    uint64_t timestamp = getCurrentUnixTimestamp();
    printf("The current unix time in seconds is: %" PRIu64 "\n", timestamp);
}