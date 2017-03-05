The current grammer implemented by the parser

## Unmodified Grammar

```
Program -> FunctionDeclaration Program
        | EOF

FunctionDeclaration -> FunctionReturnType IDENTIFIER OPEN_PAREN FunctionDeclarationParameterListStart CLOSE_PAREN SEMI_COLON
FunctionReturnType -> VOID | CHAR | INT | DOUBLE
FunctionDeclarationParameterListStart -> ParameterDataType IDENTIFIER FunctionDeclarationParameterListMore | epsilon
FunctionDeclarationParameterListMore -> COMMA ParameterDataType IDENTIFIER FunctionDeclarationParameterListMore | epsilon
ParameterDataType -> CHAR | INT | DOUBLE
```

## Modified Grammar

```

Program -> VOID IDENTIFIER OPEN_PAREN FunctionDeclarationParameterListStart CLOSE_PAREN SEMI_COLON Program
        |  CHAR IDENTIFIER OPEN_PAREN FunctionDeclarationParameterListStart CLOSE_PAREN SEMI_COLON Program
        |  INT IDENTIFIER OPEN_PAREN FunctionDeclarationParameterListStart CLOSE_PAREN SEMI_COLON Program
        |  DOUBLE IDENTIFIER OPEN_PAREN FunctionDeclarationParameterListStart CLOSE_PAREN SEMI_COLON Program
        | EOF

FunctionDeclaration -> FunctionReturnType IDENTIFIER OPEN_PAREN FunctionDeclarationParameterListStart CLOSE_PAREN SEMI_COLON
FunctionReturnType -> VOID | CHAR | INT | DOUBLE
FunctionDeclarationParameterListStart -> CHAR IDENTIFIER FunctionDeclarationParameterListMore
                                       | INT IDENTIFIER FunctionDeclarationParameterListMore
                                       | DOUBLE IDENTIFIER FunctionDeclarationParameterListMore
                                       | epsilon
FunctionDeclarationParameterListMore -> COMMA ParameterDataType IDENTIFIER FunctionDeclarationParameterListMore | epsilon
ParameterDataType -> CHAR | INT | DOUBLE

```
