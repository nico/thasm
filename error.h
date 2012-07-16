#define ASSEMB 0
#define SYMTAB 1
#define PASS1 2
#define PASS2 3

// ASSEMB
#define USAGE 0
#define ILLEGAL 1

// SYMTAB
#define TOO_MANY_COMMANDS 0
#define INVALID_ACCESS 1

// PASS1
#define INVALID_FILE 0
#define INVALID_INST 1
#define INVALID_INPUT 2

// PASS2
#define MISSING_TOK 0
#define INVALID_LISTNAM 1

extern void err(int module, int errno, int row, const char* msg);
