#ifndef __PL0_INS_H__
#define __PL0_INS_H__
#define INSMAX 100


int insline;
char * insbuff[INSMAX];

void blankforjmp();
void put(int var);
//LD /*从栈顶取一个值（address），将memory中address位置的值放入栈顶 */
void ld();
//LD address /* 将memory中address位置的值放入栈顶 */
void ldAddress(int address);
//ST  /*从栈顶取一个值（address），再取一个值（v），将值v存入memory中address位置 */
void st();
//ST address  /*从栈顶取一个值（v），将值v存入memory中address位置 */
void stAddress(int address);
void add();
void sub();
void mul();
void divNum();
void eq();
void neq();
void le();
void lt();
void ge();
void gt();
void jpc(int num);
void jmp();
void jmptoaddress(int back);
void logicAnd();
void logicOr();
void logicNot();
void INT(int flag);
void swapTop();
#endif /* ins.h */
