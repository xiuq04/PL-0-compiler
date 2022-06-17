#ifndef __PL0_TAX_H__
#define __PL0_TAX_H__

#define TXMAX				500 		// max length of token table
#define MAXADDRESS 	32767		// max address
#define MAX_ID_LEN	10	/* Maximum length of an identifier */

/*variables for token table*/
int table_index = 0;
int offsetforst = 2;
/*struct for token table*/
typedef struct {
	char name[MAX_ID_LEN + 1];
	int kind;
	int value;
	int bound1;//belong to array
	int bound2;
	short level;
	short address;
	char owner[MAX_ID_LEN + 1];
} Table;

/*definition for token table*/
Table token_table[TXMAX];

struct Stack{
	int top;
	int bottom;
	char element[100][20];
};

struct Stack stack;
int level = 0;
/*identifier type*/
enum idtype {
	ID_CONSTANT,
	ID_ARRAY1,
	ID_ARRAY2,
	ID_VARIABLE,
	ID_PROCEDURE
};
struct addrlist{
	int line;
	struct addrlist* next;
};

/*push and pop operation on the stack*/
void initStack();
char * pop();
int push(char* element);
char * getTop();
void printStack();

/*operations for token table*/
void table_append(PL0Lex * lex, int kind);

/*functions for syntax analysis*/
void statement(PL0Lex * lex); //analysis the statement
void condition(PL0Lex * lex); //analysis the condition statement
void expression(PL0Lex * lex); //analysis the expression
void term(PL0Lex * lex); //analysis the term
void factor(PL0Lex * lex); //analysis the factor
void program_block(PL0Lex * lex); //analysis the program block
void program(PL0Lex * lex);
void initposses();
#endif //pl0_tax.h
