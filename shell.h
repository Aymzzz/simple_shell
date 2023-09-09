#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// Function prototypes
char *read_line(void);
void execute_command(char *input);

#endif /* SHELL_H */
