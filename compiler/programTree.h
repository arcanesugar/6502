#include <stdio.h>

//program tree nodes
enum PTNODE_TYPES{
  CODE_GROUP,
  FUNCTION_DECLARATION,
  TOKEN
};

typedef struct PTNode{
  int type;
  struct PTNode* next;
  struct PTNode* parent;
  void* data;
}PTNode;


PTNode* generateProgramTree(FILE* stream);
void printProgramTree(PTNode *root, int indentationLevel);
