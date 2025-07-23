#ifndef PARSER_H
#define PARSER_H

#include "lexer.h" 
#include <stdbool.h>

// --- Structures for Parsing ---

// Node type for the Abstract Syntax Tree (AST)
typedef enum {
    NODE_COMMAND,       // Simple command
    NODE_PIPELINE,      // Pipeline of commands
    NODE_SUBSHELL,      // Subshell (commands grouped in parentheses)
    NODE_AND_OR,        // Logical AND (&&) or OR (||)
    NODE_SEQUENCE       // Sequence of commands (;) or background (&)
} NodeType;

// Structure for redirections
typedef struct {
    char *input_file;   // File for input redirection (<)
    char *output_file;  // File for output redirection (>)
    bool append_output; // Append to output file (>>) if true
} Redirections;

// Structure for a simple command (no pipes, logical operators)
typedef struct {
    char **argv;        // Null-terminated array of command arguments
    int argc;           // Number of arguments
} SimpleCommand;

// Structure for AST nodes
typedef struct Node {
    NodeType type;
    union {
        // For NODE_COMMAND
        SimpleCommand *command;

        // For NODE_PIPELINE (A | B | C)
        struct {
            struct Node **commands; // Array of pointers to Node (simple_command or subshell)
            int num_commands;
        } pipeline;

        // For NODE_SUBSHELL ((cmd1; cmd2) or (cmd1 | cmd2))
        struct {
            struct Node *sub_node; // Nested node (typically NODE_SEQUENCE or NODE_PIPELINE)
        } subshell;

        // For NODE_AND_OR (A && B || C)
        struct {
            struct Node *left;
            struct Node *right;
            TokenType op; // TOKEN_LOGICAL_AND or TOKEN_LOGICAL_OR
        } and_or;

        // For NODE_SEQUENCE (A; B; C) or (A & B & C)
        struct {
            struct Node **nodes; // Array of pointers to Node (pipeline or and_or)
            int num_nodes;
            bool is_background; // True if the entire sequence should run in the background (only for the outermost sequence)
        } sequence;

    } data;
    Redirections *redirs; // Redirections can be applied to an entire subshell or pipeline
    bool is_background_command; // Only for commands (simple_command) if they end with & (single command)

} Node;


// --- Function Prototypes ---
Node *parse_input_line();
void free_ast(Node *node);
void free_redirections(Redirections *redirs);
void free_simple_command(SimpleCommand *cmd);
void parser_cleanup();

#endif 
