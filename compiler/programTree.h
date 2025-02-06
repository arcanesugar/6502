#pragma once
#include <stdio.h>

//program tree nodes
enum PTNODE_TYPES{
  NODE_CODE_GROUP,
  NODE_FUNCTION_DECLERATION,
  NODE_VARIABLE_DECLERATION,
  NODE_VARIABLE_ASSIGNMENT,
  NODE_EXPRESSION,
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

PTNode* generateProgramTree(FILE* stream);
