#ifndef _TOKEN_H
#define _TOKEN_H

#include "def.h"
#include <stddef.h>
#include <stdio.h>

enum Token {
  TOKEN_NONE,
  TOKEN_ILLEGAL,
  TOKEN_EOF,
  TOKEN_COMMENT,

  // Literal types
  TOKEN_LITERAL_BEG,
  TOKEN_IDENTIFIER,
  TOKEN_INT_LITERAL,
  TOKEN_FLOAT_LITERAL,
  TOKEN_CHAR_LITERAL,
  TOKEN_STRING_LITERAL,
  TOKEN_LITERAL_END,

  // Operator types
  TOKEN_OPERATOR_BEG,
  TOKEN_LEFT_PARENTHESIS,  // (
  TOKEN_RIGHT_PARENTHESIS, // )
  TOKEN_LEFT_BRACKET,      // [
  TOKEN_RIGHT_BRACKET,     // ]
  TOKEN_LEFT_BRACE,        // {
  TOKEN_RIGHT_BRACE,       // }
  TOKEN_SEMICOLON,         // ;
  TOKEN_COLON,             // :
  TOKEN_COMMA,             // ,
  TOKEN_PERIOD,            // .
  TOKEN_OPERATOR_END,

  // Keyword types
  TOKEN_KEYWORD_BEG,
  TOKEN_RETURN,   // return
  TOKEN_MAIN,     // main
  TOKEN_VOID,     // void
  TOKEN_INT,      // int
  TOKEN_FLOAT,    // float
  TOKEN_DOUBLE,   // double
  TOKEN_LONG,     // long
  TOKEN_SIGNED,   // signed
  TOKEN_UNSIGNED, // unsigned
  TOKEN_STRUCT,   // struct
  TOKEN_CONST,    // const
  TOKEN_KEYWORD_END,

  TOKEN_ADDITIONAL_BEG,
  TOKEN_ADDITIONAL_END,
  TOKEN_COUNT
};

extern const char *token_strings[TOKEN_COUNT];

const char *token_to_string(enum Token t);

struct TokenPosition {
  // char *filename; // filename, if any
  struct Scanner *scanner; // If filename is needed ask the associated scanner
  sizet offset;
  sizet lineoffset;
  sizet linecharoffset;
};

#endif // _TOKEN_H
