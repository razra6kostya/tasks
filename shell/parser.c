#include "parser.h"
#include "lexer.h"
#include "mem_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// --- Global Parser State (static variable, accessible only within this file) ---
static Token *current_token = NULL; // The current token being analyzed

// --- Helper Functions for Token Management ---

// Gets the next token from the lexer and frees the previous one (if any).
// Returns the new current token.
static Token *get_and_advance_token() {
    if (current_token != NULL) {
        free_token(current_token);
    }
    current_token = get_next_token();
    if (current_token == NULL) {
        fprintf(stderr, "Parser: Fatal error getting next token (memory or IO error).\n");
        exit(EXIT_FAILURE);
    }
    return current_token;
}

// Checks the type of the current token without consuming it.
static bool match(TokenType type) {
    return current_token != NULL && current_token->type == type;
}

// Checks the type of the current token and advances to the next one if it matches.
// Returns true on match, false otherwise.
static bool expect(TokenType type) {
    if (match(type)) {
        get_and_advance_token();
        return true;
    }
    return false;
}

// Generates a parsing error message.
static void parse_error(const char *message) {
    fprintf(stderr, "Parser error: %s. Current token type: %d (%s).\n",
            message,
            current_token ? current_token->type : -1,
            current_token && current_token->value ? current_token->value : "NULL");
    // In a real shell: free AST, skip until EOL, restart.
    // For simplicity here, we return NULL to indicate an error.
}

// --- Functions for Creating AST Nodes and Freeing Memory ---

static Node *create_node(NodeType type) {
    Node *node = (Node *)safe_malloc(sizeof(Node));
    memset(node, 0, sizeof(Node)); // Zero-initialize fields
    node->type = type;
    return node;
}

Redirections *create_redirections() {
    Redirections *redirs = (Redirections *)safe_malloc(sizeof(Redirections));
    memset(redirs, 0, sizeof(Redirections));
    return redirs;
}

SimpleCommand *create_simple_command() {
    SimpleCommand *cmd = (SimpleCommand *)safe_malloc(sizeof(SimpleCommand));
    memset(cmd, 0, sizeof(SimpleCommand));
    cmd->argv = (char **)safe_malloc(sizeof(char *)); // Initial array with one NULL
    cmd->argv[0] = NULL;
    cmd->argc = 0;
    return cmd;
}

void free_redirections(Redirections *redirs) {
    if (redirs == NULL) return;
    safe_free(redirs->input_file);
    safe_free(redirs->output_file);
    safe_free(redirs);
}

void free_simple_command(SimpleCommand *cmd) {
    if (cmd == NULL) return;
    if (cmd->argv != NULL) {
        for (int i = 0; i < cmd->argc; i++) {
            safe_free(cmd->argv[i]);
        }
        safe_free(cmd->argv);
    }
    safe_free(cmd); // Redirections are freed at the Node level
}

void free_ast(Node *node) {
    if (node == NULL) return;

    switch (node->type) {
        case NODE_COMMAND:
            free_simple_command(node->data.command);
            break;
        case NODE_PIPELINE:
            if (node->data.pipeline.commands != NULL) {
                for (int i = 0; i < node->data.pipeline.num_commands; i++) {
                    free_ast(node->data.pipeline.commands[i]);
                }
                safe_free(node->data.pipeline.commands);
            }
            break;
        case NODE_SUBSHELL:
            free_ast(node->data.subshell.sub_node);
            break;
        case NODE_AND_OR:
            free_ast(node->data.and_or.left);
            free_ast(node->data.and_or.right);
            break;
        case NODE_SEQUENCE:
            if (node->data.sequence.nodes != NULL) {
                for (int i = 0; i < node->data.sequence.num_nodes; i++) {
                    free_ast(node->data.sequence.nodes[i]);
                }
                safe_free(node->data.sequence.nodes);
            }
            break;
    }
    free_redirections(node->redirs); // Free redirections if present at the node level
    safe_free(node);
}

// --- Parsing Functions (in order of precedence, highest to lowest) ---
static Node *parse_pipeline_list(); // Forward declaration for recursion
static Node *parse_pipeline();
static Node *parse_command();
static Node *parse_simple_command();
static Redirections *parse_redirections(); // Helper for parsing redirections

// Processes all redirections following a command or subshell.
static Redirections *parse_redirections() {
    Redirections *redirs = NULL;

    while (match(TOKEN_REDIRECT_IN) || match(TOKEN_REDIRECT_OUT) || match(TOKEN_DOUBLE_REDIRECT_OUT)) {
        if (redirs == NULL) {
            redirs = create_redirections();
        }

        TokenType redir_type = current_token->type;
        get_and_advance_token(); // Consume the redirection token

        if (!match(TOKEN_WORD)) {
            parse_error("Expected filename after redirection operator.");
            free_redirections(redirs);
            return NULL; // Error
        }

        char *filename = current_token->value;
        if (redir_type == TOKEN_REDIRECT_IN) {
            if (redirs->input_file != NULL) safe_free(redirs->input_file);
            redirs->input_file = safe_strdup(filename);
        } else if (redir_type == TOKEN_REDIRECT_OUT) {
            if (redirs->output_file != NULL) safe_free(redirs->output_file);
            redirs->output_file = safe_strdup(filename);
            redirs->append_output = false;
        } else if (redir_type == TOKEN_DOUBLE_REDIRECT_OUT) {
            if (redirs->output_file != NULL) safe_free(redirs->output_file);
            redirs->output_file = safe_strdup(filename);
            redirs->append_output = true;
        }
        get_and_advance_token(); // Consume the filename token
    }
    return redirs;
}


// simple_command ::= WORD+
// Parses a simple command (a sequence of words).
static Node *parse_simple_command() {
    if (!match(TOKEN_WORD)) {
        return NULL;
    }

    Node *node = create_node(NODE_COMMAND);
    node->data.command = create_simple_command();
    SimpleCommand *cmd = node->data.command;

    size_t argv_capacity = 4;
    cmd->argv = (char **)safe_realloc(cmd->argv, sizeof(char *) * argv_capacity);

    while (match(TOKEN_WORD)) {
        if (cmd->argc + 1 >= argv_capacity) { // +1 for NULL-terminator
            argv_capacity *= 2;
            cmd->argv = (char **)safe_realloc(cmd->argv, sizeof(char *) * argv_capacity);
        }
        cmd->argv[cmd->argc++] = safe_strdup(current_token->value);
        get_and_advance_token();
    }
    cmd->argv = (char **)safe_realloc(cmd->argv, sizeof(char *) * (cmd->argc + 1)); // Trim and add NULL
    cmd->argv[cmd->argc] = NULL; // NULL-terminator

    return node;
}

// command ::= simple_command ( ( '<' | '>' | '>>' ) WORD )*
//           | '(' pipeline_list ')' ( ( '<' | '>' | '>>' ) WORD )*
static Node *parse_command() {
    Node *node = NULL;
    Redirections *redirs = NULL;

    if (match(TOKEN_LPAREN)) { // Parse a subshell
        get_and_advance_token(); // Consume '('
        Node *sub_node = parse_pipeline_list(); // Parse the inner pipeline_list
        if (sub_node == NULL) {
            // If there's an error in the subshell, the whole command is invalid
            parse_error("Empty or invalid subshell content.");
            return NULL;
        }
        if (!expect(TOKEN_RPAREN)) {
            parse_error("Expected ')' to close subshell.");
            free_ast(sub_node);
            return NULL;
        }
        node = create_node(NODE_SUBSHELL);
        node->data.subshell.sub_node = sub_node;
        redirs = parse_redirections(); // Redirections can follow parentheses
        node->redirs = redirs; // Attach redirections to the subshell node
    } else { // Parse a simple command
        node = parse_simple_command();
        if (node == NULL) {
            return NULL;
        }
        redirs = parse_redirections(); // Redirections after a simple command
        node->redirs = redirs; // Attach redirections to the command node
    }
    return node;
}

// pipeline ::= command ( '|' command )*
static Node *parse_pipeline() {
    Node *left_command = parse_command();
    if (left_command == NULL) {
        return NULL; // Empty pipeline or error in the first command
    }

    if (match(TOKEN_PIPE)) {
        Node *pipeline_node = create_node(NODE_PIPELINE);
        size_t cmd_capacity = 2;
        pipeline_node->data.pipeline.commands = (Node **)safe_malloc(sizeof(Node *) * cmd_capacity);
        pipeline_node->data.pipeline.commands[0] = left_command;
        pipeline_node->data.pipeline.num_commands = 1;

        while (match(TOKEN_PIPE)) {
            get_and_advance_token(); // Consume '|'
            Node *right_command = parse_command();
            if (right_command == NULL) {
                parse_error("Expected command after '|'");
                free_ast(pipeline_node);
                return NULL;
            }
            if (pipeline_node->data.pipeline.num_commands + 1 > cmd_capacity) {
                cmd_capacity *= 2;
                pipeline_node->data.pipeline.commands = (Node **)safe_realloc(pipeline_node->data.pipeline.commands, sizeof(Node *) * cmd_capacity);
            }
            pipeline_node->data.pipeline.commands[pipeline_node->data.pipeline.num_commands++] = right_command;
        }
        // Resize to exact number of commands
        pipeline_node->data.pipeline.commands = (Node **)safe_realloc(pipeline_node->data.pipeline.commands, sizeof(Node *) * pipeline_node->data.pipeline.num_commands);
        return pipeline_node;
    }
    return left_command; // If no '|', it's just a single command
}

// pipeline_list ::= pipeline ( (';' | '&' | '&&' | '||') pipeline )* [';' | '&']?
static Node *parse_pipeline_list() {
    Node *left_node = parse_pipeline();
    if (left_node == NULL) {
        return NULL;
    }

    Node *current_list_node = left_node; // This will be the root of the list if it has only one element

    while (match(TOKEN_SEMICOLON) || match(TOKEN_AMPERSAND) || match(TOKEN_LOGICAL_AND) || match(TOKEN_LOGICAL_OR)) {
        TokenType op_type = current_token->type;
        get_and_advance_token(); // Consume the delimiter

        // If it's ';', '&', '&&', '||' followed immediately by EOL, EOF, or ')' (for subshell),
        // it's a valid terminating delimiter.
        if ((match(TOKEN_EOL) || match(TOKEN_EOF) || match(TOKEN_RPAREN))) {
            // If it's a background process, set is_background_command for the last command in the list.
            if (op_type == TOKEN_AMPERSAND) {
                if (current_list_node->type == NODE_COMMAND || current_list_node->type == NODE_SUBSHELL) {
                    current_list_node->is_background_command = true;
                }
                // For NODE_PIPELINE and NODE_AND_OR, backgrounding is more complex, leaving as is for now.
            }
            break; // Just end the list
        }

        Node *right_node = parse_pipeline(); // The next element in the list

        if (right_node == NULL) {
            parse_error("Expected a command after operator.");
            free_ast(current_list_node); // Free what was built so far
            return NULL;
        }

        if (op_type == TOKEN_LOGICAL_AND || op_type == TOKEN_LOGICAL_OR) {
            Node *and_or_node = create_node(NODE_AND_OR);
            and_or_node->data.and_or.left = current_list_node;
            and_or_node->data.and_or.right = right_node;
            and_or_node->data.and_or.op = op_type;
            current_list_node = and_or_node; // Update the root for the next iteration
        } else { // TOKEN_SEMICOLON or TOKEN_AMPERSAND
            // Create a sequence node
            Node *sequence_node;
            if (current_list_node->type == NODE_SEQUENCE &&
                current_list_node->data.sequence.is_background == (op_type == TOKEN_AMPERSAND)) {
                // If it's already a sequence of the same type (background/foreground), append to it.
                sequence_node = current_list_node;
            } else {
                // Otherwise, create a new sequence.
                sequence_node = create_node(NODE_SEQUENCE);
                sequence_node->data.sequence.nodes = (Node **)safe_malloc(sizeof(Node *));
                sequence_node->data.sequence.nodes[0] = current_list_node;
                sequence_node->data.sequence.num_nodes = 1;
                sequence_node->data.sequence.is_background = (op_type == TOKEN_AMPERSAND);
            }

            size_t current_num = sequence_node->data.sequence.num_nodes;
            sequence_node->data.sequence.nodes = (Node **)safe_realloc(sequence_node->data.sequence.nodes, sizeof(Node *) * (current_num + 1));
            sequence_node->data.sequence.nodes[current_num] = right_node;
            sequence_node->data.sequence.num_nodes++;

            if (op_type == TOKEN_AMPERSAND) {
                 if (right_node->type == NODE_COMMAND || right_node->type == NODE_SUBSHELL) {
                     right_node->is_background_command = true;
                 }
            }
            current_list_node = sequence_node;
        }
    }
    return current_list_node;
}

// --- Main Parser Entry Point ---

Node *parse_input_line() {
    // Debug: Print token before processing
    printf("[DEBUG] Parser: Starting parse_input_line. Current token before advance: type=%d (%s)\n",
           current_token ? current_token->type : -1,
           current_token && current_token->value ? current_token->value : "NULL");

    // Get the first token to start parsing.
    get_and_advance_token();

    // Debug: Print token after advance
    printf("[DEBUG] Parser: After advance. Current token: type=%d (%s)\n",
           current_token ? current_token->type : -1,
           current_token && current_token->value ? current_token->value : "NULL");

    // If the very first thing is EOL or EOF, it's an empty line.
    if (match(TOKEN_EOL)) { // We explicitly handle EOL here as a valid empty input
        get_and_advance_token(); // Consume the EOL
        return NULL; // Empty input
    }
    if (match(TOKEN_EOF)) { // Handle EOF explicitly
        return NULL; // EOF input, main loop will handle exit
    }

    Node *ast = parse_pipeline_list();

    // Debug: Print AST status
    printf("[DEBUG] Parser: parse_pipeline_list returned AST: %p\n", (void*)ast);
    
    // After parsing a command or list of commands, expect EOL, EOF, or ')' (if inside a subshell).
    // If something remains and it's not EOL/EOF, it's a syntax error.
    if (!(match(TOKEN_EOL) || match(TOKEN_EOF) || match(TOKEN_RPAREN))) {
        parse_error("Unexpected token after command.");
        free_ast(ast); // Free what was built
        ast = NULL; // Return NULL, signaling an error
    } else {
        // IMPORTANT: Consume the EOL or EOF after successful parsing,
        // so the next call to parse_input_line starts clean.
        if (match(TOKEN_EOL) || match(TOKEN_EOF)) {
            printf("[DEBUG] Parser: Consuming trailing TOKEN_EOL.\n");
            get_and_advance_token();
        }
    }

    // Debug: Final AST returned    
    printf("[DEBUG] Parser: parse_input_line returning AST: %p\n", (void*)ast);
    
    return ast;
}

// --- Parser State Cleanup Function ---
void parser_cleanup() {
    if (current_token != NULL) {
        free_token(current_token);
        current_token = NULL;
    }
}
