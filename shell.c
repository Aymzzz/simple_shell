#include "shell.h"

int main(void)
{
    char *input;

    while (1)
    {
        // Display prompt
        write(STDOUT_FILENO, "#cisfun$ ", 9);

        // Read user input
        input = read_line();

        // Execute the command
        execute_command(input);

        // Free allocated memory
        free(input);
    }

    return (0);
}

//reading line

char *read_line(void)
{
    char *line = NULL;
    size_t bufsize = 0;

    if (getline(&line, &bufsize, stdin) == -1)
    {
        if (feof(stdin))
        {
            write(STDOUT_FILENO, "\n", 1); // Handle Ctrl+D (EOF)
            exit(EXIT_SUCCESS);
        }
        else
        {
            perror("getline"); // Handle other errors
            exit(EXIT_FAILURE);
        }
    }

    return (line);
}

//function execute_command

void execute_command(char *input)
{
    pid_t child_pid;
    int status;
    char *args[] = {input, NULL}; // Command and NULL-terminated argument array
    char *envp[] = {NULL}; // NULL-terminated environment variable array

    input[strlen(input) - 1] = '\0'; // Remove newline character

    child_pid = fork();

    if (child_pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0)
    {
        // Child process
        if (execve(input, args, envp) == -1)
        {
            perror("execve");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        // Parent process
        wait(&status);
    }
}

