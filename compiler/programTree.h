#include <stdio.h>

//program tree nodes
enum PTNODE_TYPES{
  CODE_GROUP,
  TOKEN
};

typedef struct PTNode{
  int type;
  struct PTNode* next;
  struct PTNode* parent;
  void* data;
}PTNode;
typedef struct FunctionDeclerationData{
  int returnType;
  char* name;
  PTNode* code;
}FunctionDeclerationData;

PTNode* generateProgramTree(FILE* stream);
void printProgramTree(PTNode *root, int indentationLevel);
