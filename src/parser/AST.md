## AST Produced by Parsing

### Node

```
interface Node {
    type: "Node",

}
```

### Program

```
interface Program extends Node {
    type: "Program",
    decl: [VariableDeclarationStatement],
    main: [Statement]
}

```

### Statement

```
interface Statement extends Node {
    type: "Statement"
}
```

### EmptyStatement

```
interface EmptyStatement extends Statement {
    type: "EmptyStatement"
}
```

### ReturnStatement

```
interface ReturnStatement extends Statement {
    type: "ReturnStatement",
    argument: Expression
}
```

### Variable Declaration Statement

```
interface VariableDeclarationStatement extends Statement {
    type: "VariableDeclarationStatement",
    declarations: [VariableDeclarator],
    kind: INT | DOUBLE | CHAR
}
```

### Variable Declarator

```
interface VariableDeclarator extends Node {
    type: "VariableDeclarator",
    id: Identifier,
    init: Expression | null
}
```

### Assignment Statement

```
interface AssignmentStatement extends Statement {
    type: "AssignmentStatement",
    expr: AssignmentExpression
}
```

### While Statement

```
interface WhileStatement extends Statement {
    type: "WhileStatement",
    test: Expression,
    body: [Statement]
}
```

### Expression

```
interface Expression extends Node {
    type: "Expression"
}
```

### Indentifier

```
interface Indentifier extends Expression {
    type: "Indentifier",
    id: symTabEntry
}
```

### Literal

```
interface Literal extends Expression {
    type: "Indentifier",
    val: char | int | double,
    kind: CHAR | INT | DOUBLE
}
```

### Assignment Expression

```
interface AssignmentExpression extends Expression {
    type: "AssignmentExpression",
    id: Identifier,
    val: Expression
}
```

### BinaryExpression

```
interface BinaryExpression extends Expression {
    type: "BinaryExpression",
    op: && | || | == | != | > | >= | < | <= | + | - | / | *
    left: Expression
    right: Expression
}
```
