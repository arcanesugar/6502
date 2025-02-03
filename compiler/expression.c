#include <malloc.h>
#include <string.h>
#include "expression.h"
#include "error.h"

Expression* generateFunctionCall(FILE* stream, char* lastChar, Token* currentToken);
Expression* generateOperationNode(Expression* leftOperand, char operator, FILE* stream, char* lastChar, Token* currentToken);

Expression* parseExpression(FILE* stream, char* lastChar, Token* currentToken){
  //this is the most complicated one, expressions are basically their own smaller language
  //assumes that currentToken is the first token of the expression
  Expression* expr = malloc(sizeof(Expression));
  if(currentToken->type == TOK_NAME){
    char* name = malloc(sizeof(char)*(strlen(currentToken->str)+1));
    strcpy(name, currentToken->str);

    *currentToken = getNextToken(stream, lastChar);
    expectTokenType(currentToken, TOK_CHAR, 0);
    if(currentToken->str[0] == '('){
      expr->type = EXPR_FUNCTION_CALL;
      expr->value = malloc(sizeof(ExprFunctionCall));
      ExprFunctionCall* value = expr->value;
      value->name = name;
      value->argument = NULL;
      *currentToken = getNextToken(stream, lastChar);
      if(currentToken->str[0] != ')'){
        value->argument = parseExpression(stream, lastChar, currentToken);
      }
      expectTokenType(currentToken, TOK_CHAR, ')');
      *currentToken = getNextToken(stream, lastChar);
    }else{
      expr->type = EXPR_VARIABLE;
      expr->value = malloc(sizeof(ExprVariable));
      ExprVariable* value = expr->value;
      value->name = name;
    }
  }else{
    if(currentToken->type == TOK_BYTELIT){
      expr->type = EXPR_BYTE_LITERAL;
      expr->value = malloc(sizeof(char));
      *(char*)expr->value = currentToken->value;
      *currentToken = getNextToken(stream, lastChar);
    }else{
      raiseError("unexpected token when generating expression node");
    }
  }
  switch(currentToken->str[0]){
    case '+':
    case '-':
      return generateOperationNode(expr, currentToken->str[0], stream, lastChar, currentToken);
    break;
    case ')':
    case ';':
      return expr;
    break;
    default:
      raiseError("unexpected token when generating expression node");
      return expr;
    break;
  }
}

Expression* generateOperationNode(Expression* leftOperand, char operator, FILE* stream, char* lastChar, Token* currentToken){
  *currentToken = getNextToken(stream, lastChar);
  Expression* expr = malloc(sizeof(Expression));
  expr->type = EXPR_OPERATION;
  expr->value = malloc(sizeof(ExprOperation));
  ExprOperation* operationData = expr->value;
  operationData->operator = operator;
  operationData->leftOperand = leftOperand;
  operationData->rightOperand = parseExpression(stream, lastChar, currentToken);
  return expr;
}
