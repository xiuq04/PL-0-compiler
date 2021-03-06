#ifndef __PL0_ERROR_H__
#define __PL0_ERROR_H__
#include <stdio.h>

//Possible errors in lex
#define ERR_UNRECOGNIZED_CHAR "An unrecognized character appears."
#define ERR_WRONG_IDEN_FORMAT "Wrong identifier format: identifier cannot start with a number."
#define ERR_MISS_END_FOR_COMMENT "Missing a end for comment: has reached the EOF."
#define ERR_TOO_LONG_IDEN_OR_NUM "Too long identifier or number."
#define ERR_COLON_EXPECT_EQUAL "Expect an '=' after ':'"
#define ERR_AND_EXPECT_AND "Expect an '&' after '&'"
#define ERR_OR_EXPECT_OR "Expect an '|' after '|'"

//Possible errors in syntax
#define ERR_SEMICOLON_MISS "Missing ';'."
#define ERR_DOT_MISS "Missing '.'."
#define ERR_COMMA_MISS "Missing ','."
#define ERR_RIGHT_PARENTHESIS_MISS "Right parenthesis Missing')'."
#define ERR_INDEX_MISS "The index of array is Missing."
#define ERR_RIGHT_MRPAREN_MISS "Right parenthesis Missing']'."
#define ERR_RIGHT_GRPAREN_MISS "Right parenthesis Missing'}'."
#define ERR_SEMICOLON_OR_COMMA_MISS "Missing ';' or ','."
#define ERR_CONST_EXPECT_IDEN "Expect an identifier after const."
#define ERR_VAR_EXPECT_IDEN "Expect an identifier after var."
#define ERR_PROC_EXPECT_IDEN "Expect an identifier after procedure."
#define ERR_CALL_EXPECT_IDEN "Expect an identifier after call."
#define ERR_COMMA_EXPECT_IDEN "Expect an identifier after ','."
#define ERR_IDEN_EXPECT_EQUAL "Expect an '=' after identifier."
#define ERR_ASSIGN_EXPECT_IDEN "Expect an ':=' after identifier."
#define ERR_ILLEGAL_ASSIGNMENT "Only a number can be assigned to '='"
#define ERR_MUNI_EXPECT_NUMBER "Expect a number after '-'."
#define ERR_NEED_END_FOR_BEGIN "'begin' needs to end with 'end'."
#define ERR_NEED_THEN_FOR_IF "'Then' needs to end with 'if'."
#define ERR_NEED_THEN_FOR_ELIF "'Then' needs to end with 'elif'."
#define ERR_NEED_DO_FOR_WHILE "Need a 'do' for the loop statement after the condition."
#define ERR_INDEX_OF_ARRAY "The index of array  cannot be less than zero"
#define ERR_WRONG_OP_IN_BINARY_COND "Wrong op in a binary conditional expression: could only use '=', '!=', '<', '>', '<=', '>='."
#define ERR_WRONG_BEGIN_OF_PROC "Wrong beginning of procedure: could only use 'const', 'var', 'procdure', 'ident', 'call', 'begin', 'if', 'while'."
#define ERR_WRONG_BEGIN_OF_STATEMENT "Wrong beginning of statement: could only use 'ident', 'call', 'begin', 'if', 'while'."
#define ERR_WRONG_BEGIN_OF_COND "Wrong beginning of condition: could only use 'odd', 'not', '{', 'iden', 'number', '-', '('."
#define ERR_WRONG_BEGIN_OF_FACTOR "Wrong beginning of factor: could only use 'iden', 'number', '-', '('."
#define ERR_UD_IDEN "Undeclared identifier."
#define ERR_UC_IDEN "The identifier for the index mast be a const."
#define ERR_UNWANT_LEX "There is an unwanted lex."
#define ERR_IDEN_OF_ARRAY "The identifier is not an array"
#define ERR_IDEN_OF_VAR "The identifier is not a var"
#define ERR_IDEN_OF_PRO "The identifier is not a procedure"

#define ERR_IDEN_OF_VAR_OR_CONST "The identifier is not a var or const"

void PL0_print_error(char * err_msg, char* cur_line_content, int cur_line_num, int offset);

#endif
