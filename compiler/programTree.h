#include <stdio.h>

//program tree nodes
enum PTNODE_TYPES{
  NODE_CODE_GROUP,
  NODE_FUNCTION_DECLERATION,
  NODE_EXPRESSION
};

typedef struct PTNode{
  int type;
  void* data;
  struct PTNode* next;
}PTNode;

typedef struct FunctionDeclerationData{
  int returnType;
  int argumentType;
  char* name;
  PTNode* code;
}FunctionDeclerationData;

enum EXPRESSION_TYPES{
  EXPR_OPERATION,
  EXPR_BYTE_LITERAL,
  EXPR_VARIABLE,
  EXPR_FUNCTION_CALL
};

typedef struct ExpressionData{
  int type;
  void* value;
}ExpressionData;

typedef struct ExprOperation{
  PTNode* leftOperand;
  PTNode* rightOperand;
  char operator;
}ExprOperation;

typedef struct ExprFunctionCall{
  char* name;
  PTNode* argument;
}ExprFunctionCall;

typedef struct ExprVariable{ //struct is kind of unnecissary
  char* name;
}ExprVariable;

PTNode* generateProgramTree(FILE* stream);
void printProgramTree(PTNode *root, int indentationLevel);
