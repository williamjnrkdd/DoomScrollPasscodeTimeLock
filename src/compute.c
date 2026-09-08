#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "logger.h"

#define PRIME_NUMBER 2071030823

uint64_t get_radom_integer(){
    uint64_t random_int;

    FILE *fp = fopen("/dev/urandom", "rb");
    if(!fp){
        printf("Unable to open /dev/urandom\n");
        return -1;
    }

    fread(&random_int, sizeof(random_int), 1, fp);

    return random_int;
}

uint64_t modular_exponentiation(uint64_t base, uint64_t exponent, uint64_t modulus){
    if (exponent == 0){
        return 1;
    }
    base %= modulus;
    if (exponent % 2 == 0 ){
        uint64_t half = modular_exponentiation(base, exponent/2, modulus);
        return (half * half) % modulus;
    }
    uint64_t half = modular_exponentiation(base, (exponent-1)/2, modulus);
    return (base * half * half) % modulus;
}

// base = 2
// power = 3
// cpower = 1

// 1. 2 * e(2, 3, 2) = 2*4 = 8
// 2. e(2, 3, 2) -> 2 * e(2, 3, 3) = 2* 2 = 4
// 3. e(2, 3, 3) -> 2 * e(2, 3, 4) = 2* 1 = 2
// 4. e(2, 3, 4) -> 1 

int compute_passcode(uint64_t release_timestamp){
    int count = 0;
    int passcode;
    // uint64_t base = get_radom_integer();
    uint64_t base = 17707010320199972569;
    uint64_t result;
    time_t start_time  = clock() , end_time = clock();
    // while(((double) (end_time - start_time) / CLOCKS_PER_SEC) < (5 * 60)){
    while (count < 1386634965){
        // log("time: %f < %d \n", ((double) (end_time - start_time) / CLOCKS_PER_SEC), (5 * 60));
        result = modular_exponentiation(base, release_timestamp, PRIME_NUMBER);
        // log("modular exponentiation: %lu ^ %lu %% %d = %lu\n", base, release_timestamp, PRIME_NUMBER, result);
        base = result;
        // printf("base = %lu \n", base);
        count += 1;
        end_time = clock();
    }

    printf("The number of cycles to give 5 minutes is: %d\n", count);
    printf("The random base integer used is: %lu\n", base);
    printf("The time is: %f\n", (double) (end_time - start_time) / CLOCKS_PER_SEC);
    printf("The result is: %lu\n", result);

    // take the last 4 digits
    passcode = result % 10000;

    return passcode;
}

// 1234567
// 1234567 / 10000 = 123.4567
//  123.4567 - 123 = 0.4567
// 0.4567 * 10000 = 4567

