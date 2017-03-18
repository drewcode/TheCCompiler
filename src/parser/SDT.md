## The SDT for the current Grammar
```

Program --> Vardecl {list = Program.inhDeclList.push(Vardecl.node); Program1.inhDeclList = list} Program
    | main() { Stmtlist } EOF {Program.node = new Program(Program.inhDeclList, Stmtlist.node)}

Vardecl 	-->	Vartype {Decl.inhType = Vartype.type, Decl.inhList = []} Decl {Vardecl.node = new VariableDeclarationStatement(Decl.list ,Vartype.type)};
Vartype	-->	 CHAR {Vartype.type = CHAR}
    | INT {Vartype.type = INT}
    | DOUBLE {Vartype.type = DOUBLE}

Decl	-->	ID { id = new Identifier(); Declcontd1.inhList = Decl.inhList; Declcontd1.inhId = id; Declcontd1.inhType = Decl.inhType } Declcontd1 {Decl.list = Declcontd1.list}

Declcontd1	--> , { list = Declcontd1.inhList.push(new VariableDeclarator(Declcontd1.inhId, null)); Decl.inhList = list; Decl.inhType = Declcontd1.inhType} Decl { Declcontd1.list = Decl.list }
    | = Expr1 {list = Declcontd1.inhList.push(new VariableDeclarator(Declcontd1.inhId, Expr1.node)); Declcontd2.inhList = list; Declcontd2.inhType = Declcontd1.inhType} Declcontd2 {Declcontd1.list = Declcontd2.list}
    | ep {Declcontd1.list = Declcontd1.inhList }

Declcontd2	--> , { Decl.inhList = Declcontd2.inhList; Decl.inhType = Declcontd2.inhType} Decl {Declcontd2.list = Decl.list}
    | ep {Declcontd2.list = Declcontd2.inhList }


Expr	-->	ID = Expr1 {id = new Identifier(); Expr.node = new AssignmentExpression(id, Expr1.node)}

Expr1	-->	Expr2 {Expr1'.inhNode = Expr2.node} Expr1' {Expr1.node = Expr1'.node}
Expr1'	-->	|| Expr2 { result = new BinaryExpression(!=, Expr1'.inhNode, Expr2.node); Expr1'1.inhNode = result; } Expr1' {Expr1'.node = Expr1'1.node}
    | ep {Expr1'.node = Expr1'.inhNode}

Expr2	-->	Expr3 {Expr2'.inhNode = Expr3.node} Expr2' {Expr2.node = Expr2'.node}
Expr2'	-->	&& Expr3 { result = new BinaryExpression(!=, Expr2'.inhNode, Expr3.node); Expr2'1.inhNode = result; } Expr2' {Expr2'.node = Expr2'1.node}
    | ep {Expr2'.node = Expr2'.inhNode}

Expr3	-->	Expr4 {Expr3'.inhNode = Expr4.node} Expr3' {Expr3.node = Expr3'.node}
Expr3'	-->	== Expr4 { result = new BinaryExpression(==, Expr3'.inhNode, Expr4.node); Expr3'1.inhNode = result; } Expr3' {Expr3'.node = Expr3'1.node}
    | != Expr4 { result = new BinaryExpression(!=, Expr3'.inhNode, Expr4.node); Expr3'1.inhNode = result; } Expr3' {Expr3'.node = Expr3'1.node}
    | ep {Expr3'.node = Expr3'.inhNode}

Expr4	-->	Expr5 {Expr4'.inhNode = Expr5.node} Expr4' {Expr4.node = Expr4'.node}
Expr4'	-->	> Expr5 { result = new BinaryExpression(>, Expr4'.inhNode, Expr5.node); Expr4'1.inhNode = result; } Expr4' {Expr4'.node = Expr4'1.node}
    | >= Expr5 { result = new BinaryExpression(>=, Expr4'.inhNode, Expr5.node); Expr4'1.inhNode = result; } Expr4' {Expr4'.node = Expr4'1.node}
    | <= Expr5 { result = new BinaryExpression(<=, Expr4'.inhNode, Expr5.node); Expr4'1.inhNode = result; } Expr4' {Expr4'.node = Expr4'1.node}
    | < Expr5 { result = new BinaryExpression(<, Expr4'.inhNode, Expr5.node); Expr4'1.inhNode = result; } Expr4' {Expr4'.node = Expr4'1.node}
    | ep {Expr4'.node = Expr4'.inhNode}

Expr5	-->	Expr6 {Expr5'.inhNode = Expr6.node} Expr5' {Expr5.node = Expr5'.node}
Expr5'	-->	+ Expr6 { result = new BinaryExpression(+, Expr5'.inhNode, Expr6.node); Expr5'1.inhNode = result;  } Expr5' {Expr5'.node = Expr5'1.node}
    | - Expr6 { result = new BinaryExpression(-, Expr5'.inhNode, Expr6.node); Expr5'1.inhNode = result;  } Expr5' {Expr5'.node = Expr5'1.node}
    | ep {Expr5'.node = Expr5'.inhNode}

Expr6	-->	Expr7 {Expr6'.inhNode = Expr7.node} Expr6' {Expr6.node = Expr6'.node}
Expr6'	-->	* Expr7 { result = new BinaryExpression(*, Expr6'.inhNode, Expr7.node); Expr6'1.inhNode = result;  } Expr6' {Expr6'.node = Expr6'1.node}
    | / Expr7 { result = new BinaryExpression(/, Expr6'.inhNode, Expr7.node); Expr6'1.inhNode = result;  } Expr6' {Expr6'.node = Expr6'1.node}
    | ep {Expr6'.node = Expr6'.inhNode}

Expr7	-->	LITERAL {Expr7.node = new Literal()}
    | ID {Expr7.node = new Identifier()}
    | (Expr1) {Expr7.node = Expr1.node}

Stmt --> ; {Stmt.node = new EmptyStatement()}
    | Expr ; {Stmt.node = new AssignmentStatement(Expr.node)}
    | Vardecl {Stmt.node = Vardecl.node}
    | while ( Expr ) { {Stmtlist.inhList = []} Stmtlist } {Stmt.node = new WhileStatement(Expr.node, Stmtlist.node)}
    | return Expr ; {Stmt.node = new ReturnStatement(Expr.node)}

Stmtlist --> Stmt {list = Stmtlist.inhList.push(Stmt.node); Stmtlist1.inhList = list} Stmtlist
    | ep {Stmtlist.list = Stmtlist.inhType}

```
