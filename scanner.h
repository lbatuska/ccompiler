#ifndef _SCANNER_H
#define _SCANNER_H

#include "token.h"
#include <stdio.h>

struct ScannerResult {
  // int pos;
  struct TokenPosition pos;
  enum Token token;
  char *literal;
};

struct Scanner;

// Scanner takes ownership of file and src
struct Scanner *ScannerInit(char *filename);
struct ScannerResult ScannerScan(struct Scanner *s);

char *ScannerGetFileName(struct Scanner *s);

void debug_scanner(struct Scanner *s);
#endif // _SCANNER_H
