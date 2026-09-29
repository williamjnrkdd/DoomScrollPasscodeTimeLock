#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "http_time_service.h"
#include "time_conversion.h"    
#include "compute.h"
#include "clipboard.h"
#include "qrcode_image_generator.h"

    
enum OPERATIONS {
    CREATE = 1,
    RETRIEVE = 2,

    DISPLAY = 1,
    CLIPBOARD = 2,
    QRCODE = 3
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
    char *userChoiceEnd;
    printf("Welcome to the Doom Scroll Passcode Time Lock:\n1. Create passcode \n2. Retrieve passcode\n");
    fflush(stdout);

    user_main_action_input:
        fgets(userChoiceStr , sizeof(userChoiceStr), stdin);
        userChoiceStr[strcspn(userChoiceStr, "\n")] = '\0';
        
        userChoice = (int) strtol(userChoiceStr, &userChoiceEnd, 10);
        if (userChoiceStr == userChoiceEnd){
            goto user_main_action_input_invalid;
        }
        
        int code;

        if (userChoice == CREATE){
            //code = createPassCode();
            code = 1234;
        }
        // else if (userChoice == RETRIEVE){
        //     code_int = retrievePassCode();
        // }
        else{
            user_main_action_input_invalid:
                printf("Invalid choice. Please try again.\n1. Create passcode \n2. Retrieve passcode\n");
                fflush(stdout);
                goto user_main_action_input;
        }

        char codeText[5];
    
        snprintf(codeText, sizeof(codeText), "%04d", code);

        printf("Choose your output format for code:\n1. Display passcode \n2. Copy passcode to clipboard \n3. Generate QR code\n");
        fflush(stdout);

        user_output_input:
            fgets(userChoiceStr , sizeof(userChoiceStr), stdin);
            userChoiceStr[strcspn(userChoiceStr, "\n")] = '\0';
            
            userChoice = (int) strtol(userChoiceStr, &userChoiceEnd, 10);
            if (userChoiceStr == userChoiceEnd){
                goto user_output_input_invalid;
            }
            

            if (userChoice == DISPLAY){
                printf("Passcode: %s \n", codeText);
            }
            else if (userChoice == CLIPBOARD){
                copy_to_clipboard(codeText);
            }
            else if (userChoice == QRCODE){
                printf("Enter the path for the png file: \n");
                fflush(stdout);
                
                char path[100];
                fgets(path , sizeof(path), stdin);
                path[strcspn(path, "\n")] = '\0';
                code_to_qrcode_png(codeText, path);
            }
            else{
                user_output_input_invalid:
                    printf("Invalid choice. Please try again.\n1. Create passcode \n2. Retrieve passcode\n");
                    fflush(stdout);
                    goto user_output_input;
            }


        fflush(stdout);
    

        return 0;
}
