#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "common/common.h"
#include "lex/pl0_lex.h"
//#include "error/pl0_error.h"

void show_help(const char * arg0)
{
	printf("Usage: %s <src> <output>\n\n", arg0);
}

int main(int argc, char * argv[])
{
	/* Parse arguments */
	if (argc != 3)
	{
		show_help(argv[0]);
		exit(-1);
	}
	fin = fopen(argv[1], "rb");
	fout = fopen(argv[2], "wb");
	if (!fin)
	{
		printf("Fatal: Cannot open file for reading: %s\n", argv[1]);
		exit(-1);
	}

	/* Start */
	printf("=== This is the program for PL/0 testing ===\n");

	/* Setup */
	compiler = PL0Compiler_create();

	/* Lexical Analysis */
	printf("--- Lexical Analysis testing ---\n");
	PL0Lex * lex = compiler->lex;
	/**
	while (PL0Lex_get_token(lex))
	{
		assert(lex->last_token_type != TOKEN_NULL);
		assert(lex->last_token_type != TOKEN_RESWORDS);
		assert(lex->last_token_type != TOKEN_SYMBOL);
		if (lex->last_token_type == TOKEN_IDENTIFIER)
		{
			printf("Identifier:\t%-10s  %d: %d-%d\n", lex->last_id,lex->row,lex->bposition,lex->eposition);
		}
		else if (lex->last_token_type == TOKEN_NUMBER)
		{
			printf("Number:\t\t%-10d  %d: %d-%d\n", lex->last_num,lex->row,lex->bposition,lex->eposition);
		}
		else if (lex->last_token_type > TOKEN_RESWORDS)
		{
			printf("Reserved word:\t%-10s  %d: %d-%d\n", TOKEN_RESERVED_WORDS[lex->last_token_type - TOKEN_RESWORDS - 1],lex->row,lex->bposition,lex->eposition);
		}
		else if((lex->last_token_type > TOKEN_SYMBOL)&&(lex->last_token_type < TOKEN_RESWORDS))
		{
			printf("Symbol:\t\t%-10s  %d: %d-%d\n", TOKEN_SYMBOLS[lex->last_token_type - TOKEN_SYMBOL - 1],lex->row,lex->bposition,lex->eposition);
		}
	}

	printf("--- Lexical Analysis testing end. ---\n");/**/

	/*test syntax ayalysis*/
/**/
	PL0Lex_get_token(lex);
	program(lex);
	/**/

	/* Cleanup */
	fclose(fin);
	fclose(fout);
	/* Finish */
	printf("=== Normal end for testing. ===\n");
	return 0;
}
