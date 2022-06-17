# 产生式
## 程序

- A -> B.
- B ->S B|ε

## 程序体
- S -> const V SEMICOLON S | var V SEMICOLON S |
       procedure ident SEMICOLON S SEMECOLON S | P

## CONST
- V → ident EQU 　NUMBER E
- NUMBER -> MINUS number | number
- E → COMMA V | ε

## VAR
- V -> ident T E
- T -> [INDEX] T | ε
- INDEX -> ident | number | D
- E -> COMMA V | ε

## PROCEDURE
## 语句
- O -> P SEMICOLON O
- P -> ident := D | call ident | begin O end | if W then PR | while W do P | print ident
- R -> else P | elif W then P R | ε      

## 条件
*第三次试验修改的地方，但是还没有更新到ｆｉｒｓｔ　和　ｆｏｌｌｏｗ集合里面的*
- W -> JU
- J -> odd D | D Y D | NOT  J | GLPAREN  W  GRPAREN
- U -> AND W | OR W | ε
- Y -> BECOMES | EQU | LES | LEQ | GTR | GEQ

## 表达式
- D -> IZ
- Z -> PLUS IZ | MINUS IZ | ε

## 项
- I -> HQ
- Q -> TIMES HQ | SLASH HQ | ε

## 因子
- H -> ident | number | MINUS D | LPAREN D RPAREN

# FIRST and FOLLOW
非终结符|FIRST|FOLLOW   
-----|------ | -----    
A|const, ε, var, procedure, ident, call, begin, if, while, PERIOD(.)|$
B|ε, const, var, procedure, ident, call, begin, if, while|PERIOD(.)
S|const, var, procedure, ident, call, begin, if, while|PERIOD(.), const, var, procedure, ident, call, begin, if, while, SEMICOLON
C|const|PERIOD(.)
M|var|PERIOD(.)
V|ident|SEMICOLON(;)
E|ε, comma|SEMICOLON(;)
N|procedure|PERIOD(.)
O|ident, call, begin, if, while|PERIOD(.), end
P|ident, call, begin, if, while|SEMICOLON(;)
J|odd, ident, number, MINUS(-),  LPAREN (|then, do
L|odd, ident, number, MINUS(-),  LPAREN (|then, do
Y|LES(<), LEQ(<=), GTR(>), GEQ(>=), BECOMES(:=), EQU(=)|ident, number, MINUS(-),  LPAREN (
D|ident, number, MINUS(-),  LPAREN (|then, do, RPAREN )
Z|PLUS(+), MINUS(-), ε|then, do, RPAREN )
I|ident, number, MINUS(-),  LPAREN (|PLUS(+), MINUS(-), then, do
Q|ε, TIMES( * ), SLASH(/)|PLUS(+), MINUS(-), then, do
H|ident, number, MINUS(-),  LPAREN (|PLUS(+), MINUS(-), then, do, TIMES( * ), SLASH(/)


# 分析表

非终结符|number|ident|EQU(=)|COMMA(,)|SEMICOLON(;)|PLUS(+)|MINUS(-)|TIMES( * )|SLASH(/)|NEQ(!=)|LES(<)|LEQ(<=)|GTR(>)|GEQ(>=)|LPAREN ( |RPAREN ) |PERIOD(.)|BECOMES(:=)|VAR|CONST|PROCEDURE|BEGIN|END|IF|THEN|DO|WHILE|CALL|ODD|PERIOD(.)|$
-----|------ |------ | ----- |------ | ----- |------ | ----- |------ | ----- |------ | ----- |------ | ----- |------ | ----- |------ | ----- |------ | ----- |------ | -----|------ | ----- |------ | ----- |------ | ----- |------ | -----  |------ | ------
A|-|A -> B.|-|-|-|-|-|-|-|-|-|-|-|-|-|- |-|-|A -> B.|A -> B.|A -> B.|A -> B.|-|A -> B.|-|-|A -> B.|A -> B.|-|A -> B.|A -> B.
C|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|C-> const V SEMICOLON|-|-|-|-|-|-|-|-|-|-|-
B|-|B -> SB |-|-|-|-|-|-|-|-|-|-|-|-|-|- |-|-|B -> SB |B -> SB |B -> SB |B -> SB |-|B -> SB |-|-|B -> SB |B -> SB |-|B -> ε |-
S|-|S -> O |-|-|-|-|-|-|-|-|-|-|-|-|-|- |-|-|S -> M |S -> C |S -> N |S -> O |-|S -> O |-|-|S -> O |S -> O |-|- |-
M|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|M -> var V SEMICOLON|-|-|-|-|-|-|-|-|-|-|-|-
V|-|V -> ident E|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-
E|-|-|-|E -> COMMA V|E -> ε|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-
N|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|N -> procedure ident SEMICOLON S SEMICOLON|-|-|-|-|-|-|-|-|-
O|-|O -> P SEMICOLON |-|-|-|-|-|-|-|-|-|-|-|-|-|- |-|-|- |-|- |O -> P SEMICOLON |-|O -> P SEMICOLON |-|-|O -> P SEMICOLON|O -> P SEMICOLON|-|- |-
P|-|P -> ident := D |-|-|-|-|-|-|-|-|-|-|-|-|-|- |-|-|- |-|- |P -> begin O end |-|P -> if J then P |-|-|P -> while J do P|P -> call ident|-|- |-
J|J -> L|J -> L|-|-|-|-|J -> L|-|-|-|-|-|-|-|J -> L|-|-|-|-|-|-|-|-|-|-|-|-|-|J -> L|-|-
L|L -> D Y D|L -> D Y D|-|-|-|-|L -> D Y D|-|-|-|-|-|-|-|L -> D Y D|-|-|-|-|-|-|-|-|-|-|-|-|-|L -> odd D|-|-
Y|-|-|Y -> EQU|-|-|-|-|-|-|-|Y -> LES|Y -> LEQ|Y -> GTR|Y -> GEQ|-|-|-|Y -> BECOMES|-|-|-|-|-|-|-|-|-|-|-|-|-
D|D -> I|D -> I|-|-|-|-|D -> I|-|-|-|-|-|-|-|D -> I|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-
Z|-|-|-|-|-|Z -> +IZ|Z -> -IZ|-|-|-|-|-|-|-|-|Z ->  ε|-|-|-|-|-|-|-|-|Z ->  ε|Z ->  ε|-|-|-|-|-
I|I -> HQ|I -> HQ|-|-|-|-|I -> HQ|-|-|-|-|-|-|-|I -> HQ|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-
Q|-|-|-|-|-|Q -> ε|Q -> ε|Q -> TIMES HQ|Q -> SLASH HQ|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|Q -> ε|Q -> ε|-|-|-|-|-
H|H -> number|H -> ident|-|-|-|-|H -> MINUS D|-|-|-|-|-|-|-|H -> LPAREN D RPAREN|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-
