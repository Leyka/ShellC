#include <stdio.h>
#include <unistd.h>

#define NB_COMMANDS 3

char *commands[NB_COMMANDS];
int (*builtinFunctions[NB_COMMANDS]) (char **);

int cmdCd(char** args);
int cmdHelp(char** args);
int cmdExit(char** args);
int totalCommands(void);
