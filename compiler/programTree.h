#pragma once
#include <stdio.h>

//program tree nodes
enum PTNODE_TYPES{
  N_CODE_GROUP,
  N_FUNCTION_DEFINITION,

  //statements
  N_VARIABLE_DECLARATION,
  N_VARIABLE_ASSIGNMENT,

  //expressions
  N_FUNCTION_CALL,//could also be a statement
  N_OPERATION,
  N_VARIABLE,
  N_LITERAL
};

//time to pollute the namespace
typedef struct PTNode{
  int type;
  void* data;
  struct PTNode* next;
}PTNode;

typedef struct FunctionDefinition{
  char* name;
  int returnType;
  PTNode* argument;//variable declerations
  PTNode* code;
}FunctionDefinition;

typedef struct VariableDecleration{
  char* name;
  int type;
  PTNode* startValue;
}VariableDecleration;

typedef struct VariableAssignment{
  char* name;
  PTNode* value;
}VariableAssignment;

typedef struct FunctionCall{
  char* name;
  PTNode* argument;//only one argument is supported right now
}FunctionCall;

typedef struct Operation{
  PTNode* left;
  PTNode* right;
  char operator;
}Operation;

//no struct is neccisarry for code group, variable or literal

PTNode* generateProgramTree(FILE* stream);
