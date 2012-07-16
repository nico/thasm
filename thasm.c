#include <stdio.h>

#include "global.h"
#include "error.h"
#include "pass1.h"
#include "pass2.h"
#include "sym_tab.h"

int main(int argc, char* argv[]) {
  char* progname = argv[0];
  char* listname = NULL;
  char* file = NULL;
  char buf[50];
  bool print_listing;

  if (argv[1] && EQ(argv[1], "-l")) {
    strcpy(buf, "l");
    strcat(buf, argv[2]);
    listname = buf;
    print_listing = true;
    file = argv[3];
  } else {
    file = argv[1];
    print_listing = false;
  }

  if (argc == 1 || (print_listing && argc == 2))
    err(ASSEMB, USAGE, 0, progname);

  pass1_scan(file);
  pass2_convert_to_asm(listname);

  if (print_listing)
    fprintf(stderr, "Wrote listing to '%s'\n", listname);

  fprintf(stderr, "Simulating program:\n");
  int pc = 0;
  while (pc >= 0)
    st_exec_cmd(&pc);
  fprintf(stderr, "done.\n");
}
