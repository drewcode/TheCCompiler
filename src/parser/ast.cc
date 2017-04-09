#include "ast.h"
#include "../symbol_table/sym_tab.h"
#include <stdio.h>

struct ICG_state {
    int next_temp;
    int next_label;
    int is_left;
}  ICG_state;

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

void ASTIndentifier::print(int indentation, FILE *icg_f) {
    print_indent(indentation);
    printf("Identifier %d\n", index);
    if(!ICG_state.is_left) {
        fprintf(icg_f, "T%d = i%d\n", ICG_state.next_temp, index);
        ICG_state.next_temp = ICG_state.next_temp + 1;
    }
}


ASTLiteral::ASTLiteral(LiteralType type, void * val): type(type), val(val) {}
void ASTLiteral::print(int indentation, FILE *icg_f) {
    print_indent(indentation);
    printf("Literal\n");
    print_indent(indentation + 1);
    if(type == L_CHAR) {
        printf("Char: %c\n", *(char *)val);
        fprintf(icg_f, "T%d = %c\n", ICG_state.next_temp, *(char *)val);
    } else if(type == L_INT) {
        printf("Int: %d\n", *(int *)val);
        fprintf(icg_f, "T%d = %d\n", ICG_state.next_temp, *(int *)val);
    } else if(type == L_DOUBLE) {
        printf("Double: %f\n", *(double *)val);
        fprintf(icg_f, "T%d = %f\n", ICG_state.next_temp, *(double *)val);
    }
    ICG_state.next_temp = ICG_state.next_temp + 1;

}

ASTBinaryExpression::ASTBinaryExpression(TokenType op, ASTExpression *left, ASTExpression *right): op(op), left(left), right(right) {}
void ASTBinaryExpression::print(int indentation, FILE *icg_f) {
    print_indent(indentation);
    printf("BinaryExpression\n");
    print_indent(indentation + 1);
    printf("%d\n", op);
    left->print(indentation + 1, icg_f);
    int left_temp = ICG_state.next_temp - 1;
    right->print(indentation + 1, icg_f);
    fprintf(icg_f, "T%d = T%d op%d T%d\n", ICG_state.next_temp, left_temp, op, ICG_state.next_temp - 1);
    ICG_state.next_temp = ICG_state.next_temp + 1;  
}

ASTAssignmentExpression::ASTAssignmentExpression(ASTIndentifier *id, ASTExpression *val): id(id), val(val) {}
void ASTAssignmentExpression::print(int indentation, FILE *icg_f) {
    print_indent(indentation);
    printf("AssignmentExpression\n");
    ICG_state.is_left = 1;
    id->print(indentation + 1, icg_f);
    ICG_state.is_left = 0;
    val->print(indentation + 1, icg_f);
    fprintf(icg_f, "i%d = T%d\n", id->index, ICG_state.next_temp - 1);
}

ASTVariableDeclarator::ASTVariableDeclarator(ASTIndentifier *id, ASTExpression *init): id(id), init(init) {}
void ASTVariableDeclarator::print(int indentation, FILE *icg_f) {
    print_indent(indentation);
    printf("VariableDeclarator\n");
    ICG_state.is_left = 1;
    id->print(indentation + 1, icg_f);
    ICG_state.is_left = 0;
    if(init != NULL) {
        init->print(indentation + 1, icg_f);

        fprintf(icg_f, "i%d = T%d\n",  id->index, ICG_state.next_temp - 1);
    }


    
}

ASTReturnStatement::ASTReturnStatement(ASTExpression *val): val(val) {}
void ASTReturnStatement::print(int indentation, FILE *icg_f) {
    print_indent(indentation);
    printf("ReturnStatement\n");
    val->print(indentation + 1, icg_f);
}

ASTVariableDeclarationStatement::ASTVariableDeclarationStatement(TokenType type, vector<ASTVariableDeclarator *> *declarators): type(type), declarators(declarators)  {}
void ASTVariableDeclarationStatement::print(int indentation, FILE *icg_f) {
    print_indent(indentation);
    printf("VariableDeclarationStatement\n");
    print_indent(indentation + 1);
    print_typetoken(type);
    for(int i = 0; i < declarators->size(); i++) {
        declarators->operator[](i)->print(indentation + 1, icg_f);
    }
}

ASTAssignmentStatement::ASTAssignmentStatement(ASTAssignmentExpression *expr): expr(expr) {}
void ASTAssignmentStatement::print(int indentation, FILE *icg_f) {
    print_indent(indentation);
    printf("AssignmentStatement\n");
    expr->print(indentation + 1, icg_f);
}

ASTWhileStatement::ASTWhileStatement(ASTExpression *test, vector<ASTStatement *> *body): test(test), body(body) {}
void ASTWhileStatement::print(int indentation, FILE *icg_f) {
    print_indent(indentation);
    printf("WhileStatement\n");
    print_indent(indentation + 1);
    printf("Test\n");
    test->print(indentation + 2, icg_f);
    int begin_label = ICG_state.next_label;
    int end_label = ICG_state.next_label + 1;
    ICG_state.next_label += 2;
    fprintf(icg_f, "L%d: \nifFalse T%d goto L%d\n", begin_label, ICG_state.next_temp-1, end_label);
    
    print_indent(indentation + 1);
    printf("Body\n");
    for(int i = 0; i < body->size(); i++) {
        body->operator[](i)->print(indentation + 2, icg_f);
    }

    fprintf(icg_f, "goto L%d \nL%d:\n", begin_label, end_label);

}

ASTProgram::ASTProgram(vector<ASTVariableDeclarationStatement *> *gdecls, vector<ASTStatement *> *main): gdecls(gdecls), main(main) {}
void ASTProgram::print(int indentation, FILE *icg_f) {
    ICG_state.next_temp = 0;
    ICG_state.next_label = 0;
    ICG_state.is_left = 0;

    print_indent(indentation);
    printf("Program\n");
    for(int i = 0; i < gdecls->size(); i++) {
        gdecls->operator[](i)->print(indentation + 1, icg_f);
    }
    print_indent(indentation + 1);
    printf("MainFunction\n");
    for(int i = 0; i < main->size(); i++) {
        main->operator[](i)->print(indentation + 2, icg_f);
    }
}

void ASTEmptyStatement::print(int indentation, FILE *icg_f) {
    print_indent(indentation);
    printf("EmptyStatement\n");
}
