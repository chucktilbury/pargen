/*
 * This is the public interface to a simple scanner for the grammar syntax.
 */
#ifndef _SCAN_H_
#define _SCAN_H_

char* get_file_name();
int get_line_no();
int get_col_no();
int open_file(const char* fname);

/*
 * Defined by flex. Call one time to isolate a symbol and then use the global
 * symbol struct to access the symbol.
 */
extern int yylex();
extern int yyparse();
extern FILE* yyin;

// void yyerror(char *s, ...);
void yyerror(const char* s);

#endif
