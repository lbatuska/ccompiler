#include "container_of.h"
#include "list.h"
#include "scanner.h"
#include "token.h"
#include "vector.h"
#include <debug.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <trace.h>

enum GoalStage {
  Stage_None,
  Stage_Lexer,
  Stage_Parser,
  Stage_Codegen,
  Stage_Assembly,
  Stage_Full,
};

struct args_result {
  enum GoalStage stage;
  int *file_args;
  int file_args_len;
};

struct args_result parse_args(int argc, char *argv[]) {
  struct args_result res = {
      .stage = Stage_Full, .file_args_len = 0, .file_args = NULL};
  int skip_verbose_iter = -1;
  int skip_trace_iter = -1;
  for (int i = 1; i < argc; i++) {

    if ((strcmp(argv[i], "-v") == 0) || (strcmp(argv[i], "--verbose") == 0)) {
      DEBUG = 1; // Either works it's an extern int
      // debug_set(1);
      debug_printf("Verbose flag is present!\n");
      skip_verbose_iter = i;
    } else if (strcmp(argv[i], "--no-trace") == 0 ||
               strcmp(argv[i], "--trace-off") == 0) {
      trace_set(0);
      skip_trace_iter = i;
    }
  }
#if TRACE
  if (!TRACE_RUNTIME) {
    debug_printf("Tracing is off!\n");
  }
#endif

  trace_printf_ascend("parse_args\n");
  int stage_flag_count = 0;
  enum GoalStage result = Stage_Full;
  struct stat unused_stat;
  for (int i = 1; i < argc; i++) {
    // At first check if it's a flag at all (aka starts with '-')
    if (i == skip_verbose_iter || i == skip_trace_iter)
      continue;
    if (argv[i][0] == '-') {
      if (strcmp(argv[i], "--lex") == 0) {
        stage_flag_count++;
        result = Stage_Lexer;
        debug_printf("--lex flag is present!\n");
      }
      if (strcmp(argv[i], "--parse") == 0) {
        stage_flag_count++;
        result = Stage_Parser;
        debug_printf("--parse flag is present!\n");
      }
      if (strcmp(argv[i], "--codegen") == 0) {
        stage_flag_count++;
        result = Stage_Codegen;
        debug_printf("--codegen flag is present!\n");
      }
      if (strcmp(argv[i], "-S") == 0) {
        stage_flag_count++;
        result = Stage_Assembly;
        debug_printf("-S flag is present!\n");
      }
    } else if (stat(argv[i], &unused_stat) == -0) {
      debug_printf("Arg: %s is a file!\n", argv[i]);
      res.file_args_len++;
      res.file_args = realloc(res.file_args, sizeof(int) * res.file_args_len);
      res.file_args[res.file_args_len - 1] = i;

    } else {
      debug_printf("Unknow argument: %s\n", argv[i]);
    }
  }
  if (stage_flag_count > 1) {
    debug_fprintf(stderr,
                  "Error flags '--lex', '--parse', '--codegen', '-S' cannot "
                  "be combined.\n");
    res.stage = Stage_None;

    trace_descend();
    return res;
  }
  trace_descend();
  return res;
};

struct ScannerList {
  struct list_head l;
  struct Scanner *s;
};

int main(int argc, char *argv[]) {

  debug_printf("Starting Compiler!\n");
  struct args_result arg_res = parse_args(argc, argv);

  VECTOR_INIT(token_vec, 1, struct ScannerResult);

  LIST_HEAD_INIT(scanner_list);

  for (int i = 0; i < arg_res.file_args_len; i++) {
    debug_printf("Processing file arg %i\n", arg_res.file_args[i]);

    struct ScannerList *sl = malloc(sizeof(struct ScannerList));
    sl->s = ScannerInit(argv[arg_res.file_args[i]]);
    list_add(&scanner_list, &sl->l);

    debug_scanner(sl->s);

    struct ScannerResult sr;

    do {
      sr = ScannerScan(sl->s);
      vector_push_back(token_vec, sr);
    } while (sr.token != TOKEN_EOF);

    // ScannerFree(s1);

    struct ScannerResult *pos;
    vector_for_each(pos, token_vec) {

      trace_ascend();
      debug_printf("Token: %s at line: %zu char %zu abs pos: %zu \n",
                   token_to_string(pos->token), pos->pos.lineoffset,
                   pos->pos.linecharoffset, pos->pos.offset);

      trace_descend();
    }
  }

  // Cleanup
  struct ScannerResult *pos;
  vector_for_each(pos, token_vec) {
    if (pos->literal) {
      free(pos->literal);
    }
  }
  pos = NULL;
  free(token_vec.data);

  struct list_head *posl;
  list_for_each(posl, &scanner_list) {
    trace_ascend();
    ScannerFree(container_of(posl, struct ScannerList, l)->s);
    free(pos);
    trace_descend();
  }

  if (arg_res.stage == Stage_None) {

    debug_fprintf(stderr,
                  "Error determining stages that need to be invoked!\n");
    return 1;
  }

  return 0;
}
