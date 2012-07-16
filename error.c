#include <stdio.h>
#include <stdlib.h>
#include "error.h"

static char* asm_err[] = {
  "Line %d: usage: %s [-l] file\n",
  "Index %d: Invalid access to symbol table (%s)\n",
  ""
};

static char* sym_tab_err[] = {
  "Line %d: too many commands ('%s')\n"
  "Index %d: Invalid access to symbol table (%s)\n",
  ""
};

static char* pass1_err[] = {
  "Line %d: unable to open file '%s'\n",
  "Line %d: invalid instruction '%s'\n",
  "Line %d: invalid input '%s'\n",
  "Line %d: invalid character '%s'\n",
  ""
};

static char* pass2_err[] = {
  "Line %d: token '%s' not found\n",
  "Line %d: unable to open file '%s' for writing\n",
  ""
};

static char** errmsg[] = {
  asm_err,
  sym_tab_err,
  pass1_err,
  pass2_err,
  NULL
};

void err(int module, int errno, int row, const char* msg) {
  fprintf(stderr, errmsg[module][errno], row, msg);
  exit(1);
}
