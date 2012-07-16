#include <stdio.h>
#include <ctype.h>

#include "pass1.h"
#include "error.h"
#include "global.h"
#include "sym_tab.h"

typedef enum { tok, inst, op, end, comment } mode_type;

#define BUFSZ 50

static char toknam[BUFSZ],
            instnam[BUFSZ],
            opnam[BUFSZ],
            word[BUFSZ];
static FILE* fd;

static void rdword(char* buf, char c) {
  while (isalnum(c)) {
    *buf++ = tolower(c);
    c = fgetc(fd);
  }
  *buf = '\0';
  ungetc(c, fd);
}

void pass1_scan(const char* filename) {
  mode_type mode = tok;
  char c;
  bool has_row = false;
  int row = 1;

  if (!(fd = fopen(filename, "r")))
    err(PASS1, INVALID_FILE, 0, filename);
  for (;;) {
    c = fgetc(fd);
    if (c == '\n') {
      mode = tok;
      if (has_row)
        st_put_row(toknam, instnam, opnam, row);
      has_row = false;
      ++row;
      toknam[0] = instnam[0] = opnam[0] = word[0] = '\0';
    } else if (c == EOF) {
      break;
    } else if (c == '#') {
      mode = comment;
    } else if (mode == comment) {
      ;
    } else if (isspace(c)) {
      if (mode == tok) mode = inst;
      else if (mode == inst) mode = op;
      else if (mode == op) mode = end;
      while(isspace(c = fgetc(fd)))
        ;
      ungetc(c, fd);
    } else if (isalnum(c)) {
      has_row = true;
      switch(mode) {
        case tok:
          rdword(toknam, c);
          break;
        case inst:
          rdword(instnam, c);
          if (st_lookup_inst(instnam) == -1)
            err(PASS1, INVALID_INST, row, instnam);
          break;
        case op:
          rdword(opnam, c);
          break;
        case end:
          rdword(word, c);
          break;
        case comment:
          // Can't happen, intercepted above
          break;
      }
    } else {
      word[0] = c; word[1] = '\0';
      err(PASS1, INVALID_INPUT, row, word);
    }
  }
  fclose(fd);
}
