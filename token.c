#include "token.h"
#include <trace.h>

const char *token_strings[TOKEN_COUNT] = {
    [TOKEN_NONE] = "NONE",
    [TOKEN_ILLEGAL] = "ILLEGAL",
    [TOKEN_EOF] = "EOF",
    [TOKEN_COMMENT] = "COMMENT",

    // LITERAL_BEG
    /*
      Decimal: 42, -100
      Octal (prefix 0): 052
      Hexadecimal (prefix 0x): 0x2F

      123L (long integer)
      100U (unsigned integer)
      100UL (unsigned long integer)

      By default, floating-point literals are of type double. You can specify a
      literal as float using the f suffix.

      Decimal: 3.14, 0.001, -123.45
      Scientific notation: 1.23e4, 2e-2, 1.5e+3
      float literal: 3.14f, 1.5e-3f (with f suffix)

      char,string Literals


      Wide Character Literals (C99 and later)
      L'a', L'Ω'
      Universal Character Names
      L'\u03A9' (Unicode character for Omega Ω)

    */

    [TOKEN_IDENTIFIER] = "IDENTIFIER",
    [TOKEN_INT_LITERAL] = "INT_LITERAL",
    [TOKEN_FLOAT_LITERAL] = "FLOAT_LITERAL",
    [TOKEN_CHAR_LITERAL] = "CHAR_LITERAL",
    [TOKEN_STRING_LITERAL] = "STRING_LITERAL",
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
    [TOKEN_VOID] = "void",
    [TOKEN_INT] = "int",
    [TOKEN_FLOAT] = "float",
    [TOKEN_DOUBLE] = "double",
    [TOKEN_LONG] = "long",
    [TOKEN_SIGNED] = "signed",
    [TOKEN_UNSIGNED] = "unsigned",
    [TOKEN_STRUCT] = "struct",
    [TOKEN_CONST] = "const",
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
