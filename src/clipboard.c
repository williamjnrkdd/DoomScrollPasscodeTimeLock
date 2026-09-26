#include <stdio.h>

void copy_to_clipboard(const char *text){
    FILE *pipe = popen("clip.exe", "w");

    if (pipe) {
        fputs(text, pipe);
        pclose(pipe);
    }
}