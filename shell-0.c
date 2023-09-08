#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
 * Struct to represent a command and arguments
*/ 
struct command {
  char *name;
  char **args;
};

/**
 * Print prompt
*/
void print_prompt() 
{
  printf("($) ");
}

/**
 * Read input line
*/
char *read_input()
{
  char *line = NULL;
  size_t bufsize = 0;

  if (getline(&line, &bufsize, stdin) == -1) {
    free(line);
    exit(EXIT_FAILURE);
  }

  return line;
}

/**
 * Tokenize input line into commands
*/  
struct command *tokenize(char *line)
{
  struct command *cmd = malloc(sizeof(struct command));
  char *token;

  cmd->name = strtok(line, " \t\n");
  cmd->args = malloc(sizeof(char *) * 2);
  
  int i = 0;
  while ((token = strtok(NULL, " \t\n")) != NULL) {
    cmd->args[i++] = token;
  }
  cmd->args[i] = NULL;

  return cmd;
}

/**
 * Execute command
*/
void execute(struct command *cmd)
{
  pid_t pid = fork();

  if (pid == 0) {
    execve(cmd->name, cmd->args, NULL);
    perror("Error executing command");
    exit(1);
  } else {
    wait(NULL);
  }
}

/**
 * Main shell loop
*/
int main(void)
{
  while (1) {
    print_prompt();

    char *line = read_input();
    struct command *cmd = tokenize(line);

    if (strcmp(cmd->name, "exit") == 0) {
      break;
    }

    execute(cmd);
    free(cmd->name);
    free(cmd->args);
    free(cmd);
    free(line);
  }

  return 0;
}
