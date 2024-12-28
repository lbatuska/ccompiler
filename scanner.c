#include "scanner.h"
#include "def.h"
#include "token.h"
#include <debug.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <trace.h>

void compile_regexes();
struct filecontent read_file(char *filename);

static regex_t regex_identifier, regex_constant;
static int regexes_compiled = 0;

struct filecontent {
  sizet len; // includes \0 char
  char *data;
};

struct Scanner {
  // Scanner state
  char *filename;
  char *src;
  sizet src_len; // includes the \0 char
  // Scan state
  char current_char;
  sizet current_offset;
  sizet current_lineoffset;
  sizet current_linecharoffset;
};

char *ScannerGetFileName(struct Scanner *s) {
  if (!s) {
    trace_printf_ascend("ScannerGetFileName(s) -> s is null!\n");
    trace_descend();
    return NULL;
  }
  trace_printf_ascend("ScannerGetFileName(\"%s\")\n", s->filename);
  trace_descend();
  return s ? s->filename : NULL;
};

// Advances the internal current character to the next (while skipping white
// spaces, newlines)
void next(struct Scanner *s) {
  trace_printf_ascend("next(\"%s\")\n", s->filename);
  if (s->current_offset < s->src_len) {
    s->current_offset++;
    s->current_linecharoffset++;

    if (s->current_char == '\n') {
      s->current_lineoffset++;
      s->current_linecharoffset = 0;
    }

    s->current_char = s->src[s->current_offset];
  }
  trace_descend();
}

void skipwhitespace(struct Scanner *s) {
  trace_printf_ascend("skipwhitespace(\"%s\")\n", s->filename);
  while (s->current_char == ' ' || s->current_char == '\t' ||
         s->current_char == '\n' || s->current_char == '\r') {
    next(s);
  }
  trace_descend();
}

struct Scanner *ScannerInit(char *filename) {
  struct Scanner *s = malloc(sizeof(struct Scanner));
  trace_printf_ascend("ScannerInit(\"%s\")\n", filename);
  compile_regexes();
  if (!filename) {
    goto failed;
  }
  s->filename = filename;
  struct filecontent fc = read_file(filename);
  if (!fc.data || fc.len < 1) {
    debug_fprintf(stderr, "Could not read file: %s\n", filename);
    goto failed;
  }
  s->src_len = fc.len;
  s->src = fc.data;
  s->current_char = fc.data ? fc.data[0] : 0;
  s->current_offset = 0;
  s->current_lineoffset = 0;
  s->current_linecharoffset = 0;
  skipwhitespace(s);
  // Success
  trace_descend();
  return s;
failed:
  trace_descend();
  free(s);
  return NULL;
};

enum Token isKeyword(char *begin, sizet len) {
  if (strncmp(begin, "int", len) == 0) {
    return TOKEN_INT;
  }

  if (strncmp(begin, "main", len) == 0) {
    return TOKEN_MAIN;
  }

  if (strncmp(begin, "void", len) == 0) {
    return TOKEN_VOID;
  }

  if (strncmp(begin, "return", len) == 0) {
    return TOKEN_RETURN;
  }

  return TOKEN_NONE;
}

struct ScannerResult ScannerScan(struct Scanner *s) {
  struct ScannerResult result = {.pos.scanner = s, .literal = NULL};
  if (!s) {
    trace_printf_ascend("ScannerScan(s) -> s is null!\n");
    result.token = TOKEN_ILLEGAL;
    result.pos.offset = -1;
    trace_descend();
    return result;
  }
  trace_printf_ascend("ScannerScan(\"%s\")\n", s->filename);
  skipwhitespace(s);
  result.pos.offset = s->current_offset;
  result.pos.lineoffset = s->current_lineoffset;
  result.pos.linecharoffset = s->current_linecharoffset;

  if (*(s->src + s->current_offset) == '\0' ||
      s->current_offset >= (s->src_len)) {
    debug_printf("Reached EOF! \n");
    result.token = TOKEN_EOF;
    goto end;
  }

  int consumed = 1;
  sizet len = 0;
  switch (s->current_char) {
  case '(':
    result.token = TOKEN_LEFT_PARENTHESIS;
    break;
  case ')':
    result.token = TOKEN_RIGHT_PARENTHESIS;
    break;
  case '{':
    result.token = TOKEN_LEFT_BRACE;
    break;
  case '}':
    result.token = TOKEN_RIGHT_BRACE;
    break;
  case ';':
    result.token = TOKEN_SEMICOLON;
    break;
  case '"':
    result.token = TOKEN_STRING_LITERAL;
    len = 1;
    do {
      next(s);
      len++;
    } while (s->current_char != '"');
    result.literal = malloc(sizeof(char *) * (len + 1));
    result.literal[len] = '\0';
    memcpy(result.literal, s->src + result.pos.offset, len);
    break;
  case '\'':
    result.token = TOKEN_CHAR_LITERAL;
    len = 1;
    do {
      next(s);
      len++;
    } while (s->current_char != '\'');
    result.literal = malloc(sizeof(char *) * (len + 1));
    result.literal[len] = '\0';
    memcpy(result.literal, s->src + result.pos.offset, len);
    break;
    break;
  default: // If nothing matched we didn't consume the char yet
    consumed = 0;
  }
  if (consumed) {
    debug_printf("Found: %s\n", token_to_string(result.token));
    goto end;
  }

  regmatch_t match;
  int matched =
      regexec(&regex_identifier, s->src + s->current_offset, 1, &match, 0);
  if (matched == 0) {
    sizet matchlen = match.rm_eo - match.rm_so;

    // printf("Identifier mathed! Len: %zu\n", matchlen);

    enum Token maybeKeyword = isKeyword(s->src + s->current_offset, matchlen);
    debug_printf("Is keyword? %s\n", token_to_string(maybeKeyword));
    if (maybeKeyword != TOKEN_NONE) {
      // Was a keyword
      result.token = maybeKeyword;
    }
    for (int i = 0; i < matchlen - 1; i++) {
      next(s);
    }
  } else if (0 == (regexec(&regex_constant, s->src + s->current_offset, 1,
                           &match, 0))) {
    result.token = TOKEN_INT_LITERAL;
  } else {

    debug_printf("No regex mathed!\n");
  }

end:
  next(s);
  trace_descend();
  return result;
}

void ScannerFree(struct Scanner *s) {
  if (!s) {
    trace_printf_ascend("ScannerFree(s) -> s is null!\n");
    trace_descend();
  }
  trace_printf_ascend("ScannerFree(s)\n");
  free(s->src);
  free(s);
  trace_descend();
}

struct filecontent read_file(char *filename) {
  trace_printf_ascend("read_file(\"%s\")\n", filename);
  struct filecontent result = {0, NULL};
  FILE *file = fopen(filename, "rb");

  if (file == NULL) {
    debug_fprintf(stderr, "Error opening file\n");
    trace_descend();
    return result;
  }

  if (fseek(file, 0, SEEK_END) != 0) {
    debug_fprintf(stderr, "Error seeking to the end of the file\n");
    goto end;
  }

  long file_size = ftell(file);
  if (file_size == -1) {
    debug_fprintf(stderr, "Error getting file size\n");
    goto end;
  }

  rewind(file);

  result.data = (char *)malloc(file_size + 1);
  if (result.data == NULL) {
    debug_fprintf(stderr, "Memory allocation error\n");
    goto end;
  }

  sizet bytes_read = fread(result.data, 1, file_size, file);
  if (bytes_read != file_size) {
    debug_fprintf(stderr, "Error reading the file\n");
    free(result.data);
    goto end;
  }

  result.data[bytes_read] = '\0';

  result.len = bytes_read + 1;

end:
  fclose(file);
  trace_descend();
  return result;
}

void debug_scanner(struct Scanner *s) {
  if (!s) {
    trace_printf_ascend("debug_scanner(s) -> s is null!\n");
    trace_descend();
    return;
  }
  trace_printf_ascend("debug_scanner(\"%s\")\n", s->filename);
  debug_printf("struct Scanner {\n");
  debug_printf("\t filename: %s\n", s->filename);

  if (s->src) {
    debug_printf("\t src: points to a string with %lu chars\n", strlen(s->src));
  } else {
    debug_printf("\t src: is NULL\n");
  }

  debug_printf("\t current_offset: %zu\n", s->current_offset);
  debug_printf("\t current_lineoffset: %zu\n", s->current_lineoffset);
  debug_printf("\t current_linecharoffset: %zu\n", s->current_linecharoffset);
  debug_printf("\t current_char: %c\n", s->current_char);
  debug_printf("\t src_len: %zu\n", s->src_len);
  debug_printf("}\n");
  trace_descend();
}

void compile_regexes() {
  if (regexes_compiled) {
    trace_printf_ascend("compile_regexes() -> skipping, already compiled\n");
    trace_descend();
    return;
  }
  trace_printf_ascend("compile_regexes()\n");
  const char *identifier = "[a-zA-Z_][a-zA-Z0-9_]*\\b";
  const char *constant = "[0-9]+\\b";
  int ret;
  ret = regcomp(&regex_identifier, identifier, REG_EXTENDED);
  ret = regcomp(&regex_constant, constant, REG_EXTENDED);
  regexes_compiled = 1;
  trace_descend();
}
