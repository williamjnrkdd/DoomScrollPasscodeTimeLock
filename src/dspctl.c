#include <stdio.h>
#include <stdint.h>
#include <string.h>

extern uint64_t generate_large_prime(uint64_t low, uint64_t high, int iterations);

extern uint64_t retrieveReleaseTimestamp(name);

extern int computePassCode(uint64_t release_timestamp);

extern uint64_t getNTPServerUnixTimestamp();

int main(){
    // display options
        // create passcode
        // retrieve passcode
    
    // Get name of passcode

    // if (createPassCode)
        //code = createPassCode(name)
    // else
        // code = retrievePassCode(name)

    //return code;
}

int createPassCode(char name[]){
    // get timestamp of release date
    
    // Convert to unix timestamp
    release_timestamp = 

    int code  = computePassCode(release_timestamp);

    return code;
}

void retrievePassCode(char name[]){
    
    // retrieve release_timestamp
    uint64_t release_timestamp = retrieveReleaseTimestamp(name);

    int code  = computePassCode(release_timestamp);

    return code;
}