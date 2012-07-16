extern void st_put_row(
    const char* tok, const char* inst, const char* op, int row);

extern int st_size(void);

extern void st_get_row(int idx, char* tok, char* inst, char* op, int* row);

extern int st_lookup_tok(const char* tok);
extern int st_lookup_inst(const char* inst);

extern void st_store_code(int addr, long code);

extern void st_exec_cmd(int* pc);
