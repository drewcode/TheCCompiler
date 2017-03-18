## The current grammer implemented by the parser
```
Program --> Vardecl Program | main() { Stmtlist } EOF 

Vardecl 	-->	Vartype Decl ;
Vartype	-->	 CHAR | INT | DOUBLE

Decl	-->	ID Declcontd1
Declcontd1	--> , Decl | = Expr1 Declcontd2 | ep
Declcontd2	--> , Decl | ep

Expr	-->	ID = Expr1

Expr1	-->	Expr2 Expr1'
Expr1'	-->	|| Expr2 Expr1' | ep

Expr2	-->	Expr3 Expr2'
Expr2'	-->	&& Expr3 Expr2' | ep

Expr3	-->	Expr4 Expr3'
Expr3'	-->	== Expr4 Expr3' | != Expr4 Expr3' | ep

Expr4	-->	Expr5 Expr4'
Expr4'	-->	Relop Expr5 Expr4' | ep
Relop	-->	> | < | >= | <=

Expr5	-->	Expr6 Expr5'
Expr5'	-->	+ Expr6 Expr5' | - Expr6 Expr5' | ep

Expr6	-->	Expr7 Expr6'
Expr6'	-->	* Expr7 Expr6' | * Expr7 Expr6' | ep

Expr7	-->	LITERAL | ID | (Expr1)

Stmt --> ; | Expr ; | Vardecl | while ( Expr ) { Stmtlist } | return Expr ;
Stmtlist --> Stmt Stmtlist | ep
```
