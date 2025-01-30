#include <stdio.h>

//program tree nodes
enum PTNODE_TYPES{
  CODE_GROUP,
  FUNCTION_DECLERATION,
  FUNCTION_CALL,
  TOKEN
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

typedef struct FunctionCallData{
  char* name;
  char argument;
}FunctionCallData;

PTNode* generateProgramTree(FILE* stream);
void printProgramTree(PTNode *root, int indentationLevel);
