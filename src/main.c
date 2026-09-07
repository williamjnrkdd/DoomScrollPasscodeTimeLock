#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "http_time_service.h"
#include "time_conversion.h"    
#include "compute.h"

    
enum OPERATIONS {
    CREATE = 1,
    RETRIEVE = 2
};


int createPassCode(){

    char name[30];
    
    //2026-07-12T00:00
    char dateTimeString[16];

    // Get name of passcode
    printf("Enter the name of the passcode: \n");
    fflush(stdout);
    fgets(name, sizeof(name), stdin);

    // get timestamp of release date
    printf("Enter the target date time in YYYY-mm-ddTHH:MM format: ");
    fflush(stdout);
    fgets(dateTimeString, sizeof(dateTimeString), stdin);

    // Convert to unix timestamp
    uint64_t release_timestamp = parse_datetime_string_to_unix_timestamp(dateTimeString);

    int code  = compute_passcode(release_timestamp);

    // save release timestamp for code

    return code;
}

// int retrievePassCode(){
//     char name[30];

//     // List choices of passcodes
    
//     // retrieve release_timestamp
//     uint64_t release_timestamp = retrieveReleaseTimestamp(&name);

//     int code  = compute_passcode(release_timestamp);

//     return code;
// }

int main(){
    // display options
    char userChoiceStr[5];
    int userChoice;
    printf("Welcome to the Doom Scroll Passcode Time Lock:\n1. Create passcode \n2. Retrieve passcode\n");
    fflush(stdout);

    userInput:
        fgets(userChoiceStr , sizeof(userChoiceStr), stdin);
        userChoiceStr[strcspn(userChoiceStr, "\n")] = '\0';
        
        char *userChoiceEnd;
        userChoice = (int) strtol(userChoiceStr, &userChoiceEnd, 10);
        if (userChoiceStr == userChoiceEnd){
            goto userInputInvalid;
        }
        
        int code;

        if (userChoice == CREATE){
            code = createPassCode();
        }
        // else if (userChoice == RETRIEVE){
        //     code_int = retrievePassCode();
        // }
        else{
            userInputInvalid:
                printf("Invalid choice. Please try again.\n1. Create passcode \n2. Retrieve passcode\n");
                fflush(stdout);
                goto userInput;
        }

        printf("Passcode: %04d \n", code);
        fflush(stdout);
    

        return 0;
}
