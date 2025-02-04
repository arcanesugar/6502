#include <stdarg.h>
#include <stdio.h>

#include "programTree.h"
#include "expression.h"

void printWithIndent(int indent, char* fmt, ...){
  for(int i = 0; i<indent; i++) printf(" ");
  va_list argptr;
  va_start(argptr,fmt);
  vprintf(fmt, argptr);
  va_end(argptr);
}

void printExpression(Expression* expression, int indentationLevel);

void printFunctionCall(Expression* expression, int indent){
  ExprFunctionCall* fcd = expression->value;
  printWithIndent(indent, "FUNCTION_CALL{\n");
  printWithIndent(indent+2, "name: %s\n", fcd->name);
  printWithIndent(indent+2, "argument:\n");
  if(fcd->argument != NULL) {printExpression(fcd->argument, indent+4);}
  else{
    printWithIndent(indent, "none");
  }
  printWithIndent(indent,"}\n");
}

void printOperation(Expression* expression, int indent){
  ExprOperation* operation = expression->value;
  printWithIndent(indent, "OPERATION{\n");

  printWithIndent(indent+2, "leftOperand:\n");
  printExpression(operation->rightOperand, indent+4);

  printWithIndent(indent+2, "rightOperand:\n");
  printExpression(operation->rightOperand, indent+4);

  printWithIndent(indent+2, "operator: %c\n",operation->operator);
  printWithIndent(indent, "}\n");
}

void printExpression(Expression* expression, int indent){
  switch(expression->type){
    case EXPR_FUNCTION_CALL:
      printFunctionCall(expression, indent);
      break;
    case EXPR_BYTE_LITERAL:
      printWithIndent(indent,"%d\n", *(char*)expression->value);
      break;
    case EXPR_OPERATION:
      printOperation(expression, indent);
      break;
    default:
      printWithIndent(indent,"I cant print this expression\n");
      break;
  }
}

void printCodeGroup(PTNode *node, int indent);

void printFunctionDecleration(PTNode* node, int indent){
  FunctionDeclerationData* fdd = (FunctionDeclerationData*)node->data;
  printWithIndent(indent, "FUNCTION_DECLERATION { \n");
  printWithIndent(indent+2, "name: %s\n", fdd->name);
  printWithIndent(indent+2, "return: (todo: make this work) \n");
  printWithIndent(indent+2, "code:\n");
  printCodeGroup(fdd->code, indent+4);
  printWithIndent(indent, "}\n");
}

void printCodeGroup(PTNode *node, int indent){
  printWithIndent(indent, "CODE_GROUP {\n");
  PTNode* currentNode = node->data;
  while(1){
    if(currentNode == NULL) break;
    switch(currentNode->type){
      case NODE_CODE_GROUP:
        printCodeGroup(currentNode->data, indent+2);
        break;
      case NODE_FUNCTION_DECLERATION:
        printFunctionDecleration(currentNode, indent+2);
        break;
      case NODE_EXPRESSION:
        printExpression(currentNode->data, indent+2);
        break;
      default:
        printWithIndent(indent+2,"I dont know how to print this one\n");
        break;
    }
    currentNode = currentNode->next;
  }
  printWithIndent(indent,"}\n");
};

void printProgramTree(PTNode *root){
  printCodeGroup(root,0);
}
