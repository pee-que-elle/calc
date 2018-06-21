#include "ast.h"

ASTNode_T *create_node();

ASTNode_T *evaluate_node(ASTNode_T *node); /* parent function, will evalute a node */
    ASTNode_T *evaluate_operator(Operator_T *operator);
        ASTNode_T *evaluate_unary(UnaryOperator_T *operator);
        ASTNode_T *evaluate_binary(BinaryOperator_T *operator);

    ASTNode_T *evaluate_functioncall(FunctionCall_T *call);
