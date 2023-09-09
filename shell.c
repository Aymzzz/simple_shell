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
/*
void execute_command(char *input)
{
    pid_t child_pid;
    int status;
    char *token;
    char *args[1024]; // An array to hold command and arguments
    int i = 0;

    input[strlen(input) - 1] = '\0'; // Remove newline character

    // Tokenize the input
    token = strtok(input, " ");
    while (token != NULL)
    {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL; // NULL-terminate the argument array

    child_pid = fork();

    if (child_pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0)
    {
        // Child process
        if (execve("/usr/bin/ls", args, NULL) == -1) //modified this to suit paths as the linux directory
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
}*/
void execute_command(char *input)
{
    pid_t child_pid;
    int status;
    char *token;
    char *args[1024]; // An array to hold command and arguments
    int i = 0;

    input[strlen(input) - 1] = '\0'; // Remove newline character

    // Tokenize the input
    token = strtok(input, " ");
    while (token != NULL)
    {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL; // NULL-terminate the argument array

    // List of directories to search for commands
    char *directories[] = {"/bin", "/usr/bin", NULL};

    // Try to find the command in the specified directories
    char full_path[1024];
    int found = 0;
    for (int j = 0; directories[j] != NULL; j++)
    {
        snprintf(full_path, sizeof(full_path), "%s/%s", directories[j], args[0]);
        if (access(full_path, X_OK) == 0)
        {
            found = 1;
            break;
        }
    }

    if (found)
    {
        child_pid = fork();

        if (child_pid == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (child_pid == 0)
        {
            // Child process
            if (execve(full_path, args, NULL) == -1)
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
    else
    {
        printf("Command not found: %s\n", args[0]);
    }
}
