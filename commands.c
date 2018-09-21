#include "commands.h"

// List of builtin commands, followed by their corresponding functions.
char *commands[] = {
    "cd",
    "help",
    "exit"
};

int (*builtinFunctions[]) (char **) = {
    &cmdCd,
    &cmdHelp,
    &cmdExit
};

int totalCommands() {
    return sizeof(commands) / sizeof(char *);
}

// Implementation of shell bultin functions

int cmdCd(char** args) {
    if (args[1] == NULL) {
        fprintf(stderr, "Expected argument to \"cd\"\n");
    } else {
        if (chdir(args[1]) != 0) {
            perror("Failed to change directory");
        }
    }

    return 1;
}

int cmdHelp(char** args) {
    printf("SHELL SHELL");
    return 1;
}

int cmdExit(char** args) {
    return 0;
}
