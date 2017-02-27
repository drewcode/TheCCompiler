# Grammar

A specification of the grammar used by this compiler

```
Program ->

DataType -> INT | DOUBLE | FLOAT | LONG | CHAR
ReturnDataType -> VOID | DataType

FunctionDeclaration -> ReturnDataType IDENTIFIER OPEN_PAREN FunctionDeclarationArgumentListStart CLOSE_PAREN SEMI_COLON
FunctionDeclarationArgumentListStart -> DataType FunctionDeclarationArgumentIdentifier FunctionDeclarationArgumentListMore
FunctionDeclarationArgumentListMore -> COMMA DataType FunctionDeclarationArgumentIdentifier FunctionDeclarationArgumentListMore | epsilon
FunctionDeclarationArgumentIdentifier -> IDENTIFIER | epsilon

FunctionDefinition -> ReturnDataType IDENTIFIER OPEN_PAREN FunctionDefinitionArgumentListStart CLOSE_PAREN StatementBlock
FunctionDefinitionArgumentListStart -> DataType IDENTIFIER FunctionDefinitionArgumentListMore
FunctionDefinitionArgumentListMore -> COMMA DataType IDENTIFIER FunctionDefinitionArgumentListMore | epsilon

StatementBlock -> Statement SEMI_COLON | OPEN_BRACE StatementList CLOSE_BRACE
StatementList -> Statement SEMI_COLON StatementList | epsilon

Statement -> VariableDecOrDef SEMI_COLON  | AssignmentStatement SEMI_COLON | FunctionCall SEMI_COLON | WhileStatement

VariableDecOrDef -> DataType VariableDecOrDefListStart
VariableDecOrDefListStart -> IDENTIFIER VariableOptionalInitialization VariableDecOrDefListMore
VariableDecOrDefListMore -> COMMA IDENTIFIER VariableOptionalInitialization VariableDecOrDefListMore
VariableOptionalInitialization -> EQUAL Expression | epsilon

AssignmentStatement -> IDENTIFIER EQUAL Expression
Expression -> LITERAL | IDENTIFIER | FunctionCall

FunctionCall -> IDENTIFIER OPEN_PAREN FunctionCallArgumentListStart CLOSE_PAREN
FunctionCallArgumentListStart -> Expression FunctionCallArgumentListMore
FunctionCallArgumentListMore -> COMMA Expression FunctionCallArgumentListMore | epsilon

WhileStatement -> WHILE OPEN_PAREN Expression CLOSE_PAREN StatementBlock
```
