#include <assert.h>
#include <string.h>
#include <tree_sitter/parser.h>
#include <wctype.h>

enum TokenType { RAW_STRING_DELIMITER, RAW_STRING_CONTENT };

/// The spec limits delimiters to 16 chars
#define MAX_DELIMITER_LENGTH 16

typedef struct {
    /// Last raw string delimiter
    wchar_t delimiter[MAX_DELIMITER_LENGTH];
    /// Last delimiter length
    uint8_t length;
} Scanner;

/// Advance to the next character.
static inline void advance(TSLexer *lexer) { lexer->advance(lexer, false); }

/// Scan a raw string delimiter in R"delimiter(content)delimiter".
static bool scan_raw_string_delimiter(Scanner *scanner, TSLexer *lexer) {
    if (scanner->length > 0) {
        // Closing delimiter: must exactly match the opening delimiter.
        // We already checked this when scanning content, but this is how we
        // know when to stop. We can't stop at ", because R"""hello""" is valid.
        for (int i = 0; i < scanner->length; ++i) {
            if (lexer->lookahead != scanner->delimiter[i])
                return false;
            advance(lexer);
        }
        scanner->length = 0;
        memset(scanner->delimiter, 0, sizeof scanner->delimiter);
        return true;
    }

    // Opening delimiter: record the d-char-sequence up to (.
    // d-char is any basic character except parens, backslashes, and spaces.
    for (;;) {
        if (scanner->length >= MAX_DELIMITER_LENGTH || lexer->eof(lexer) ||
            lexer->lookahead == '\\' || iswspace(lexer->lookahead)) {
            return false;
        }
        if (lexer->lookahead == '(') {
            // Rather than create a token for an empty delimiter, we fail and
            // let the grammar fall back to a delimiter-less rule.
            return scanner->length > 0;
        }
        scanner->delimiter[scanner->length++] = lexer->lookahead;
        advance(lexer);
    }
}

/// Scan the raw string content in R"delimiter(content)delimiter".
static bool scan_raw_string_content(Scanner *scanner, TSLexer *lexer) {
    // The progress made through the delimiter since the last ')'.
    // The delimiter may not contain ')' so a single counter suffices.
    int idx = -1;
    for (;;) {
        // If we hit EOF, consider the content to terminate there.
        // This forms an incomplete raw_string_literal, and models the code
        // well.
        if (lexer->eof(lexer)) {
            lexer->mark_end(lexer);
            return true;
        }

        if (idx >= 0) {
            if (idx == scanner->length) {
                if (lexer->lookahead == '"') {
                    return true;
                } else {
                    idx = -1;
                }
            } else {
                if (lexer->lookahead == scanner->delimiter[idx]) {
                    idx++;
                } else {
                    idx = -1;
                }
            }
        }

        if (idx == -1 && lexer->lookahead == ')') {
            // The content doesn't include the )delimiter" part.
            // We must still scan through it, but exclude it from the token.
            lexer->mark_end(lexer);
            idx = 0;
        }

        advance(lexer);
    }
}

void *tree_sitter_arduino_external_scanner_create() {
    Scanner *scanner = (Scanner *) calloc(1, sizeof (Scanner));
    memset(scanner, 0, sizeof (Scanner));
    return scanner;
}

bool tree_sitter_arduino_external_scanner_scan(void *payload, TSLexer *lexer, const bool *valid_symbols) {
    // No skipping leading whitespace: raw-string grammar is space-sensitive.
    Scanner *scanner = (Scanner *) payload;

    if (valid_symbols[RAW_STRING_DELIMITER]) {
        lexer->result_symbol = RAW_STRING_DELIMITER;
        return scan_raw_string_delimiter(scanner, lexer);
    }

    if (valid_symbols[RAW_STRING_CONTENT]) {
        lexer->result_symbol = RAW_STRING_CONTENT;
        return scan_raw_string_content(scanner, lexer);
    }

    return false;
}

unsigned tree_sitter_arduino_external_scanner_serialize(void *payload, char *buffer) {
    assert(MAX_DELIMITER_LENGTH * sizeof (wchar_t) < TREE_SITTER_SERIALIZATION_BUFFER_SIZE &&
           "Serialized delimiter is too long!");

    Scanner *scanner = (Scanner *) payload;
    size_t size = scanner->length * sizeof (wchar_t);
    memcpy(buffer, scanner->delimiter, size);
    return (unsigned) size;
}

void tree_sitter_arduino_external_scanner_deserialize(void *payload, const char *buffer, unsigned length) {
    assert(length % sizeof (wchar_t) == 0 && "Can't decode serialized delimiter!");

    Scanner *scanner = (Scanner *) payload;
    scanner->length = length / sizeof (wchar_t);
    memcpy(&scanner->delimiter[0], buffer, length);
}

void tree_sitter_arduino_external_scanner_destroy(void *payload) {
    Scanner *scanner = (Scanner *) payload;
    free(scanner);
}
