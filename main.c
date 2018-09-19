/* Inspired on: https://brennan.io/2015/01/16/write-a-shell-in-c/ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define READ_BUFSIZE 1024

#define TOKEN_BUFSIZE 64
#define TOKEN_DELIMITERS "\t "

int execute(char** args)
{
    return 0;
}

char** splitLine(char* line)
{
    int bufsize = TOKEN_BUFSIZE, pos = 0;
    char **tokens = malloc(sizeof(char*) * bufsize);
    char *token;

    if (!tokens) {
        fprintf(stderr, "Tokens allocation error\n");
        exit(EXIT_FAILURE);
    }

    // Split line into tokens
    token = strtok(line, TOKEN_DELIMITERS);
    while (token != NULL) {
        printf("%s \n", token);
        tokens[pos] = token;
        pos++;

        // Reallocate memory
        if (pos > bufsize) {
            bufsize += TOKEN_BUFSIZE;
            tokens = realloc(tokens, sizeof(char*) * bufsize);

            if (!tokens) {
                fprintf(stderr, "Tokens reallocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        /* Next token
        Note: The first call to strtok must pass the C string to tokenize,
        and subsequent calls must specify NULL as the first argument,
        which tells the function to continue tokenizing the string you passed in first. */
        token = strtok(NULL, TOKEN_DELIMITERS);
    }

    return tokens;
}

// Read user input
// Note: getline() do the same but for tuto, I write the whole stuff
char* readLine()
{
    // Allocate memory
    int bufsize = READ_BUFSIZE, pos = 0, c;
    char *buffer = malloc(sizeof(char) * bufsize); // sizeof(char) = 1

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
    char *line;
    char **args;
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
