#include <stdlib.h>
#include <string.h>
#include "../common/common.h"
#include "pl0_lex.h"
#include "../error/pl0_error.h"

const char * TOKEN_RESERVED_WORDS[NRW] = {"var", "const", "procedure", "begin", "end", "if", "then", "do", "while", "call", "odd","elif","else","print","return"};
const char * TOKEN_SYMBOLS[NSYM] = { "+", "-", "*", "/", "=", "!=", "<", "<=", ">", ">=", "(", ")", ",", ";", ".", ":=", "[", "]" ,"&&","||","!","{","}"};
//the last three tokens are logic operand
int position=-1;
int row = 1;
int cur_line_num= 0;
char cur_line_content[100];
int offset=0;
char err_msg[100];
PL0Lex * PL0Lex_create(PL0Compiler * parent)
{
	PL0Lex * lex = (PL0Lex *) calloc(1, sizeof(PL0Lex));
	lex->compiler = parent;

	return lex;
}
void Get_back(int i){
	position-=i;
	offset-=i;
}
void Get_last_line_content(char * last_line_content){
	strcpy(last_line_content,cur_line_content);
}
void Get_line_content(){
	int i=0;
	char buf=fgetc(fin);
	while(buf!=EOF&&buf!='\n'&&buf!='\r'){
		cur_line_content[offset+i]=buf;
		buf=fgetc(fin);
		i++;
	}
	cur_line_content[offset+i]='\0';
	fseek(fin,0-i-1,1);
}
void Print_Error1(char *error){
	strcpy(err_msg,error);
	cur_line_num=row;
	PL0_print_error(err_msg,cur_line_content,cur_line_num, offset-1);
	cur_line_content[offset+1]='\0';
}
void Print_Error2(char *error,int off){
	strcpy(err_msg,error);
	cur_line_num=row;
	PL0_print_error(err_msg,cur_line_content,cur_line_num, off);
	cur_line_content[offset+1]='\0';
}
BOOL PL0Lex_destroy(PL0Lex * lex)
{
	free(lex);
	return TRUE;
}

BOOL PL0Lex_get_token(PL0Lex * lex)
{
        char buf;
        int state=0;
        int i=0;
        int check=0;
        char id[MAX_ID_LEN + 1]="\0";
        while(1){
          buf=fgetc(fin);   //read a char.
          position++;
          if(buf == EOF){
              lex->last_token_type=TOKEN_EOF;
              lex->eposition=0;
              lex->bposition=0;
              Print_Error1(ERR_MISS_END_FOR_COMMENT);
              return FALSE;   //It is the end of the file
          }
          cur_line_content[offset++]=buf;
	  cur_line_content[offset]='\0';
          switch(state){
             	case 0: lex->bposition = position;// It's the initial state,
                     lex->row =row;
                     check =0;
                     i=0;
                     if((buf>='a'&&buf<='z') ||(buf>='A'&&buf<='Z')||buf=='_'){//It's the letter leading to character state.
                          id[i]=buf;
                          i++;
                          state=1;
                     }
                     else if(buf== '-'){//This is negative  symbol.
                          lex->last_token_type=TOKEN_SYMBOL+2;
                          lex->eposition=position;
                          return TRUE;
                     }
                     else if(buf>='0'&&buf<='9'){//This is the number state
                          lex->last_num=buf-'0';
                          i++;
                          state=2;
                     }
                     else if(buf== '+'){//plus
                          lex->last_token_type=TOKEN_SYMBOL+1;
                          lex->eposition=position;
                          return TRUE;
                     }
                     else if(buf== '*'){//multiple
                          lex->last_token_type=TOKEN_SYMBOL+3;
                          lex->eposition=position;
                          return TRUE;
                     }
                     else if(buf== '/')
                           state=3;
                     else if(buf == '='){
                          lex->last_token_type=TOKEN_SYMBOL+5;
                          lex->eposition=position;
                          return TRUE;
                     }
                     else if(buf== '!')
                          state=7;
                     else if(buf== '<')
                          state=8;
                     else if(buf== '>')
                          state=9;
                     else if(buf== '('){
                          lex->last_token_type=TOKEN_SYMBOL+11;
                          lex->eposition=position;
                          return TRUE;
                     }
                     else if(buf== ')'){
                          lex->last_token_type=TOKEN_SYMBOL+12;
                          lex->eposition=position;
                          return TRUE;
                     }
                     else if(buf== ','){
                          lex->last_token_type=TOKEN_SYMBOL+13;
                          lex->eposition=position;
                          return TRUE;
                     }
                     else if(buf== ';'){
                          lex->last_token_type=TOKEN_SYMBOL+14;
                          lex->eposition=position;
                          return TRUE;
                     }
                     else if(buf== ':')
                          state=10;
                     else if(buf== '.'){
                          lex->last_token_type=TOKEN_SYMBOL+15;
                          lex->eposition=position;
                          return TRUE;
                     }
                     else if(buf == '['){
                         lex->last_token_type = TOKEN_SYMBOL + 17;
                         lex->eposition = position;
                         return TRUE;
                     }
										 else if(buf == ']'){
                         lex->last_token_type = TOKEN_SYMBOL + 18;
                         lex->eposition = position;
                         return TRUE;
                     }
										 else if(buf == '{'){
                         lex->last_token_type = TOKEN_GLPAREN;
                         lex->eposition = position;
                         return TRUE;
                     }
										 else if(buf == '}'){
                         lex->last_token_type = TOKEN_GRPAREN;
                         lex->eposition = position;
                         return TRUE;
                     }
										 else if(buf == '&')
										 			state = 11;
										 else if(buf == '|')
										 			state = 12;
                     else if(buf==' ' |buf=='\t')
                          state=0;
                     else if(buf=='\n' ){
                          row++;
                          state=0;
                          position=-1;
                          offset=0;
                     }
                     else if(buf=='\r' ){
                          state=0;
                          position=-1;
			  offset=0;
                     }
                     else {
                          lex->last_token_type=TOKEN_NULL;
			  lex->eposition=position;
			  Get_line_content();
			  Print_Error1(ERR_UNRECOGNIZED_CHAR);
                          return TRUE;
                     }
                     break;
              case 1:if((buf >='a'&&buf<='z' )||(buf >='A'&&buf <='Z')|| (buf>='0'&&buf<='9')||buf=='_'){//this is the identifier and reserved state
                         if(i<MAX_ID_LEN)
                            id[i]=buf;
                         i++;
                      }
                      else{// end of id
                         fseek(fin,-1,1);
                         position--;
			 cur_line_content[--offset]='\0';
                         lex->eposition=position;
                         if(i>=MAX_ID_LEN){// the length of id is too long
                             lex->last_token_type=TOKEN_NULL;
              		     	Get_line_content();
			     			 				Print_Error1(ERR_TOO_LONG_IDEN_OR_NUM);
                             return TRUE;
                         }
                         int j;
                         for(j=0;j<NRW;j++)//reserved words
                            if(strcmp(id,TOKEN_RESERVED_WORDS[j])==0){
															//	if(j == NRW-1)
																//		printf("in lex :%s\n",id);
                               	lex->last_token_type=TOKEN_RESWORDS+j+1;
                                return TRUE;
                            }
                         lex->last_token_type=TOKEN_IDENTIFIER;
                         strcpy(lex->last_id,id);
                         return TRUE;
                     }
                     break;
              case 2:if(buf>='0'&&buf<='9'){//number state
                         i++;
                         lex->last_num=lex->last_num*10+buf-'0';
                      }
                      else if((buf>='a'&&buf<='z') ||(buf>='A'&&buf<='Z')||buf=='_'){
                         check=1;
                         i++;
                         lex->last_token_type=TOKEN_NULL;
                      }
                      else{
                         fseek(fin,-1,1);
                         position--;
                         cur_line_content[--offset]='\0';
                         lex->eposition=position;
                         if(check==1){
			    lex->last_token_type=TOKEN_NULL;
			    Get_line_content();
			    Print_Error1(ERR_WRONG_IDEN_FORMAT);
                            return TRUE;
                          }
                         if(i>9){
                             lex->last_token_type=TOKEN_NULL;
			     Get_line_content();
			     Print_Error1(ERR_TOO_LONG_IDEN_OR_NUM);
                             return TRUE;
                         }
                         lex->last_token_type=TOKEN_NUMBER;
                         return TRUE;
                     }
                     break;
              case 3:switch(buf){//comments or '/'
                        case '/':state=4;break;
                        case '*':state=5;break;
                        default:lex->last_token_type=TOKEN_SYMBOL+4;
                                fseek(fin,-1,1);
                                position--;
                                cur_line_content[--offset]='\0';
                                lex->eposition=position;
                                return TRUE;
                     }
                     break;
              case 4:switch(buf){//"//" commemts
                       case '\n':row++;state=0;position=-1;offset=0;break;
                       case '\r':state=0;offset=0;break;
                       default:break;
                     }
                     break;
              case 5:switch(buf){//"/*" comments
                       case '\n':row++;position=-1;offset=0;break;
                       case '*':state=6;break;
                       default:break;
                     }
                     break;
              case 6:switch(buf){//end of "/*" comments
                       case '/':state=0;break;
                       case '\n':row++;state=5;position=-1;offset=0;break;

                       default:state=5;break;
                     }
                     break;
              case 7:switch(buf){// '!='
                       case '=':lex->last_token_type=TOKEN_SYMBOL+6;
                                lex->eposition=position;
                                return TRUE;
             	       default: lex->last_token_type=TOKEN_NOT;
				fseek(fin,-1,1);position--;
				cur_line_content[--offset]='\0';
		                lex->eposition=position;
				return TRUE;
              	    }break;
              case 8:switch(buf){
                       case '=':lex->last_token_type=TOKEN_SYMBOL+8;
                                lex->eposition=position;
                                return TRUE;
                       default:lex->last_token_type=TOKEN_SYMBOL+7;
                               	fseek(fin,-1,1);position--;
				cur_line_content[--offset]='\0';
                               	lex->eposition=position;
                               	return TRUE;
                     }break;
              case 9:switch(buf){
                       case '=':lex->last_token_type=TOKEN_SYMBOL+10;
                                lex->eposition=position;
                                return TRUE;
                       default:	lex->last_token_type=TOKEN_SYMBOL+9;
                               	fseek(fin,-1,1);
                               	position--;
				cur_line_content[--offset]='\0';
                               	lex->eposition=position;
                               return TRUE;
                     }break;
              case 10:switch(buf){
                        case '=':lex->last_token_type=TOKEN_SYMBOL+16;
                                 lex->eposition=position;
                                 return TRUE;
                        default:lex->last_token_type=TOKEN_NULL;
                                fseek(fin,-1,1);position--;
				cur_line_content[--offset]='\0';
				//Get_line_content();
				Print_Error1(ERR_COLON_EXPECT_EQUAL);
                                return TRUE;
                     	}break;
		case 11://This is used to deal with the &&
			switch(buf){
				case '&':lex->last_token_type=TOKEN_AND;
					lex->eposition=position;
					return TRUE;
				default:
					lex->last_token_type=TOKEN_NULL;
					fseek(fin,-1,1);position--;
					cur_line_content[--offset]='\0';
					Get_line_content();
					Print_Error1(ERR_AND_EXPECT_AND);
					return TRUE;
			}break;
		case 12://This is used to deal with the ||
			switch(buf){
				case '|':lex->last_token_type=TOKEN_OR;
					lex->eposition=position;
					return TRUE;
				default:lex->last_token_type=TOKEN_NULL;
					fseek(fin,-1,1);position--;
					cur_line_content[--offset]='\0';
					Get_line_content();
					Print_Error1(ERR_OR_EXPECT_OR);
					return TRUE;
			}break;
             default:lex->last_token_type=TOKEN_NULL;return TRUE;
	}
       }//while
	return FALSE;
}
