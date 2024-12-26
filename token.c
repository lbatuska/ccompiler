#include "token.h"
#include <trace.h>

const char *token_strings[TOKEN_COUNT] = {
    [TOKEN_NONE] = "NONE",
    [TOKEN_ILLEGAL] = "ILLEGAL",
    [TOKEN_EOF] = "EOF",
    [TOKEN_COMMENT] = "COMMENT",

    // LITERAL_BEG
    [TOKEN_IDENTIFIER] = "IDENTIFIER",
    [TOKEN_VOID] = "VOID",
    [TOKEN_INT] = "INT",
    [TOKEN_FLOAT] = "FLOAT",
    [TOKEN_CHAR] = "CHAR",
    [TOKEN_STRING] = "STRING",
    // LITERAL_END

    // OPERATOR_BEG
    [TOKEN_LEFT_PARENTHESIS] = "(",
    [TOKEN_RIGHT_PARENTHESIS] = ")",
    [TOKEN_LEFT_BRACKET] = "[",
    [TOKEN_RIGHT_BRACKET] = "]",
    [TOKEN_LEFT_BRACE] = "{",
    [TOKEN_RIGHT_BRACE] = "}",
    [TOKEN_SEMICOLON] = ";",
    [TOKEN_COLON] = ":",
    [TOKEN_COMMA] = ",",
    [TOKEN_PERIOD] = ".",
    // OPERATOR_END

    // KEYWORD_BEG
    [TOKEN_RETURN] = "return",
    [TOKEN_MAIN] = "main",
    // KEYWORD_END

    // ADDITIONAL_BEG
    // ADDITIONAL_END
};

const char *token_to_string(enum Token t) {
  trace_printf_ascend("token_to_string\n");
  if (t < 0 || t >= TOKEN_COUNT) {
    trace_descend();
    return "UNKNOWN";
  }
  trace_descend();
  return token_strings[t];
}
