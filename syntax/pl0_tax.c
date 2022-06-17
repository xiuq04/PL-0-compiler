#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../common/common.h"
#include "../lex/pl0_lex.h"
#include "../error/pl0_error.h"
#include "pl0_tax.h"
#include "../ins/ins.h"
#define  ARRAY_MIN  1
#define isSLASH 0
#define isTIMES 1
#define isPLUS  1
#define isMINUS 0
#define isAND   1
#define isOR    0
#define isEQU   0
#define isNEQ   1
#define isLES   2
#define isLEQ   3
#define isGTR   4
#define isGEQ   5

int cur_row;
int cur_position;
char last_line_content[100];
char posses[MAX_ID_LEN + 1];
PL0TokenType last_token_type;


void record(PL0Lex * lex){
	cur_row=lex->row;
	cur_position=lex->eposition;
        Get_last_line_content(last_line_content);
      //  printf("%d:%s\n",lex->last_token_type,last_line_content);
	last_token_type=lex->last_token_type;
}
/*operation pop and push on stack*/
void initposses()
{
        int i = 0;
        for(; i <= MAX_ID_LEN; i ++)
                posses[i] = '\0';
}
void initStack(){
	stack.top = 0;
	stack.bottom = 0;
}
char * pop() {
	int len = strlen(stack.element[stack.top]);
	if(len==0&&stack.top==0)
		return NULL;
	int i;
	char* pointer = (char*)malloc((len+1)*sizeof(char));
	for(i=0;i<len;i++){
		pointer[i] = stack.element[stack.top][i];
		stack.element[stack.top][i] = '\0';
	}
        pointer[len]='\0';
	stack.top--;
	return pointer;
}//pop
char * getTop() {
	int len = strlen(stack.element[stack.top]);
	if(len==0&&stack.top==0)
		return NULL;
	int i;
	char* pointer=(char*)malloc((len+1)*sizeof(char));;
	for(i=0;i<len;i++){
		pointer[i] = stack.element[stack.top][i];
	}
        pointer[len]='\0';
	return pointer;
}//getTop
int push(char* elements) {
	stack.top++;
	int i;
	int len = strlen(elements);
	for(i=0;i<len;i++)
		stack.element[stack.top][i] = elements[i];
        stack.element[stack.top][i]='\0';
	return len;
}//push
void printStack(){
	int i;
	for(i=stack.bottom;i<=stack.top;i++){
		printf("%s ",stack.element[i]);
	}
        printf("\n");
}
/*operations for token table*/
void table_append(PL0Lex * lex, int kind) {
	strcpy(token_table[table_index].name, lex->last_id);
	token_table[table_index].kind = kind;
        token_table[table_index].address = offsetforst;
        strcpy(token_table[table_index].owner, posses); 
	switch (kind) {
		case ID_CONSTANT:
			token_table[table_index].value = 0;
                        token_table[table_index].level = level;
                        offsetforst += 1;
			break;
                case ID_ARRAY1:
                        token_table[table_index].level = level;
                        token_table[table_index].bound1 = ARRAY_MIN;
                        token_table[table_index].bound2 = ARRAY_MIN;
                        break;
		case ID_VARIABLE:
			token_table[table_index].level = level;
                        offsetforst += 1;
			break;
		case ID_PROCEDURE:
			token_table[table_index].level = level - 1;
                        offsetforst += 2;
			break;
	} //switch
	table_index += 1;
} //table_append

int table_search(char * str){
	int i = -1;
	for(i = 0; i < table_index; i ++)
	{
		if(strcmp(token_table[i].name,str) == 0 && strcmp(token_table[i].owner,posses) == 0)
		{
			return i;
		}
	}
        for(i = 0; i < table_index; i ++)
                if(strcmp(token_table[i].name, str) == 0 && strcmp(token_table[i].owner,"main") == 0)
                        return i;
	return -1;
}

/*functions for syntax analysis*/
void statement_sequence(PL0Lex * lex) {
        printf("analysis the statement sequence\n");
        pop();
        push("O");
        push("SEMICOLON");
        push("P");
        printStack();
        statement(lex);
        while(lex->last_token_type!=TOKEN_EOF){
                if(lex->last_token_type == TOKEN_SEMICOLON){
                        record(lex);
                        PL0Lex_get_token(lex);
                }
                else{
                        if(lex->row == cur_row){
				Get_line_content();
			  	Print_Error1(ERR_SEMICOLON_MISS);
			}
			else
			PL0_print_error(ERR_SEMICOLON_MISS,last_line_content,cur_row,cur_position);
                }
                pop();
                printStack();
                if(lex->last_token_type == TOKEN_END || lex->last_token_type == TOKEN_PERIOD ||lex->last_token_type==TOKEN_EOF)
                       break;
                push("SEMICOLON");
                push("p");
                printStack();
                statement(lex);
        }
	char *a=getTop();
	if(strcmp(a,"SEMICOLON")==0){
		pop();
		printStack();
	}
        printf("end of statement sequence\n");
}

void analyseR(PL0Lex * lex){//used to analyse else elif
	if(lex->last_token_type == TOKEN_ELIF){
                pop();
		push("R");
		push("P");
		push("THEN");
		push("W");
		push("ELIF");
		printStack();
		pop();
		printStack();
                record(lex);
		PL0Lex_get_token(lex);
		condition(lex);
                logicNot();
                int jumpplace = ftell(fout);
                blankforjmp();
		if(lex->last_token_type != TOKEN_THEN){
			if(lex->row == cur_row){
			        Get_line_content();
			        Print_Error1(ERR_NEED_THEN_FOR_ELIF);
		        }
		        else
			        PL0_print_error(ERR_NEED_THEN_FOR_ELIF,last_line_content,cur_row,cur_position);
		}
                else{
                        record(lex);
		        PL0Lex_get_token(lex);
                }
	        pop();//pop then
		printStack();
		statement(lex);//analyse the P
                fseek(fout, jumpplace - ftell(fout), 1);
                jpc(insline);
                insline --;
                fseek(fout, 0, 2);
		analyseR(lex);
	}//if
	else if(lex->last_token_type == TOKEN_ELSE){
                pop();
		push("P");
		push("ELSE");
	        printStack();
		pop();
		printStack();
                record(lex);
		PL0Lex_get_token(lex);
		statement(lex);
	}
        else {
                pop();
		printStack();
        }
}
void statement(PL0Lex * lex) {
	printf("analysis the statement\n");
        if (lex->last_token_type == TOKEN_IDENTIFIER || lex->last_token_type == TOKEN_CALL || lex->last_token_type == TOKEN_BEGIN || lex->last_token_type == TOKEN_IF || lex->last_token_type == TOKEN_WHILE || lex->last_token_type == TOKEN_PRINT || lex->last_token_type == TOKEN_RETURN) {
		if(lex->last_token_type == TOKEN_IDENTIFIER){
                        pop();
                        push("D");
                        push(":=");
                        push("T");
                        push("ident");
                        printStack();
                        int i;
                        i = table_search(lex->last_id);
                        if(i == -1){
                                Get_line_content();
                                Print_Error1(ERR_UD_IDEN);
                        }
                        pop();//match ident
                        printStack();
                        record(lex);
                        put(token_table[i].address);
                        PL0Lex_get_token(lex);
                        if(lex->last_token_type == TOKEN_LEFT){//the factor must be a array element
                                if(i!=-1&&token_table[i].kind != ID_ARRAY1&&token_table[i].kind != ID_ARRAY2){
                                        Get_line_content();
                                        Print_Error1(ERR_IDEN_OF_ARRAY);
                                }
                                pop();//T -> T [ INDEX ]
                                push("T");
                                push("]");
                                push("INDEX");
                                push("[");
                                printStack();
                                pop();//match [
                                printStack();
                                pop();
                                push("D");
                                printStack();
                                record(lex);
                                PL0Lex_get_token(lex);
                                expression(lex);

                                put(token_table[i].bound2);
                                mul();

                                if(lex->last_token_type == TOKEN_RIGHT)
                                {
                                        pop();
                                        printStack();
                                        record(lex);
                                        PL0Lex_get_token(lex);
                                }
                                else{
                                        if(lex->row == cur_row){
			                        Get_line_content();
			                        Print_Error1(ERR_RIGHT_MRPAREN_MISS);
		                        }
		                        else
			                        PL0_print_error(ERR_RIGHT_MRPAREN_MISS,last_line_content,cur_row,cur_position);
                                        pop();//match ]
                                        printStack();
                                }
                                if(lex->last_token_type == TOKEN_LEFT){//the factor must be a array element
                                        if(i!=-1&&token_table[i].kind != ID_ARRAY2){
                                                Get_line_content();
                                                Print_Error1(ERR_IDEN_OF_ARRAY);
                                        }
                                        pop();//T -> [ INDEX ]
                                        push("T");
                                        push("]");
                                        push("INDEX");
                                        push("[");
                                        printStack();
                                        pop();//match [
                                        printStack();
                                        pop();
                                        push("D");
                                        printStack();
                                        record(lex);
                                        PL0Lex_get_token(lex);
                                        expression(lex);

                                        add();
                                        

                                        if(lex->last_token_type == TOKEN_RIGHT){
                                                pop();
                                                printStack();
                                                record(lex);
                                                PL0Lex_get_token(lex);
                                        }
                                        else{
                                                if(lex->row == cur_row){
			                                Get_line_content();
			                                Print_Error1(ERR_RIGHT_MRPAREN_MISS);
		                                }
		                                else
			                                PL0_print_error(ERR_RIGHT_MRPAREN_MISS,last_line_content,cur_row,cur_position);
                                                pop();
                                                printStack();
                                        }
                                }
                                else if(i!=-1&&token_table[i].kind == ID_ARRAY2){
                                                Get_line_content();
                                                Print_Error1(ERR_IDEN_OF_ARRAY);
                                }

                                add();

                                pop();//kick T out of stack
                                printStack();
                        }
                        else {
                                if(i!=-1&&token_table[i].kind != ID_VARIABLE){
                                        Get_line_content();
                                        Print_Error1(ERR_IDEN_OF_VAR);
                                }

                                pop();
                                printStack();
                        }
                        if(lex->last_token_type == TOKEN_BECOMES){
                                pop();
                                printStack();
                                record(lex);
                                PL0Lex_get_token(lex);
                                expression(lex);
                        }
                        else{
                                if(lex->row == cur_row){
			                Get_line_content();
			                Print_Error1(ERR_ASSIGN_EXPECT_IDEN);
		                }
		                 else
			                PL0_print_error(ERR_ASSIGN_EXPECT_IDEN,last_line_content,cur_row,cur_position);
                                pop();
                                printStack();
                                expression(lex);
                        }


                        swapTop();
                        st();


                }
                if(lex->last_token_type == TOKEN_CALL){
                        pop();
                        push("ident");
                        push("call");
                        printStack();
                        pop();
                        printStack();
                        record(lex);
                        PL0Lex_get_token(lex);
                        if(lex->last_token_type != TOKEN_IDENTIFIER){
                                if(lex->row == cur_row){
			                Get_line_content();
			                Print_Error1(ERR_CALL_EXPECT_IDEN);
		                }
		                else
			                PL0_print_error(ERR_CALL_EXPECT_IDEN,last_line_content,cur_row,cur_position);
                        	}
                        else{
                                int i = table_search(lex->last_id);
                                if(i==-1||token_table[i].kind!=ID_PROCEDURE){
                                        Get_line_content();
			                Print_Error1(ERR_UD_IDEN);
                                }
                                put(insline + 3);
                                stAddress(token_table[i].address);
                                jmptoaddress(token_table[i].bound1);
                                fprintf(fout, "%c", '\n');
                                record(lex);
                                PL0Lex_get_token(lex);
                        }
                        pop();
                        printStack();
                }//call
		if(lex->last_token_type == TOKEN_PRINT){
                        pop();
			//printf("print here\n");
                        push("H");
                        push("print");
                        printStack();
                        pop();
                        printStack();
                        record(lex);
                        PL0Lex_get_token(lex);
                        factor(lex);
                        INT(0);
                }//print
                if(lex->last_token_type == TOKEN_BEGIN){
                        pop();
                        push("end");
                        push("O");
                        push("begin");
                        printStack();
                        pop();
                        printStack();
                        record(lex);
                        PL0Lex_get_token(lex);
                        statement_sequence(lex);
                        pop();//"O"
                        printStack();
                        if(lex->last_token_type != TOKEN_END){
                                if(lex->row == cur_row){
			                Get_line_content();
			                Print_Error1(ERR_NEED_END_FOR_BEGIN);
		                }
		                else
			                PL0_print_error(ERR_NEED_END_FOR_BEGIN,last_line_content,cur_row,cur_position);
                        }
                        else{
                                record(lex);
                                PL0Lex_get_token(lex);
                        }
                        pop();
                        printStack();
                }
                if(lex->last_token_type == TOKEN_IF){
                        pop();
			push("R");
                        push("P");
                        push("then");
                        push("W");
                        push("if");
                        printStack();
                        pop();
                        printStack();
                        record(lex);
                        PL0Lex_get_token(lex);
                        condition(lex);
                        logicNot();
                        int jumpplace = ftell(fout);
                        blankforjmp();
                        if(lex->last_token_type != TOKEN_THEN){
                                if(lex->row == cur_row){
			                Get_line_content();
			                Print_Error1(ERR_NEED_THEN_FOR_IF);
		                }
		                 else
			                PL0_print_error(ERR_NEED_THEN_FOR_IF,last_line_content,cur_row,cur_position);
                        }
                        else{
                                record(lex);
                                PL0Lex_get_token(lex);
                        }
                        pop();
                        printStack();
                        
                        statement(lex);//analyize the statement P
                        fseek(fout, jumpplace - ftell(fout), 1);
                        jpc(insline);
                        insline --;
                        fseek(fout, 0, 2);
			analyseR(lex);

                        
                }
                if(lex->last_token_type == TOKEN_WHILE){
                        pop();
                        push("P");
                        push("do");
                        push("W");
                        push("while");
                        printStack();
                        pop();
                        printStack();
                        record(lex);
                        PL0Lex_get_token(lex);
                        int back = insline;
                        condition(lex);
                        logicNot();
                        int jumpplace = ftell(fout);
                        blankforjmp();
                        if(lex->last_token_type == TOKEN_DO){
                                pop();
                                printStack();
                                record(lex);
                                PL0Lex_get_token(lex);
                                
                                statement(lex);
                                
                        }
                        else {
                                if(lex->row == cur_row){
			                Get_line_content();
			                Print_Error1(ERR_NEED_DO_FOR_WHILE);
		                }
		                else
			                PL0_print_error(ERR_NEED_DO_FOR_WHILE,last_line_content,cur_row,cur_position);
                                pop();
                                printStack();
                                statement(lex);
                        }
                        jmptoaddress(back);
                        fprintf(fout, "%c", '\n');
                        fseek(fout, jumpplace - ftell(fout), 1);
                        jpc(insline);
                        insline --;
                        fseek(fout, 0, 2);
                }
                if(lex->last_token_type == TOKEN_RETURN){
                        pop();
                        push("H");
                        push("return");
                        printStack();
                        pop();
                        printStack();
                        record(lex);
                        PL0Lex_get_token(lex);
                        int i = table_search(posses);
                        if(i!=-1&&token_table[i].kind != ID_PROCEDURE){
                                Get_line_content();
                                Print_Error1(ERR_IDEN_OF_PRO);
                        }
                        put(token_table[i].address + 1);
                        factor(lex);
                        swapTop();
                        st();
                }//return
        }
        else {
		Get_line_content();
		Print_Error1(ERR_WRONG_BEGIN_OF_STATEMENT);
                if(lex->last_token_type!=TOKEN_EOF&&lex->last_token_type!=TOKEN_SEMICOLON&&lex->last_token_type!=TOKEN_VAR&&lex->last_token_type!=TOKEN_CONST&&lex->last_token_type!=TOKEN_PROCEDURE&&lex->last_token_type!=TOKEN_END&&lex->last_token_type!=TOKEN_PERIOD){
                        record(lex);
                        PL0Lex_get_token(lex);
                        statement(lex);
                }
                else{
                        pop();
                        printStack();
                }
        }
        printf("end of statement\n");
}

void condition2(PL0Lex * lex){//deal with J
        if (lex->last_token_type == TOKEN_IDENTIFIER || lex->last_token_type == TOKEN_NUMBER || lex->last_token_type == TOKEN_MINUS || lex->last_token_type == TOKEN_LPAREN  || lex->last_token_type == TOKEN_GLPAREN || lex->last_token_type == TOKEN_ODD || lex->last_token_type == TOKEN_NOT) {
		if (lex->last_token_type == TOKEN_ODD) {
                        pop();
                        push("D");
                        push("odd");
                        printStack();
                        pop();
                        printStack();
                        record(lex);
                        PL0Lex_get_token(lex);
                        expression(lex);
                }
                else if (lex->last_token_type == TOKEN_NOT){
                        pop();
                        push("J");
                        push("NOT");
                        printStack();
                        pop();
                        printStack();
                        record(lex);
                        PL0Lex_get_token(lex);
                        condition2(lex);
                        logicNot();
                }
                else if (lex->last_token_type == TOKEN_LPAREN){
                        pop();
                        push("RPAREN");
                        push("W");
                        push("LPAREN");
                        printStack();
                        pop();
                        printStack();
                        record(lex);
                        PL0Lex_get_token(lex);
                        condition(lex);
                        if(lex->last_token_type != TOKEN_RPAREN) {
                           if(lex->row == cur_row){
				        Get_line_content();
			  	        Print_Error1(ERR_RIGHT_GRPAREN_MISS);
			        }
			        else
			                PL0_print_error(ERR_RIGHT_GRPAREN_MISS,last_line_content,cur_row,cur_position);
                        }
                        else{
                                record(lex);
                                PL0Lex_get_token(lex);

                        }
                        pop();
                        printStack();
                }
                else {
                        pop();
                        push("D");
                        push("Y");
                        push("D");
                        printStack();
                        expression(lex);
                        if(lex->last_token_type == TOKEN_EQU || lex->last_token_type == TOKEN_NEQ || lex->last_token_type == TOKEN_LES || lex->last_token_type == TOKEN_LEQ || lex->last_token_type == TOKEN_GTR || lex->last_token_type == TOKEN_GEQ){
                                pop();
                                int logictype;
                                switch(lex->last_token_type)
                                {
                                        case TOKEN_EQU :
                                                push("EQU");
                                                printStack();
                                                logictype = isEQU;
                                                break;
                                        case TOKEN_NEQ :
                                                push("NEQ");
                                                printStack();
                                                logictype = isNEQ;
                                                break;
                                        case TOKEN_LES :
                                                push("LES");
                                                printStack();
                                                logictype = isLES;
                                                break;
                                        case TOKEN_LEQ :
                                                push("LEQ");
                                                printStack();
                                                logictype = isLEQ;
                                                break;
                                        case TOKEN_GTR :
                                                push("GTR");
                                                printStack();
                                                logictype = isGTR;
                                                break;
                                        case TOKEN_GEQ :
                                                push("GEQ");
                                                printStack();
                                                logictype = isGEQ;
                                                break;
                                }
                                pop();
                                printStack();
                                record(lex);
                                PL0Lex_get_token(lex);
                                expression(lex);
                                swapTop();
                                switch(logictype){
                                        case isEQU:
                                                eq();
                                                break;
                                        case isNEQ:
                                                neq();
                                                break;
                                        case isLES:
                                                lt();
                                                break;
                                        case isLEQ:
                                                le();
                                                break;
                                        case isGTR:
                                                gt();
                                                break;
                                        case isGEQ:
                                                ge();
                                                break;
                                }
                                
                        }
                        else{
                                if(lex->row == cur_row){
				        Get_line_content();
			  	        Print_Error1(ERR_WRONG_OP_IN_BINARY_COND);
			        }
			        else
			                PL0_print_error(ERR_WRONG_OP_IN_BINARY_COND,last_line_content,cur_row,cur_position);
                                pop();
                                printStack();
                                //record(lex);
                                //PL0Lex_get_token(lex);
                                expression(lex);
                        }
                }
        }
        else {
                Get_line_content();
		Print_Error1(ERR_WRONG_BEGIN_OF_COND);
                pop();
                printStack();
        }
}

void condition(PL0Lex * lex) {
	printf("analysis the condition expression\n");
	struct addrlist *head=NULL;
	struct addrlist *tail=NULL;
	pop();
        push("U");
	push("J");
	printStack();
	condition2(lex);
        int and_or;
	while(lex->last_token_type == TOKEN_AND || lex->last_token_type == TOKEN_OR){
	        pop();
		push("W");
		if(lex->last_token_type == TOKEN_AND){
                        push("AND");
                        and_or = isAND;
                }
		else{
			push("OR");
                        and_or = isOR;
                }
		printStack();
                pop();
		printStack();
		pop();
		push("U");
		push("J");
		printStack();
                record(lex);
		PL0Lex_get_token(lex);
                
                put(0);
                st();
                ldAddress(0);
                ldAddress(0);
                if(and_or == isAND)
                        logicNot();
		struct addrlist *jumpplace=malloc(sizeof(struct addrlist));
                jumpplace->line = ftell(fout);
		jumpplace->next=NULL;
		if(head==NULL){
			head=jumpplace;
			tail=head;
		}
		else{
			tail->next=jumpplace;
			tail=tail->next;
		}
                blankforjmp();
		condition2(lex);
                if(and_or == isAND){
                        logicAnd();
                }
                else{
                        logicOr();
                }
	}
	struct addrlist *p=head;
 	while(head!=NULL){
		fseek(fout, head->line - ftell(fout), 1);
                jpc(insline);
                insline --;
		fseek(fout, 0, 2);
		p=head;	
		head=head->next;
		free(p);
	}
	pop();
	printStack();
        printf("end of condition\n");
}

void expression(PL0Lex * lex) {
	printf("analysis the expression\n");
        pop();
        push("Z");
        push("I");
        printStack();
        term(lex);
	int plusOrMinus;
        while(lex->last_token_type == TOKEN_PLUS || lex->last_token_type == TOKEN_MINUS){
                push("I");
                if(lex->last_token_type == TOKEN_PLUS)
                {       
                        push("PLUS");
			plusOrMinus = isPLUS;
		}
                else{
                        push("MINUS");
			plusOrMinus = isMINUS;
		}
                printStack();
                pop();
                printStack();
                record(lex);
                PL0Lex_get_token(lex);
                term(lex);
		if(plusOrMinus == isPLUS)
			add();
		else{
			swapTop();
			sub();
		}
        }
        pop();
        printStack();
        printf("end of expression\n");
}

void term(PL0Lex * lex) {
	printf("analysis the term\n");
        pop();
        push("Q");
        push("H");
        printStack();
        factor(lex);
	int flag;
        while(lex->last_token_type == TOKEN_TIMES || lex->last_token_type == TOKEN_SLASH){
                push("H");
                if (lex->last_token_type == TOKEN_TIMES)
                {
			push("TIMES");
			flag = isTIMES;
		}
                else
                {
			push("SLASH");
			flag = isSLASH;
		}
                printStack();
                pop();
                printStack();
                record(lex);
                PL0Lex_get_token(lex);
                factor(lex);
		swapTop();
		if (flag == isTIMES)
		{
			mul();
			//var1 = var1*var2;
		}
		else
		{
		        divNum();
		        //var1 = var1/var2;
		}
        }
        pop();
        printStack();
        printf("end of term\n");
}
void factor(PL0Lex * lex) {
	printf("analysis the factor\n");
        if (lex->last_token_type == TOKEN_IDENTIFIER){
                int i;
                pop();
                push("T");
                push("ident");
                printStack();
                i = table_search(lex->last_id);
                if(i == -1){
                        Get_line_content();
                        Print_Error1(ERR_UD_IDEN);
                }
                put(token_table[i].address);
                pop();//match ident
                printStack();
                record(lex);
                PL0Lex_get_token(lex);
                if(lex->last_token_type == TOKEN_LEFT){//the factor must be a array element
                        if(i!=-1&&token_table[i].kind != ID_ARRAY1&&token_table[i].kind != ID_ARRAY2){
                                Get_line_content();
                                Print_Error1(ERR_IDEN_OF_ARRAY);
                        }
                        pop();//T -> T [ INDEX ]
                        push("T");
                        push("]");
                        push("INDEX");
                        push("[");
                        printStack();
                        pop();//match [
                        printStack();
                        record(lex);
                        PL0Lex_get_token(lex);
                        pop();
                        push("D");
                        printStack();
                        expression(lex);
                        put(token_table[i].bound2);
                        mul();
                        if(lex->last_token_type == TOKEN_RIGHT){
                                pop();
                                printStack();
                                record(lex);
                                PL0Lex_get_token(lex);
                        }
                        else{
                                if(lex->row == cur_row){
			                Get_line_content();
			                Print_Error1(ERR_RIGHT_MRPAREN_MISS);
		                }
		                else
			                PL0_print_error(ERR_RIGHT_MRPAREN_MISS,last_line_content,cur_row,cur_position);
                                pop();//match ]
                                printStack();
                        }
                        if(lex->last_token_type == TOKEN_LEFT){//the factor must be a array element
                                
                                if(i!=-1&&token_table[i].kind != ID_ARRAY2){
                                        Get_line_content();
                                        Print_Error1(ERR_IDEN_OF_ARRAY);
                                }
                                pop();//T -> [ INDEX ]
                                push("T");
                                push("]");
                                push("INDEX");
                                push("[");
                                printStack();
                                pop();//match [
                                printStack();
                                record(lex);
                                PL0Lex_get_token(lex);
                                pop();
                                push("D");
                                printStack();
                                expression(lex);
                                add();
                                if(lex->last_token_type == TOKEN_RIGHT){
                                        pop();
                                        printStack();
                                        record(lex);
                                        PL0Lex_get_token(lex);
                                }
                                else{
                                        if(lex->row == cur_row){
					        Get_line_content();
			                        Print_Error1(ERR_RIGHT_MRPAREN_MISS);
		                        }
		                        else
			                PL0_print_error(ERR_RIGHT_MRPAREN_MISS,last_line_content,cur_row,cur_position);
                                        pop();
                                        printStack();
                                }
                        }
                        else if(i!=-1&&token_table[i].kind == ID_ARRAY2){
                                Get_line_content();
                                Print_Error1(ERR_IDEN_OF_ARRAY);
                        }
                        pop();//kick T out of stack
                        printStack();
                        add();
                        ld();
                }//array
                else{
                        if(i!=-1&&token_table[i].kind != ID_VARIABLE && token_table[i].kind != ID_CONSTANT){
                                Get_line_content();
                                Print_Error1(ERR_IDEN_OF_VAR_OR_CONST);
                        }
                        pop();//T->NULL
                        printStack();
                        //printf("here\n");
                        ld();
		}
        }
        else if (lex->last_token_type == TOKEN_NUMBER) {
                pop();
                push("number");
                printStack();
                pop();
                printStack();
                record(lex);
		put(lex->last_num);
                PL0Lex_get_token(lex);
        }
        else if (lex->last_token_type == TOKEN_MINUS) {
                pop();
                push("D");
                push("MINUS");
                printStack();
                pop();
                printStack();
                record(lex);
		put(-1);
	        PL0Lex_get_token(lex);
		expression(lex);
		mul();
				}
        else if (lex->last_token_type == TOKEN_LPAREN) {
                pop();
                push("RPAREN");
                push("D");
                push("LPAREN");
                printStack();
                pop();
                printStack();
                record(lex);
                PL0Lex_get_token(lex);
		expression(lex);
                if(lex->last_token_type != TOKEN_RPAREN) {
                        if(lex->row == cur_row){
			        Get_line_content();
			        Print_Error1(ERR_RIGHT_PARENTHESIS_MISS);
		        }
		        else
			        PL0_print_error(ERR_RIGHT_PARENTHESIS_MISS,last_line_content,cur_row,cur_position);
                }
                else{
                        record(lex);
                        PL0Lex_get_token(lex);
                }
                pop();
                printStack();
        }//L_paren
        else if(lex->last_token_type == TOKEN_CALL){
                pop();
                push("ident");
                push("call");
                printStack();
                pop();
                printStack();
                record(lex);
                PL0Lex_get_token(lex);
                if(lex->last_token_type != TOKEN_IDENTIFIER){
                        if(lex->row == cur_row){
			        Get_line_content();
			        Print_Error1(ERR_CALL_EXPECT_IDEN);
		        }
		        else
			        PL0_print_error(ERR_CALL_EXPECT_IDEN,last_line_content,cur_row,cur_position);
                }
                else{
                        int i = table_search(lex->last_id);
                        if(i==-1||token_table[i].kind!=ID_PROCEDURE){
                                Get_line_content();
			        Print_Error1(ERR_UD_IDEN);
                        }
                        put(insline + 3);
                        stAddress(token_table[i].address);
                        jmptoaddress(token_table[i].bound1);
                        fprintf(fout, "%c", '\n');
                        ldAddress(token_table[i].address + 1);
                        record(lex);
                        PL0Lex_get_token(lex);
                }
                pop();
                printStack();

        }//call
       else{
                Get_line_content();
	        Print_Error1(ERR_WRONG_BEGIN_OF_FACTOR);
                pop();
                printStack();
       }
       printf("end of factor\n");
}

void program(PL0Lex * lex) {
        initStack();
        insline = 0;
        push("$");
        push("A");
        printStack();
        pop();
        push(".");
        push("B");
        printStack();
        push("S");
        printStack();
        initposses();
        strcpy(posses,"main");
        program_block(lex);
        pop();
        printStack();
        
        while(lex->last_token_type != TOKEN_EOF){
                if (lex->last_token_type == TOKEN_PERIOD){
                        pop();
                        printStack();
                        printf("the program has been analysised\n");
                        INT(1);
                        break;
                }
                else{
                        Print_Error1(ERR_UNWANT_LEX);
                        record(lex);
                        PL0Lex_get_token(lex);
                }
        }
        if(lex->last_token_type == TOKEN_EOF){
                PL0_print_error(ERR_DOT_MISS,last_line_content,cur_row,cur_position);
                pop();
                printStack();
                printf("the program has been analysised\n");
        }

}
void checkindex(PL0Lex * lex , int div) {
        if(lex->last_token_type == TOKEN_MINUS){
                Get_line_content();
		Print_Error1(ERR_INDEX_OF_ARRAY);
		record(lex);
                PL0Lex_get_token(lex);
        }
        if(lex->last_token_type == TOKEN_NUMBER){
		pop();
                push("number");
                printStack();
		pop();
                printStack();
                if(div == 1)
		        token_table[table_index - 1].bound1 = lex->last_num;
                else
                        token_table[table_index - 1].bound2 = lex->last_num;
		record(lex);
                PL0Lex_get_token(lex);
        }
        else if(lex->last_token_type == TOKEN_IDENTIFIER){
		pop();
                push("ident");
                printStack();
                int i;
                i = table_search(lex->last_id);
                if(i == -1){
                	Get_line_content();
			Print_Error1(ERR_UD_IDEN);
                }
		else{
                	if(token_table[i].kind != ID_CONSTANT){
                        	Get_line_content();
				Print_Error1(ERR_UC_IDEN);
			}
                        else{
                                if(token_table[i].value < 0){
                                	Get_line_content();
					Print_Error1(ERR_INDEX_OF_ARRAY);
                                        if(div == 1)
                                                token_table[table_index - 1].bound1 = 0 - token_table[i].value;
                                        else
                                	        token_table[table_index - 1].bound2 = 0 - token_table[i].value;
                                }
                                else
                                      	if(div == 1)
                                                token_table[table_index - 1].bound1 = token_table[i].value;
                                        else
                                	        token_table[table_index - 1].bound2 = token_table[i].value;
                        }
                }
                pop();//match ident
                printStack();
		record(lex);
                PL0Lex_get_token(lex);
	}
        else{
                 if(lex->row == cur_row){
			Get_line_content();
			Print_Error1(ERR_INDEX_MISS);
		}
		else
			PL0_print_error(ERR_INDEX_MISS,last_line_content,cur_row,cur_position);
                pop();//match index
                printStack();

        }
        if(lex->last_token_type == TOKEN_RIGHT){//normal
                pop();//match ]
                printStack();
		record(lex);
                PL0Lex_get_token(lex);
        }
        else{
                if(lex->row == cur_row){
			Get_line_content();
			Print_Error1(ERR_RIGHT_MRPAREN_MISS);
		}
		else
			PL0_print_error(ERR_RIGHT_MRPAREN_MISS,last_line_content,cur_row,cur_position);
                pop();//match ]
                printStack();
	}
}

void const_declaration(PL0Lex * lex) {
	if (lex->last_token_type == TOKEN_IDENTIFIER) {

                pop();
                push("E");
                push("NUMBER");//NUMBER -> number | - number
                push("=");
                push("ident");
                printStack();
                pop();
                printStack();
		record(lex);
		PL0Lex_get_token(lex);
                
		if (lex->last_token_type == TOKEN_EQU || lex->last_token_type == TOKEN_BECOMES) {
			if (lex->last_token_type == TOKEN_BECOMES)
				printf("found ':=' when expecting '='\n");
                        pop();
                        printStack();
			record(lex);
			PL0Lex_get_token(lex);
                }
                else{
			Get_line_content();
			if(lex->row == cur_row)
				Print_Error1(ERR_IDEN_EXPECT_EQUAL);
			else
				PL0_print_error(ERR_IDEN_EXPECT_EQUAL,last_line_content,cur_row,cur_position);
                        pop();
                        printStack();
		}
                if(lex->last_token_type == TOKEN_MINUS){
			pop();
                        push("number");
                        push("MINUS");
                        printStack();
			record(lex);
			pop();//match -
                        printStack();
                        PL0Lex_get_token(lex);
                        if(lex->last_token_type == TOKEN_NUMBER) {
                                table_append(lex, ID_CONSTANT);
                                token_table[table_index - 1].value = 0-lex->last_num;
                                
                                pop();//match number
                                printStack();
				record(lex);
                                PL0Lex_get_token(lex);
                        }
                        else{
				Get_line_content();
				if(lex->row == cur_row)
					Print_Error1(ERR_MUNI_EXPECT_NUMBER);
				else
					PL0_print_error(ERR_MUNI_EXPECT_NUMBER,last_line_content,cur_row,cur_position);
					pop();//match number
                                        printStack();
                                }
			}
		else if(lex->last_token_type == TOKEN_NUMBER){
                        table_append(lex, ID_CONSTANT);
                        token_table[table_index-1].value = lex->last_num;
                        pop();
                        push("number");
                        printStack();
                        pop();//match number
                        printStack();
			record(lex);
			PL0Lex_get_token(lex);
		}
                else{
			Get_line_content();
			if(lex->row == cur_row)
				Print_Error1(ERR_ILLEGAL_ASSIGNMENT);
			else
				PL0_print_error(ERR_ILLEGAL_ASSIGNMENT,last_line_content,cur_row,cur_position);
                        pop();
                        printStack();
		}
                put(token_table[table_index - 1].value);
                put(token_table[table_index - 1].address);
                st();
	}
        else{
		if(last_token_type ==TOKEN_CONST){
			if(lex->row == cur_row){
				Get_line_content();
				Print_Error1(ERR_CONST_EXPECT_IDEN);
			}
			else
				PL0_print_error(ERR_CONST_EXPECT_IDEN,last_line_content,cur_row,cur_position);
		}
		else{
			if(lex->row == cur_row){
				Get_line_content();
				Print_Error1(ERR_COMMA_EXPECT_IDEN);
			}
			else
				PL0_print_error(ERR_COMMA_EXPECT_IDEN,last_line_content,cur_row,cur_position);
		}
	}
} //const_declaration

void var_declaration(PL0Lex * lex) {
	if (lex->last_token_type == TOKEN_IDENTIFIER) {
                pop();
                push("E");
                push("T");
                push("ident");
                printStack();
                pop();
                printStack();
		record(lex);
		PL0Lex_get_token(lex);
		if(lex->last_token_type == TOKEN_LEFT){//there must be a array
                	pop();// T -> [ INDEX ] T
                        push("T");
                        push("]");
                        push("INDEX");// INDEX -> number | ident
                        push("[");
                        printStack();
                        table_append(lex, ID_ARRAY1);
                        pop();//match '['
                        printStack();
			record(lex);
                        PL0Lex_get_token(lex);
                        checkindex(lex, 1);
                        if(lex->last_token_type == TOKEN_LEFT){//DimV = 2
                        	pop();
                                push("T");
                                push("]");
                                push("INDEX");// INDEX -> number | ident
                                push("[");
                                printStack();
                                token_table[table_index - 1].kind = ID_ARRAY2;
                                pop();//match '['
                                printStack();
				record(lex);
                                PL0Lex_get_token(lex);
                                checkindex(lex, 2);
                        }
                        offsetforst += token_table[table_index - 1].bound1 * token_table[table_index - 1].bound2 ;
		}
		else{
                       table_append(lex, ID_VARIABLE);
                }
                pop();//match 'T'
                printStack();
	}
        else{
		if(last_token_type == TOKEN_VAR){
			if(lex->row == cur_row){
				Get_line_content();
				Print_Error1(ERR_VAR_EXPECT_IDEN);
			}
			else
				PL0_print_error(ERR_VAR_EXPECT_IDEN,last_line_content,cur_row,cur_position);
		}
		else{
			if(lex->row == cur_row){
				Get_line_content();
				Print_Error1(ERR_COMMA_EXPECT_IDEN);
			}
			else
				PL0_print_error(ERR_COMMA_EXPECT_IDEN,last_line_content,cur_row,cur_position);
		}
	}
} //var_declaration

void program_block(PL0Lex * lex) {
	printf("analysis the program block\n");
	do {

		if (lex->last_token_type == TOKEN_CONST) {
                        pop();
                        push("S");
                        push("SEMICOLON");
                        push("V");
                        push("const");
                        printStack();
                        pop();
                        printStack();
			record(lex);
			PL0Lex_get_token(lex);
			const_declaration(lex);
			while (lex->last_token_type == TOKEN_COMMA) {
                                pop();
                                push("V");
                                push("COMMA");
                                printStack();
                                pop();
                                printStack();
				record(lex);
				PL0Lex_get_token(lex);
				const_declaration(lex);
			}
			if (lex->last_token_type == TOKEN_SEMICOLON) {
				pop();
                                printStack();
                                pop();
                                printStack();
				record(lex);
				PL0Lex_get_token(lex);
			}
			else {
				if(lex->row == cur_row){
						Get_line_content();
			  			Print_Error1(ERR_SEMICOLON_OR_COMMA_MISS);
				}
				else
					PL0_print_error(ERR_SEMICOLON_OR_COMMA_MISS,last_line_content,cur_row,cur_position);
                                pop();
				printStack();
				pop();
                                printStack();
			}
		}//if  TOKEN_CONST
  		else if (lex->last_token_type == TOKEN_VAR) {
			record(lex);
			PL0Lex_get_token(lex);
                        pop();
                        push("S");
                        push("SEMICOLON");
                        push("V");
                        push("var");
                        printStack();
                        pop();
                        printStack();
			var_declaration(lex);
		        while (lex->last_token_type == TOKEN_COMMA) {
                                        pop();
                                        push("V");
                                        push("COMMA");
                                        printStack();
                                        pop();
                                        printStack();
					record(lex);
					PL0Lex_get_token(lex);
					var_declaration(lex);
			}
			if (lex->last_token_type == TOKEN_SEMICOLON) {
				pop();
                                printStack();
                                pop();
                                printStack();
				record(lex);
				PL0Lex_get_token(lex);
			}
			else {
				if(lex->row == cur_row){
					Get_line_content();
			  		Print_Error1(ERR_SEMICOLON_OR_COMMA_MISS);
				}
				else
					PL0_print_error(ERR_SEMICOLON_OR_COMMA_MISS,last_line_content,cur_row,cur_position);
                                pop();
				printStack();
				pop();
                                printStack();
			}
		}//else if TOKEN_VAR
  		else if (lex->last_token_type == TOKEN_PROCEDURE) {
                        level++;
                        pop();
                        push("S");
                        push("SEMICOLON");
                        push("S");
                        push("SEMICOLON");
                        push("ident");
                        push("PROCEDURE");
                        printStack();
                        pop();
                        printStack();
                        record(lex);
			PL0Lex_get_token(lex);
		        if(lex->last_token_type == TOKEN_IDENTIFIER){
                		pop();
                		printStack();
                		table_append(lex, ID_PROCEDURE);
                                record(lex);
                		PL0Lex_get_token(lex);
                        }
                        else{
                                if(lex->row == cur_row){
				Get_line_content();
			  	Print_Error1(ERR_PROC_EXPECT_IDEN);
				}
				else
					PL0_print_error(ERR_PROC_EXPECT_IDEN,last_line_content,cur_row,cur_position);
                      		pop();
                      		printStack();
                        }
			if (lex->last_token_type == TOKEN_SEMICOLON) {
                      		pop();
                     		printStack();
                                record(lex);
				PL0Lex_get_token(lex);
			}
			else {
				if(lex->row == cur_row){
				Get_line_content();
			  	Print_Error1(ERR_SEMICOLON_MISS);
				}
				else
					PL0_print_error(ERR_SEMICOLON_MISS,last_line_content,cur_row,cur_position);
                      		pop();
                      		printStack();
			}
                        int jumpplace = ftell(fout);
                        blankforjmp();
                        token_table[table_index - 1].bound1 = insline;//for jump into procedure
                        int i = table_index;
                        
                        char buff[MAX_ID_LEN + 1];
                        strcpy(buff, posses);
                        initposses();
                        strcpy(posses,token_table[i - 1].name);
                        
                        
                	program_block(lex);
                        
                        initposses();
                        strcpy(posses,buff);
                        ldAddress(token_table[i - 1].address);
                        jmp();
                        fprintf(fout, "%c", '\n');
                        fseek(fout, jumpplace - ftell(fout), 1);
                        jmptoaddress(insline);
                        insline --;
                        fseek(fout, 0, 2);
                	if (lex->last_token_type == TOKEN_SEMICOLON) {
                    		pop();
                    		printStack();
                                record(lex);
				PL0Lex_get_token(lex);
			}
			else {
				if(lex->row == cur_row){
				        Get_line_content();
			  	        Print_Error1(ERR_SEMICOLON_MISS);
				}
				else
					PL0_print_error(ERR_SEMICOLON_MISS,last_line_content,cur_row,cur_position);
                    		pop();
                    		printStack();
			}
          		level--;
		}//else if TOKEN_PROCEDURE
                else if(lex->last_token_type != TOKEN_IDENTIFIER&&lex->last_token_type != TOKEN_CALL&&lex->last_token_type != TOKEN_BEGIN&&lex->last_token_type != TOKEN_IF&&lex->last_token_type != TOKEN_WHILE){
                        Get_line_content();
			Print_Error1(ERR_WRONG_BEGIN_OF_PROC);
                }
	} while(lex->last_token_type == TOKEN_CONST || lex->last_token_type == TOKEN_VAR || lex->last_token_type == TOKEN_PROCEDURE);
        pop();
	push("P");
  	printStack();
  	statement(lex);
  	printf("end of program blook\n");
} //program _ block
