#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "pass2.h"
#include "error.h"
#include "sym_tab.h"

#define BUFSZ 50

static char toknam[BUFSZ],
            inst[BUFSZ],
            op[BUFSZ];

void pass2_convert_to_asm(const char* listing_name) {
  FILE* fd;
  int row;

  if (listing_name)
    if (!(fd = fopen(listing_name, "w")))
      err(PASS2, INVALID_LISTNAM, 0, listing_name);

  int sym_tab_size = st_size();
  for (int i = 0; i <= sym_tab_size; i++) {
    int addr_val = 0;
    st_get_row(i, toknam, inst, op, &row);
    if (*op) {
      if (isalpha(*op)) {
        if ((addr_val = st_lookup_tok(op)) == -1)
          err(PASS2, MISSING_TOK, row, op);
      } else {
        addr_val = atoi(op);
      }
    }
    int opcode = st_lookup_inst(inst);
    long code = 1000*opcode + addr_val;
    st_store_code(i, code);
    if (listing_name)
      fprintf(fd, "%5d: %05ld %15s %15s %15s\n",
          i, code, toknam, inst, op);
  }

  if (listing_name)
    fclose(fd);
}
