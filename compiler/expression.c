#include <malloc.h>
#include <string.h>
#include "expression.h"
#include "error.h"

ExprFunctionCall* parseFunctionCall(char* name, FILE* stream, Token* currentToken);
Expression* generateOperationNode(Expression* leftOperand, char operator, FILE* stream, Token* currentToken);

Expression* parseExpression(FILE* stream, Token* currentToken){
  //assumes that currentToken is the first token of the expression
  Expression* expr = malloc(sizeof(Expression));
  char* name;
  switch(currentToken->type){
    case TOK_NAME:
      name = malloc(sizeof(char)*(strlen(currentToken->str)+1));
      strcpy(name, currentToken->str);
      *currentToken = getNextToken(stream);
      expectTokenType(currentToken, TOK_CHAR, 0);
      if(currentToken->str[0] == '('){
        expr->type = EXPR_FUNCTION_CALL;
        expr->value = parseFunctionCall(name, stream, currentToken);
      }
      else{
        expr->type = EXPR_VARIABLE;
        expr->value = name;
      }
      break;
    case TOK_BYTELIT:
      expr->type = EXPR_BYTE_LITERAL;
      expr->value = malloc(sizeof(char));
      *(char*)expr->value = currentToken->value;
      *currentToken = getNextToken(stream);
      break;
    default:
      raiseError("unexpected token when generating expression node");
      break;
  }

  switch(currentToken->str[0]){
    case '+':
    case '-':
      return generateOperationNode(expr, currentToken->str[0], stream, currentToken);
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

Expression* generateOperationNode(Expression* leftOperand, char operator, FILE* stream, Token* currentToken){
  *currentToken = getNextToken(stream);
  Expression* expr = malloc(sizeof(Expression));
  expr->type = EXPR_OPERATION;
  expr->value = malloc(sizeof(ExprOperation));
  ExprOperation* operationData = expr->value;
  operationData->operator = operator;
  operationData->leftOperand = leftOperand;
  operationData->rightOperand = parseExpression(stream, currentToken);
  return expr;
}

ExprFunctionCall* parseFunctionCall(char* name, FILE* stream, Token* currentToken){
  //expects that currentToken is (
  ExprFunctionCall* call = malloc(sizeof(ExprFunctionCall));
  
  call->name = name;
  call->argument = NULL;
  *currentToken = getNextToken(stream);
  if(currentToken->str[0] != ')'){
    call->argument = parseExpression(stream, currentToken);
  }
  expectTokenType(currentToken, TOK_CHAR, ')');
  *currentToken = getNextToken(stream);

  return call;
}
