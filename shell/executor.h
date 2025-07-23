#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "parser.h" // Requires Node structure

// Executes the given Abstract Syntax Tree (AST).
// This function will interpret the AST and call appropriate system commands.
// Returns 0 on success, -1 on failure.
int execute_ast(Node *ast);

#endif // EXECUTOR_H
