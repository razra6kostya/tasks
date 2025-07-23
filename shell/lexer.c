#include "lexer.h"
#include "mem_utils.h"

#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int current_char = -1;
static bool unclosed_quotes_error = false;

static int read_char()
{
    current_char = fgetc(stdin);
    return current_char;
}

static void skip_whitespace()
{
    while (current_char != EOF && current_char != '\n' && isspace(current_char)) {
        read_char();
    }
}

static Token *create_token(TokenType type, const char *value) {
    Token *token = (Token *)safe_malloc(sizeof(Token));
    token->type = type;
    token->value = (value != NULL) ? safe_strdup(value) : NULL;
    return token;
}

void lexer_init()
{
    if (current_char == -1) {
        read_char();
    }
    unclosed_quotes_error = false;

    // Debug:
    printf("[DEBUG] Lexer: lexer_init finished. current_char: '%c' (0x%x)\n",
           current_char == EOF ? 'E' : (current_char == '\n' ? 'N' : current_char), current_char);
}

Token *get_next_token()
{
    // Reset unclosed quotes error flag at the beginning of a new line or EOF
    if (current_char == '\n' || current_char == EOF) {
        unclosed_quotes_error = false;
    }

    // Debug: Char before skip_whitespace
    printf("[DEBUG] Lexer: get_next_token starting. current_char before skip_whitespace: '%c' (0x%x)\n",
           current_char == EOF ? 'E' : (current_char == '\n' ? 'N' : current_char), current_char);

    skip_whitespace();

    if (current_char == '\n') {
        //read_char(); // Consume the newline
        printf("[DEBUG] Lexer: Returning TOKEN_EOL.\n");
        return create_token(TOKEN_EOL, NULL);
    }
    if (current_char == EOF) {
        printf("[DEBUG] Lexer: Returning TOKEN_EOF.\n");
        return create_token(TOKEN_EOF, NULL);
    }
    
    int c = current_char;

    if (c == '|') {
        read_char();
        if (current_char == '|') {
            read_char(); return create_token(TOKEN_LOGICAL_OR, "||");
        }
        return create_token(TOKEN_PIPE, "|");
    } else if (c == '&') {
        read_char();
        if (current_char == '&') {
            read_char(); return create_token(TOKEN_LOGICAL_AND, "&&");
        }
        return create_token(TOKEN_AMPERSAND, "&");
    } else if (c == '>') {
        read_char();
        if (current_char == '>') {
            read_char(); return create_token(TOKEN_DOUBLE_REDIRECT_OUT, ">>");
        }
        return create_token(TOKEN_REDIRECT_OUT, ">");
    } else if (c == '<') {
        read_char(); return create_token(TOKEN_REDIRECT_IN, "<");
    } else if (c == ';') {
        read_char(); return create_token(TOKEN_SEMICOLON, ";");
    } else if (c == '(') {
        read_char(); return create_token(TOKEN_LPAREN, "(");
    } else if (c == ')') {
        read_char(); return create_token(TOKEN_RPAREN, ")");
    }
    
    char *word_buffer = NULL;
    int buffer_capacity = 0;
    int word_len = 0;
    bool in_quotes = false;
    int quote_count = 0;

    bool started_with_quote = (c == '"');

    word_buffer = safe_malloc(buffer_capacity);

    while ((current_char != EOF && current_char != '\n')  &&
        (in_quotes || (!isspace(current_char) && !strchr("|&;><()", current_char)))) {

        c = current_char;

        if (buffer_capacity <= word_len) {
            buffer_capacity = (buffer_capacity == 0) ? 16 : buffer_capacity * 2;
            word_buffer = safe_realloc(word_buffer, buffer_capacity);
        }

        if (c == '\\') {
            read_char();
            if (current_char == EOF || current_char == '\n') {
                word_buffer[word_len++] = '\\';
                break;
            }
            word_buffer[word_len++] = current_char;
        } else if (c == '"') {
            quote_count++;
            in_quotes = !in_quotes;
        } else {
            word_buffer[word_len++] = c;
        }
        read_char();

    }
    word_buffer[word_len] = '\0';

    if (in_quotes) {
      unclosed_quotes_error = true;
      safe_free(word_buffer);
      return create_token(TOKEN_ERROR, "unclosed quotes");
    }

    if (started_with_quote && word_len == 0 && quote_count  == 2) {
        if (isspace(current_char) || current_char == EOF || 
                current_char == '\n' || strchr("|&;><()", current_char)) {
            safe_free(word_buffer);
            return create_token(TOKEN_WORD, "");
        }
    }
    printf("[DEBUG] Lexer: Returning TOKEN_WORD '%s'.\n", word_buffer); 
    Token *tmp = create_token(TOKEN_WORD, word_buffer);
    safe_free(word_buffer);
    return tmp;
}

void free_token(Token *token) {
    if (token == NULL) return;
    safe_free(token->value);
    safe_free(token);        
}

void lexer_cleanup() {
    current_char = -1;
    unclosed_quotes_error = false;
}

bool lexer_has_unclosed_quotes() {
    return unclosed_quotes_error;
}
