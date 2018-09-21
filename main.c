/* Inspired on: https://brennan.io/2015/01/16/write-a-shell-in-c/ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "commands.h"

#define READ_BUFSIZE 1024
#define TOKEN_BUFSIZE 64
#define TOKEN_DELIMITERS "\t "

int launchProcess(char **args)
{
    pid_t pid, wpid;
    int status;

    pid = fork();

    // Error forking
    if (pid < 0) {
        perror("Error forking process");
    }
    // Child process
    if (pid == 0) {
        // Execute user's commands
        if (execvp(args[0], args) == -1) {
            perror("Failed to launch process");
        }
        exit(EXIT_FAILURE); // exit so that the shell can keep running
    }
    // Parent, fork() executed successfully
    else {
        // Parent process
        // child is going to execute the process, so the parent needs to wait for the command to finish running.
        do {
            // wait until either the processes are exited or killed
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}


int execute(char** args)
{
    int i;

    // Empty command
    if (args[0] == NULL) {
        return 1;
    }

    for (i= 0; i < NB_COMMANDS; i++) {
        if (strcmp(args[0], commands[i]) == 0) { // commands exist
            return (*builtinFunctions)(args);
        }
    }

    return launchProcess(args);
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
    printf("~~ ShellShell ~~ \n");
    loop();
    return EXIT_SUCCESS;
}
