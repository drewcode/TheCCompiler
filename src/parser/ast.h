#ifndef CCOMPILER_AST_H
#define CCOMPILER_AST_H

#include "../lexer/token.h"

#include <vector>

using std::vector;

class ASTNode {
    public:
        virtual void print(int) = 0;
};




//ASTExpression
class ASTExpression : public ASTNode {
    public:
        virtual void print(int) = 0;
};

class ASTIndentifier : public ASTExpression {
    public:
        int index;
        ASTIndentifier(int);
        void print(int);

};

class ASTLiteral : public ASTExpression {
    public:
        void *val;
        LiteralType type;
        ASTLiteral(LiteralType, void *);
        void print(int);
};

class ASTBinaryExpression : public ASTExpression {
    public:
        TokenType op;
        ASTExpression *left;
        ASTExpression *right;
        ASTBinaryExpression(TokenType, ASTExpression *, ASTExpression *);
        void print(int);
};

class ASTAssignmentExpression : public ASTExpression {
    public:
        ASTIndentifier *id;
        ASTExpression *val;
        ASTAssignmentExpression(ASTIndentifier *, ASTExpression *);
        void print(int);
};



class ASTVariableDeclarator : public ASTNode {
    public:
        ASTIndentifier *id;
        ASTExpression *init;
        ASTVariableDeclarator(ASTIndentifier *, ASTExpression *);
        void print(int);
};




//ASTStatement
class ASTStatement : public ASTNode {
    public:
        virtual void print(int) = 0;
};

class ASTEmptyStatement : public  ASTStatement {
    public:
        void print(int);
};

class ASTReturnStatement : public ASTStatement {
    public:
        ASTExpression *val;
        ASTReturnStatement(ASTExpression *);
        void print(int);
};

class ASTVariableDeclarationStatement : public ASTStatement {
    public:
        TokenType type;
        vector<ASTVariableDeclarator *> *declarators;
        ASTVariableDeclarationStatement(TokenType, vector<ASTVariableDeclarator *> *);
        void print(int);
};





class ASTAssignmentStatement : public ASTStatement {
    public:
        ASTAssignmentExpression *expr;
        ASTAssignmentStatement(ASTAssignmentExpression *);
        void print(int);
};

class ASTWhileStatement : public ASTStatement {
    public:
        ASTExpression *test;
        vector<ASTStatement *> *body;
        ASTWhileStatement(ASTExpression *, vector<ASTStatement *> *);
        void print(int);
};


class ASTProgram : public ASTNode {
    public:
        vector<ASTVariableDeclarationStatement *> *gdecls;
        vector<ASTStatement *> *main;

        ASTProgram(vector<ASTVariableDeclarationStatement *> *, vector<ASTStatement *> *);
        void print(int);
};

#endif
