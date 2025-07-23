#include "lexer.h"
#include "parser.h"
#include "utils.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> // For exit()

// --- Helper function for printing AST (for debugging) ---
static void print_ast_indent(Node *node, int indent) {
    if (node == NULL) return;
    for (int i = 0; i < indent; i++) printf("  ");

    switch (node->type) {
        case NODE_COMMAND:
            printf("COMMAND [");
            if (node->data.command->argv) {
                for (int i = 0; node->data.command->argv[i] != NULL; i++) {
                    printf("'%s'%s", node->data.command->argv[i], node->data.command->argv[i+1] != NULL ? ", " : "");
                }
            }
            printf("]");
            if (node->is_background_command) printf(" & (background)");
            break;
        case NODE_PIPELINE:
            printf("PIPELINE (\n");
            for (int i = 0; i < node->data.pipeline.num_commands; i++) {
                print_ast_indent(node->data.pipeline.commands[i], indent + 1);
            }
            for (int i = 0; i < indent; i++) printf("  ");
            printf(")\n");
            break;
        case NODE_SUBSHELL:
            printf("SUBSHELL (\n");
            print_ast_indent(node->data.subshell.sub_node, indent + 1);
            for (int i = 0; i < indent; i++) printf("  ");
            printf(")\n");
            if (node->is_background_command) printf(" & (background)");
            break;
        case NODE_AND_OR:
            printf("AND_OR (%s\n", node->data.and_or.op == TOKEN_LOGICAL_AND ? "&&" : "||");
            print_ast_indent(node->data.and_or.left, indent + 1);
            print_ast_indent(node->data.and_or.right, indent + 1);
            for (int i = 0; i < indent; i++) printf("  ");
            printf(")\n");
            break;
        case NODE_SEQUENCE:
            printf("SEQUENCE (%s\n", node->data.sequence.is_background ? "&" : ";");
            for (int i = 0; i < node->data.sequence.num_nodes; i++) {
                print_ast_indent(node->data.sequence.nodes[i], indent + 1);
            }
            for (int i = 0; i < indent; i++) printf("  ");
            printf(")\n");
            break;
    }
    // Print redirections if they exist
    if (node->redirs != NULL) {
        printf(" [");
        if (node->redirs->input_file) printf(" < '%s'", node->redirs->input_file);
        if (node->redirs->output_file) {
            printf(" %s '%s'", node->redirs->append_output ? ">>" : ">", node->redirs->output_file);
        }
        printf("]");
    }
    // Add a newline for nodes that don't already ensure one
    if (node->type == NODE_COMMAND || (node->type == NODE_SUBSHELL && !node->is_background_command)) {
        printf("\n");
    }
}

static void print_ast(Node *node) {
    print_ast_indent(node, 0);
}

// Main Read-Eval-Print-Loop
void repl_loop() {
    Node *ast = NULL;


    while (true) {

        display_prompt(); // Print the prompt for a new command
        lexer_init(); // Initialize the lexer once at the beginning
        // parse_input_line now manages its internal current_token
        ast = parse_input_line();

        // Check for EOF using feof(stdin) after attempting to read.
        // This is more robust than relying on the parser's internal current_token state.
        if (feof(stdin)) {
            printf("\nExiting shell.\n");
            break; // Exit REPL on Ctrl+D
        }

        if (ast == NULL) {
            // If AST is NULL but not EOF, it means there was
            // a syntax error (error message already printed by parser)
            // or an empty input line (just EOL).
            // In either case, continue the loop for a new command.
            continue;
        }

        // If AST is successfully built, print it for debugging
        printf("--- Parsed AST ---\n");
        print_ast(ast);
        printf("------------------\n");

        int exit_status = execute_ast(ast); // Execute the parsed command
        if (exit_status != 0) {
            // Handle execution errors if necessary
            // For now, perror is called inside execute_simple_command
        }

        free_ast(ast); // Free the entire AST after use
        ast = NULL;    // Nullify the pointer
        lexer_cleanup();
    }

    // Clean up lexer and parser states before exiting
    parser_cleanup();
}

int main() {
    repl_loop();
    return 0;
}
