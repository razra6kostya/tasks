#include "executor.h"
#include "parser.h" // For Node, SimpleCommand, etc.
#include "mem_utils.h" // For safe_free
#include "lexer.h" // For TokenType (if needed for logical operators later)

#include <stdio.h>    // For printf, fprintf, perror
#include <stdlib.h>   // For exit, getenv
#include <unistd.h>   // For fork, execvp, chdir
#include <sys/wait.h> // For waitpid
#include <string.h>   // For strcmp

// --- Static Helper Functions (local to this file) ---

// Executes a simple command (e.g., "ls -l", "echo hello").
// Handles built-in 'cd' command.
// Returns 0 on success (or if cd was handled), -1 on error.
static int execute_simple_command(SimpleCommand *cmd, Redirections *redirs) {
    if (cmd == NULL || cmd->argc == 0 || cmd->argv == NULL || cmd->argv[0] == NULL) {
        return -1; // No command to execute
    }

    // --- Built-in: cd command ---
    if (strcmp(cmd->argv[0], "cd") == 0) {
        char *path = NULL;
        if (cmd->argc == 1) { // cd without arguments: go to HOME
            path = getenv("HOME");
            if (path == NULL) {
                fprintf(stderr, "shell: cd: HOME not set\n");
                return -1;
            }
        } else if (cmd->argc == 2) { // cd with one argument
            path = cmd->argv[1];
        } else { // cd with too many arguments
            fprintf(stderr, "shell: cd: too many arguments\n");
            return -1;
        }

        if (chdir(path) != 0) {
            perror("shell: cd");
            return -1;
        }
        return 0; // cd command handled successfully
    }

    // --- External command execution ---
    pid_t pid = fork();

    if (pid == -1) {
        perror("shell: fork failed");
        return -1;
    } else if (pid == 0) {
        // Child process

        // TODO: Handle redirections in the child process
        // For now, we only execute the command without redirections.
        // This will involve freopen() or dup2().

        execvp(cmd->argv[0], cmd->argv);
        // If execvp returns, it means an error occurred
        perror("shell"); // Prints "shell: command not found" or other execvp error
        exit(EXIT_FAILURE); // Child process exits on failure
    } else {
        // Parent process
        int status;
        if (waitpid(pid, &status, 0) == -1) {
            perror("shell: waitpid failed");
            return -1;
        }
        if (WIFEXITED(status)) {
            return WEXITSTATUS(status); // Return child's exit status
        } else {
            fprintf(stderr, "shell: child process terminated abnormally\n");
            return -1;
        }
    }
}

// Main execution function
int execute_ast(Node *ast) {
    if (ast == NULL) {
        return 0; // Nothing to execute
    }

    int exit_status = 0; // Default success

    switch (ast->type) {
        case NODE_COMMAND:
            exit_status = execute_simple_command(ast->data.command, ast->redirs);
            break;
        case NODE_PIPELINE:
            fprintf(stderr, "shell: Pipeline execution not implemented yet.\n");
            exit_status = -1; // Indicate error
            break;
        case NODE_SUBSHELL:
            fprintf(stderr, "shell: Subshell execution not implemented yet.\n");
            exit_status = -1;
            break;
        case NODE_AND_OR:
            fprintf(stderr, "shell: Logical AND/OR execution not implemented yet.\n");
            exit_status = -1;
            break;
        case NODE_SEQUENCE:
            fprintf(stderr, "shell: Command sequence execution not implemented yet.\n");
            exit_status = -1;
            break;
        default:
            fprintf(stderr, "shell: Unknown AST node type: %d\n", ast->type);
            exit_status = -1;
            break;
    }
    return exit_status;
}
