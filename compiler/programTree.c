#include <stdbool.h>
#include <malloc.h>
#include <string.h>
#include "programTree.h"
#include "tokeniser.h"

PTNode* newPTNode(int type, void* data){
  PTNode* node = (PTNode*)malloc(sizeof(PTNode));
  node->type = type;
  node->data = data;
  node->next = NULL;
  node->parent = NULL;
  return node;
};

void codeGroupAdd(PTNode* codeGroup, PTNode *node){
  if(codeGroup->type != CODE_GROUP) {
    fprintf(stderr, "ERROR: codeGroupAdd(): PTNode* codeGroup must be a codeGroup");
    return;
  }
  node->parent = codeGroup;
  //if codeGroup.data is empty just add the node to the beginning
  if(codeGroup->data == NULL){
    codeGroup->data = node;
    return;
  }
  
  //append the node to the end of codeGroup.data which is a linked list
  PTNode *currentNode = (PTNode*)codeGroup->data;
  while(currentNode->next != NULL){
    currentNode = currentNode->next;
  }
  currentNode->next = node;
}

PTNode* generateTokenNode(FILE* stream, char* lastChar, Token* currentToken){
  //generate a TOKEN node
  //allocate a string large enough to fit the token
  char* data = malloc(sizeof(char)*(strlen(currentToken->str)+1));
  strcpy(data, currentToken->str);
  PTNode* node = newPTNode(TOKEN,data);
  return node;
}

PTNode* generateProgramTree(FILE* stream){
  PTNode* program = newPTNode(CODE_GROUP, NULL);

  Token currentToken;
  char lastChar = ' ';
  PTNode* codeGroup = program;
  while(1){
    currentToken = nextToken(stream,&lastChar);
    printf("{ %s, %d, %d }\n", currentToken.str, currentToken.type, currentToken.value);
    if(currentToken.str[0] == EOF) break;
    if(currentToken.str[0] == '{'){ //Generate a CODE_GROUP node and enter into it
      PTNode *node = newPTNode(CODE_GROUP, NULL);
      codeGroupAdd(codeGroup, node);
      codeGroup = node;
      continue;
    }
    if(currentToken.str[0] == '}'){// } signifies the end of the current code group
      codeGroup = codeGroup->parent;
      continue;
    }
    PTNode* node = generateTokenNode(stream, &lastChar,&currentToken);
    codeGroupAdd(codeGroup, node);
  }
  return program;
}

void printProgramTree(PTNode *root, int indentationLevel){
  PTNode* currentNode = root;
  while(1){
    for(int i = 0; i<indentationLevel; i++) printf(" ");
    switch(currentNode->type){
      case CODE_GROUP:
        printf("CODE_GROUP {\n");
        printProgramTree(currentNode->data, indentationLevel+2);
        for(int i = 0; i<indentationLevel; i++) printf(" ");
        printf("}\n");
      break;
      case TOKEN:
        printf("TOKEN { %s }\n", (char*)currentNode->data);
      break;
      default:
      printf("I dont know how to print this one\n");
      break;
    }
    if(currentNode->next == NULL) break;
    currentNode = currentNode->next;
  }
};
