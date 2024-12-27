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
  TOKEN_VOID,
  TOKEN_INT,
  TOKEN_FLOAT,
  TOKEN_CHAR,
  TOKEN_STRING,
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
  TOKEN_RETURN, // return
  TOKEN_MAIN,   // main
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
