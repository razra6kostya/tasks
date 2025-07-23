#ifndef LEXER_H
#define LEXER_H

#include <stdbool.h> 

// --- Token Types ---
typedef enum {
    TOKEN_WORD,                // Regular word (e.g., "ls", "-l", "file.txt")
    TOKEN_PIPE,                // '|' symbol
    TOKEN_REDIRECT_IN,         // '<' symbol
    TOKEN_REDIRECT_OUT,        // '>' symbol
    TOKEN_DOUBLE_REDIRECT_OUT, // '>>' symbols
    TOKEN_SEMICOLON,           // ';' symbol
    TOKEN_AMPERSAND,           // '&' symbol
    TOKEN_LOGICAL_AND,         // '&&' symbols
    TOKEN_LOGICAL_OR,          // '||' symbols
    TOKEN_LPAREN,              // '(' symbol
    TOKEN_RPAREN,              // ')' symbol
    TOKEN_ERROR,               // Lexical error (e.g., unmatched quotes, unrecognized character)
    TOKEN_EOF,                 // End of file/input (Ctrl+D)
    TOKEN_EOL                  // End of line (Enter key)
} TokenType;

// --- Token Structure ---
typedef struct {
    TokenType type;
    char *value; 
} Token;

// --- Function Prototypes ---
void lexer_init();
Token *get_next_token();
void free_token(Token *token);
void lexer_cleanup();
bool lexer_has_unclosed_quotes();

#endif
