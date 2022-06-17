#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ins.h"
#include "../common/common.h"
#include "../lex/pl0_lex.h"
#include "../error/pl0_error.h"

char blank[8] = {/*' ', ' ', ' ', ' ', */' ', ' ', ' ', ' ', ' ', ' ', ' ', '\n'};
void blankforjmp(){
  fprintf(fout, "%s", blank);
  insline ++;
}
void put(int var){/* 将value放入栈顶 */
    fprintf(fout, "%s %d%c", "PUT", var, '\n');
    insline ++;
}//put
//LD /*从栈顶取一个值（address），将memory中address位置的值放入栈顶 */
void ld(){
  fprintf(fout, "%s%c", "LD", '\n');
  insline ++;
}
//LD address /* 将memory中address位置的值放入栈顶 */
void ldAddress(int address){
  fprintf(fout, "%s %d%c", "LD", address, '\n');
  insline ++;
}
//ST  /*从栈顶取一个值（address），再取一个值（v），将值v存入memory中address位置 */
void st(){
  fprintf(fout, "%s%c", "ST", '\n');
  insline ++;
}
//ST address  /*从栈顶取一个值（v），将值v存入memory中address位置 */
void stAddress(int address){
  fprintf(fout, "%s %d%c", "ST", address, '\n');
  insline ++;
}
void add(){
  fprintf(fout, "%s%c", "ADD", '\n');
  insline ++;
}
void sub(){
  fprintf(fout, "%s%c", "SUB", '\n');
  insline ++;
}
void mul(){
  fprintf(fout, "%s%c", "MUL", '\n');
  insline ++;
}
void divNum(){
  fprintf(fout, "%s%c", "DIV", '\n');
  insline ++;
}
void eq(){
  fprintf(fout, "%s%c", "EQ", '\n');
  insline ++;
}
void neq(){
  eq();
  logicNot();
}
void le(){
  fprintf(fout, "%s%c", "LE", '\n');
  insline ++;
}
void lt(){
  fprintf(fout, "%s%c", "LT", '\n');
  insline ++;
}
void ge(){
  fprintf(fout, "%s%c", "GE", '\n');
  insline ++;
  }
void gt(){
  fprintf(fout, "%s%c", "GT", '\n');
  insline ++;
}
void jpc(int num){
  fprintf(fout, "%s %d", "JPC", num);
  insline ++;
}
void jmp(){
  fprintf(fout, "%s", "JMP");
  insline ++;
}
void jmptoaddress(int back){
  fprintf(fout, "%s %d", "JMP", back);
  insline ++;
}
void logicAnd(){
  fprintf(fout, "%s%c", "AND", '\n');
  insline ++;
}
void logicOr(){
  fprintf(fout, "%s%c", "OR", '\n');
  insline ++;
}
void logicNot(){
  fprintf(fout, "%s%c", "NOT", '\n');
  insline ++;
}
void INT(int flag){
  fprintf(fout, "%s %d%c", "INT", flag, '\n');
  insline ++;
}
void swapTop(){
  //用来将栈顶的两个值互换位置
  stAddress(0);
  stAddress(1);
  ldAddress(0);
  ldAddress(1);
}
