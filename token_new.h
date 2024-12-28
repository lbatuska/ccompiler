#ifndef _TOKEN_H
#define _TOKEN_H

typedef enum {
  TOKEN_INVALID,
  // [a-zA-Z_][a-zA-Z0-9_]*
  TOKEN_KEYWORD,
  TOKEN_DELIMITER,
  TOKEN_OPERATOR,
  // [a-zA-Z_][a-zA-Z0-9_]*
  TOKEN_IDENTIFIER,
  TOKEN_LITERAL,
  TOKEN_EOF,
} TokenType;

typedef enum {
  // Type specifiers
  KEYWORD_CHAR,
  KEYWORD_INT,
  KEYWORD_FLOAT,
  KEYWORD_DOUBLE,
  KEYWORD_VOID,

  // Type qualifiers
  KEYWORD_CONST,
  KEYWORD_VOLATILE,

  // Modifiers
  KEYWORD_SIGNED,
  KEYWORD_UNSIGNED,
  KEYWORD_SHORT,
  KEYWORD_LONG,

  // Control statements
  KEYWORD_IF,
  KEYWORD_ELSE,
  KEYWORD_FOR,
  KEYWORD_WHILE,
  KEYWORD_SWITCH,
  KEYWORD_CASE,

  // Storage classes
  KEYWORD_AUTO,
  KEYWORD_EXTERN,
  KEYWORD_REGISTER,
  KEYWORD_STATIC,

  // Other
  KEYWORD_RETURN,
  KEYWORD_SIZEOF,
  KEYWORD_GOTO,
  KEYWORD_CONTINUE,
  KEYWORD_BREAK,

  // C99 additions
  KEYWORD_INLINE,     // inline keyword for inlining functions
  KEYWORD_RESTRICT,   // restrict keyword for pointer aliasing
  KEYWORD__BOOL,      // _Bool for boolean type
  KEYWORD__COMPLEX,   // _Complex for complex numbers
  KEYWORD__IMAGINARY, // _Imaginary for imaginary numbers

  // C11 additions
  KEYWORD__ALIGNAS,       // _Alignas for alignment specification
  KEYWORD__ALIGNOF,       // _Alignof for alignment queries
  KEYWORD__ATOMIC,        // _Atomic for atomic types
  KEYWORD__GENERIC,       // _Generic for generic selection
  KEYWORD__NORETURN,      // _Noreturn for functions that do not return
  KEYWORD__STATIC_ASSERT, // _Static_assert for compile-time assertions
  KEYWORD__THREAD_LOCAL   // _Thread_local for thread-local storage
} KeywordType;

typedef enum {
  DELIM_OPEN_BRACE,    // {
  DELIM_CLOSE_BRACE,   // }
  DELIM_OPEN_PAREN,    // (
  DELIM_CLOSE_PAREN,   // )
  DELIM_OPEN_BRACKET,  // [
  DELIM_CLOSE_BRACKET, // ]
  DELIM_SEMICOLON,     // ;
  DELIM_COMMA          // ,
} DelimiterType;

typedef enum {
  // Arithmetic operators
  OPERATOR_PLUS,    // +
  OPERATOR_MINUS,   // -
  OPERATOR_STAR,    // *
  OPERATOR_SLASH,   // /
  OPERATOR_PERCENT, // %

  // Increment and decrement
  OPERATOR_INCREMENT, // ++
  OPERATOR_DECREMENT, // --

  // Bitwise operators
  OPERATOR_AMP,         // &
  OPERATOR_PIPE,        // |
  OPERATOR_CARET,       // ^
  OPERATOR_TILDE,       // ~
  OPERATOR_LEFT_SHIFT,  // <<
  OPERATOR_RIGHT_SHIFT, // >>

  // Logical operators
  OPERATOR_LOGICAL_AND, // &&
  OPERATOR_LOGICAL_OR,  // ||
  OPERATOR_LOGICAL_NOT, // !

  // Comparison operators
  OPERATOR_EQUAL,         // ==
  OPERATOR_NOT_EQUAL,     // !=
  OPERATOR_LESS,          // <
  OPERATOR_GREATER,       // >
  OPERATOR_LESS_EQUAL,    // <=
  OPERATOR_GREATER_EQUAL, // >=

  // Assignment operators
  OPERATOR_ASSIGN,             // =
  OPERATOR_PLUS_ASSIGN,        // +=
  OPERATOR_MINUS_ASSIGN,       // -=
  OPERATOR_STAR_ASSIGN,        // *=
  OPERATOR_SLASH_ASSIGN,       // /=
  OPERATOR_PERCENT_ASSIGN,     // %=
  OPERATOR_AMP_ASSIGN,         // &=
  OPERATOR_PIPE_ASSIGN,        // |=
  OPERATOR_CARET_ASSIGN,       // ^=
  OPERATOR_LEFT_SHIFT_ASSIGN,  // <<=
  OPERATOR_RIGHT_SHIFT_ASSIGN, // >>=

  // Pointer-related operators
  OPERATOR_DEREFERENCE, // *
  OPERATOR_ADDRESS_OF,  // &

  // Miscellaneous
  OPERATOR_ARROW,    // ->
  OPERATOR_DOT,      // .
  OPERATOR_QUESTION, // ?
  OPERATOR_COLON     // :
} OperatorType;

typedef enum {
  // Integer literals
  // [0-9]+
  LITERAL_INTEGER, // 42

  // Floating-point literals
  LITERAL_FLOAT, // 3.14

  // Character literals
  // \'([^\\']|\\.)*\'
  LITERAL_CHAR, // 'a'

  // String literals
  // \"([^\\"]|\\.)*\"
  LITERAL_STRING, // "Hello, world!"

  // Boolean literals (C99)
  LITERAL_BOOL, // _Bool true/false

  // Complex literals (C99)
  LITERAL_COMPLEX, // 1.0 + 2.0i
} LiteralType;

typedef union {
  KeywordType keyword;     // For TOKEN_KEYWORD
  DelimiterType delimiter; // For TOKEN_DELIMITER
  OperatorType operator;   // For TOKEN_OPERATOR
  char *identifier;        // For TOKEN_IDENTIFIER
  char *literal_value;     // For TOKEN_LITERAL (e.g., "2" for integer literal)
} TokenValue;

typedef struct {
  TokenType type;   // Broad token category
  TokenValue value; // Specific token data
  int offset;
  int line;   // Line number in source
  int column; // Column number in source
} Token;

#endif // _TOKEN_H
