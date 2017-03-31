#include "ast.h"
#include <stdio.h>

void print_indent(int indentation) {
    for(int i = 0; i < indentation; i++) {
        printf("\t");
    }
}

void print_literaltype(LiteralType type) {
    if(type == L_CHAR) {
        printf("Char\n");
    } else if(type == L_INT) {
        printf("Int\n");
    } else if(type == L_DOUBLE) {
        printf("Double\n");
    }
}

void print_typetoken(TokenType type) {
    if(type == CHAR) {
        printf("Char\n");
    } else if(type == INT) {
        printf("Int\n");
    } else if(type == DOUBLE) {
        printf("Double\n");
    }
}

ASTIndentifier::ASTIndentifier(int index): index(index) {}
void ASTIndentifier::print(int indentation) {
    print_indent(indentation);
    printf("Identifier %d\n", index);
}

ASTLiteral::ASTLiteral(LiteralType type, void * val): type(type), val(val) {}
void ASTLiteral::print(int indentation) {
    print_indent(indentation);
    printf("Literal\n");
    print_indent(indentation + 1);
    if(type == L_CHAR) {
        printf("Char: %c\n", *(char *)val);
    } else if(type == L_INT) {
        printf("Int: %d\n", *(int *)val);
    } else if(type == L_DOUBLE) {
        printf("Double: %f\n", *(double *)val);
    }
}

ASTBinaryExpression::ASTBinaryExpression(TokenType op, ASTExpression *left, ASTExpression *right): op(op), left(left), right(right) {}
void ASTBinaryExpression::print(int indentation) {
    print_indent(indentation);
    printf("BinaryExpression\n");
    print_indent(indentation + 1);
    printf("%d\n", op);
    left->print(indentation + 1);
    right->print(indentation + 1);
}

ASTAssignmentExpression::ASTAssignmentExpression(ASTIndentifier *id, ASTExpression *val): id(id), val(val) {}
void ASTAssignmentExpression::print(int indentation) {
    print_indent(indentation);
    printf("AssignmentExpression\n");
    id->print(indentation + 1);
    val->print(indentation + 1);
}

ASTVariableDeclarator::ASTVariableDeclarator(ASTIndentifier *id, ASTExpression *init): id(id), init(init) {}
void ASTVariableDeclarator::print(int indentation) {
    print_indent(indentation);
    printf("VariableDeclarator\n");
    id->print(indentation + 1);
    if(init != NULL) {
        init->print(indentation + 1);
    }
}

ASTReturnStatement::ASTReturnStatement(ASTExpression *val): val(val) {}
void ASTReturnStatement::print(int indentation) {
    print_indent(indentation);
    printf("ReturnStatement\n");
    val->print(indentation + 1);
}

ASTVariableDeclarationStatement::ASTVariableDeclarationStatement(TokenType type, vector<ASTVariableDeclarator *> *declarators): type(type), declarators(declarators)  {}
void ASTVariableDeclarationStatement::print(int indentation) {
    print_indent(indentation);
    printf("VariableDeclarationStatement\n");
    print_indent(indentation + 1);
    print_typetoken(type);
    for(int i = 0; i < declarators->size(); i++) {
        declarators->operator[](i)->print(indentation + 1);
    }
}

ASTAssignmentStatement::ASTAssignmentStatement(ASTAssignmentExpression *expr): expr(expr) {}
void ASTAssignmentStatement::print(int indentation) {
    print_indent(indentation);
    printf("AssignmentStatement\n");
    expr->print(indentation + 1);
}

ASTWhileStatement::ASTWhileStatement(ASTExpression *test, vector<ASTStatement *> *body): test(test), body(body) {}
void ASTWhileStatement::print(int indentation) {
    print_indent(indentation);
    printf("WhileStatement\n");
    print_indent(indentation + 1);
    printf("Test\n");
    test->print(indentation + 2);
    print_indent(indentation + 1);
    printf("Body\n");
    for(int i = 0; i < body->size(); i++) {
        body->operator[](i)->print(indentation + 2);
    }
}

ASTProgram::ASTProgram(vector<ASTVariableDeclarationStatement *> *gdecls, vector<ASTStatement *> *main): gdecls(gdecls), main(main) {}
void ASTProgram::print(int indentation) {
    print_indent(indentation);
    printf("Program\n");
    for(int i = 0; i < gdecls->size(); i++) {
        gdecls->operator[](i)->print(indentation + 1);
    }
    print_indent(indentation + 1);
    printf("MainFunction\n");
    for(int i = 0; i < main->size(); i++) {
        main->operator[](i)->print(indentation + 2);
    }
}

void ASTEmptyStatement::print(int indentation) {
    print_indent(indentation);
    printf("EmptyStatement\n");
}
