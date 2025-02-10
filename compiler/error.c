#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#include "error.h"
#include "lexer.h"


char* getTokenTypeName(int type){
  switch(type){
    case TOK_NAME:
      return "NAME";
    case TOK_TYPE:
      return "TYPE";
    case TOK_CHAR:
      return "CHAR";
    case TOK_BYTELIT:
      return "BYTELIT";
    default:
      return "<token type name missing>";
  }
};

char* strToHeap(char* str){
  char* copy = malloc(sizeof(char) * (strlen(str)+1));
  strcpy(copy, str);
  return copy;
}

void expectTokenType(Token token, int type, char c){
  if(type == TOK_CHAR && c != 0){
    if(token.value != c || token.type != type)
      raiseError("expected %c token, got \"%s\"\n",c,token.str);
  }
  if(token.type != type)
    raiseError("expected %s token, got \"%s\"\n", getTokenTypeName(type), token.str);
};

void raiseError(char* fmt, ...){
  fprintf(stderr, "[ERROR] ");
  va_list argptr;
  va_start(argptr,fmt);
  vfprintf(stderr, fmt, argptr);
  va_end(argptr);
  exit(EXIT_FAILURE);
}

static char* typeToStr(int type){
  switch(type){
    case TYPE_BYTE:
      return "BYTE";
    case TYPE_NONE:
      return "NONE";
    default:
      return "UNKNOWN_TYPE";
  }
}

void printProgramTree(FILE* stream, PTNode* root);
static void printPTNode(PTNode* node, int indent);
static void printVariableDecleration(PTNode* node, int indent);
static void printVariableAssignment(PTNode* node, int indent);
static void printCodeGroup(PTNode* node, int indent);
static void printFunctionDefinition(PTNode* node, int indent);
static void printFunctionCall(PTNode* node, int indent);
static void printLiteral(PTNode* node, int indent);

static FILE* outputStream = NULL;

static void printWithIndent(int indent, char* fmt, ...){
  for(int i = 0; i<indent; i++) fprintf(outputStream, " ");
  va_list argptr;
  va_start(argptr,fmt);
  vfprintf(outputStream, fmt, argptr);
  va_end(argptr);
}

void printProgramTree(FILE* stream, PTNode* root){
  outputStream = stream;
  if(outputStream == NULL){
    outputStream = stdout;
  }
  PTNode* node = root->data;
  while(node != NULL){
    printPTNode(node, 0);
    node = node->next;
  }
}

void printPTNode(PTNode* node, int indent){
  if(node == NULL){ 
    printWithIndent(indent, "(null)\n"); 
    return;
  }
  switch(node->type){
    case N_LITERAL:
      printLiteral(node, indent);
    break;
    case N_CODE_GROUP:
      printCodeGroup(node, indent);
    break;
    case N_VARIABLE_DECLARATION:
      printVariableDecleration(node, indent);
    break;
    case N_VARIABLE_ASSIGNMENT:
      printVariableAssignment(node, indent);
    break;
    case N_FUNCTION_DEFINITION:
      printFunctionDefinition(node, indent);
      break;
    case N_FUNCTION_CALL:
      printFunctionCall(node, indent);
      break;
    default:
      printWithIndent(indent, "I can't print this one\n");
    break;
  }
}

void printVariableDecleration(PTNode* node, int indent){
  VariableDecleration* vd = node->data;
  printWithIndent(indent, "<variable_assignment> {\n");
  printWithIndent(indent+2, "type: %s\n", typeToStr(vd->type));
  printWithIndent(indent+2, "name: %s\n", vd->name);
  printWithIndent(indent+2, "startValue:\n");
  printPTNode(vd->startValue, indent+4);
  printWithIndent(indent, "}\n");
}

void printVariableAssignment(PTNode* node, int indent){
  VariableAssignment* va = node->data;
  printWithIndent(indent, "<variable_assignment> {\n");
  printWithIndent(indent+2, "name: %s\n", va->name);
  printWithIndent(indent+2, "value:\n");
  printPTNode(va->value, indent+4);
  printWithIndent(indent, "}\n");
}

void printFunctionCall(PTNode* node, int indent){
  FunctionCall* fc = node->data;
  printWithIndent(indent, "<function_call> {\n");
  printWithIndent(indent+2, "name: %s\n", fc->name);
  printWithIndent(indent+2, "argument:\n");
  printPTNode(fc->argument, indent+4);
  printWithIndent(indent, "}\n");
}

void printFunctionDefinition(PTNode* node, int indent){
  FunctionDefinition* fd = node->data;
  printWithIndent(indent, "<function_definition> {\n");
  printWithIndent(indent+2, "return: %s\n", typeToStr(fd->returnType));
  printWithIndent(indent+2, "name: %s\n", fd->name);
  printWithIndent(indent+2, "argument: \n");
  printPTNode(fd->argument, indent+4);
  printWithIndent(indent+2, "code:\n");
  printPTNode(fd->code, indent + 4);
  printWithIndent(indent, "}\n");
}

void printCodeGroup(PTNode* node, int indent){
  printWithIndent(indent, "<code_group> {\n");
  node = node->data;
  while(node != NULL){
    printPTNode(node, indent+2);
    node = node->next;
  }
  printWithIndent(indent, "}\n");
}

void printLiteral(PTNode* node, int indent){
  printWithIndent(indent, "%d\n", *(int*)node->data);
}
