#include <stdio.h>
#include <string.h>

#include "sym_tab.h"
#include "error.h"
#include "global.h"

#define MAX_COMMANDS 200
#define MEM_SIZE 500

#define BUFSZ 50

static struct {
  char tok[BUFSZ];
  char inst[BUFSZ];
  char op[BUFSZ];
  int row;
} sym_tab[MAX_COMMANDS];
static int sym_tab_size = -1;

typedef enum {
  CONST = 0,
  GET = 1, PUT = 2,
  LD = 3, ST = 4,
  ADD = 5, SUB = 6,
  JPOS = 7, JZ = 8,
  J = 9, HALT = 10
} TOKEN;

static struct {
  char* name;
  int opcode;
} op_tab[] = {
  {"const", CONST}, {"get", GET}, {"put", PUT}, {"ld", LD}, {"st", ST},
  {"add", ADD}, {"sub", SUB}, {"jpos", JPOS}, {"jz", JZ},
  {"j", J}, {"halt", HALT },
};
#define CMDNUM (sizeof(op_tab) / sizeof(op_tab[0]))

static long memory[MEM_SIZE];

void st_put_row(const char* tok, const char* inst, const char* op, int row) {
  if (++sym_tab_size >= MAX_COMMANDS)
    err(SYMTAB, TOO_MANY_COMMANDS, row, "Aborted");
  strcpy(sym_tab[sym_tab_size].tok, tok);
  strcpy(sym_tab[sym_tab_size].inst, inst);
  strcpy(sym_tab[sym_tab_size].op, op);
  sym_tab[sym_tab_size].row = row;
}

int st_size(void) {
  return sym_tab_size;
}

void st_get_row(int idx, char* tok, char* inst, char* op, int* row) {
  if (idx > sym_tab_size || idx < 0)
    err(SYMTAB, INVALID_ACCESS, idx, "Internal error");
  strcpy(tok, sym_tab[idx].tok);
  strcpy(inst, sym_tab[idx].inst);
  strcpy(op, sym_tab[idx].op);
  *row = sym_tab[idx].row;
}

int st_lookup_tok(const char* tok) {
  for (int i = 0; i <= sym_tab_size; ++i)
    if (EQ(tok, sym_tab[i].tok))
      return i;
  return -1;
}

int st_lookup_inst(const char* inst) {
  for (int i = 0; i < CMDNUM; ++i)
    if (EQ(inst, op_tab[i].name))
      return op_tab[i].opcode;
  return -1;
}

void st_store_code(int addr, long code) {
  memory[addr] = code;
}

// It's an assembler _and_ a VM!
static long ax;
void st_exec_cmd(int* pc) {
  long machine_word = memory[*pc];
  int addr = machine_word % 1000;
  TOKEN code = machine_word / 1000;
  (*pc)++;
  switch (code) {
    case GET: scanf("%ld", &ax); break;
    case PUT: printf("= %ld\n", ax); break;
    case ST: memory[addr] = ax; break;
    case LD: ax = memory[addr]; break;
    case ADD: ax += memory[addr]; break;
    case SUB: ax -= memory[addr]; break;
    case JPOS: if (ax > 0) *pc = addr; break;
    case JZ: if (ax == 0) *pc = addr; break;
    case J: *pc = addr; break;
    case HALT:
    default: *pc = -1; break;
  }
}
