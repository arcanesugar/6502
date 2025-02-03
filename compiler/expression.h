#pragma once
#include "tokeniser.h"

enum EXPRESSION_TYPES{
  EXPR_OPERATION,
  EXPR_BYTE_LITERAL,
  EXPR_VARIABLE,
  EXPR_FUNCTION_CALL
};

typedef struct Expression{
  int type;
  void* value;
}Expression;

typedef struct ExprOperation{
  Expression* leftOperand;
  Expression* rightOperand;
  char operator;
}ExprOperation;

typedef struct ExprFunctionCall{
  char* name;
  Expression* argument;
}ExprFunctionCall;

typedef struct ExprVariable{ //struct is kind of unnecissary
  char* name;
}ExprVariable;

Expression* parseExpression(FILE* stream, char* lastChar, Token* currentToken);
