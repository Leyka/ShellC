/* Inspired on: https://brennan.io/2015/01/16/write-a-shell-in-c/ */

#include <stdio.h>
#include <stdlib.h>

#define READ_BUFSIZE 1024

int execute(char** args) 
{
    return 0;
}

char** splitLine(char* line) 
{

}

// Read user input
// Note: getline() do the same but for tuto, I write the whole stuff
char* readLine() 
{
    // Allocate memory
    int bufsize = READ_BUFSIZE;
    int pos = 0;
    char* buffer = malloc(sizeof(char) * bufsize); // sizeof(char) = 1
    int c;

    if (!buffer) {
        fprintf(stderr, "Allocation memory error \n");
        exit(EXIT_FAILURE);
    }

    while(1) {
        // Read input (one char at a time)
        c = getchar();

        // If we hit EOF, replace it with a null character and return.
        if (c == EOF || c == '\n') {
            buffer[pos] = '\0';
            return buffer;
        } else {
            buffer[pos] = c;
        }

        pos++;

        // Check if we have exceeded the buffer, reallocate.
        if (pos > bufsize) {
            bufsize += READ_BUFSIZE;
            buffer = realloc(buffer, bufsize);
            if (!buffer) {
                fprintf(stderr, "Reallocation memory error \n");
                exit(EXIT_FAILURE);
            }
        }

    }

}

// Read, parse, execute
void loop() 
{
    char* line;
    char** args;
    int status;

    do {
        printf("> ");
        line = readLine();
        args = splitLine(line);
        status = execute(args);

        free(line);
        free(args);
    } while (status);
}

int main(int argc, char **argv) 
{
    loop();
    return EXIT_SUCCESS;
}