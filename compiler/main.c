#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <malloc.h>

typedef struct Token{
  char str[100];
  bool eof;
}Token;

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

bool isSeperator(char c){
  if(isspace(c)) return true;
  char seperators[] = "(){};";
  if(strchr(seperators,c) != NULL) return true;
  return false;
}

Token nextToken(FILE* stream, char* lastChar){
  while(isspace(*lastChar)){*lastChar = fgetc(stream);}//eat whitespace
  Token token;
  if(*lastChar == EOF) {token.eof = true; return token;}
  token.eof = false;
  int tokenLen = 0;
  token.str[tokenLen++] = *lastChar;
  //hacky solution to make seperators into tokens
  if(isSeperator(*lastChar)) {token.str[tokenLen] = 0; *lastChar = fgetc(stream); return token;}
  while(1){
    *lastChar = fgetc(stream);
    //EOF token is only returned if eof is immidiately recived from the stream,
    //otherwise it acts as a seperator
    if(isSeperator(*lastChar) || *lastChar == EOF) break;
    token.str[tokenLen++] = *lastChar;
  }
  token.str[tokenLen++] = 0;
  return token;
};

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
};

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

void compile(char* inputFilename, char* outputFilename){
  FILE* inputFile = fopen(inputFilename,"r");
  FILE* outputFile = fopen(outputFilename,"w");
  PTNode* program = newPTNode(CODE_GROUP, NULL);

  Token currentToken;
  char lastChar = ' ';
  PTNode* codeGroup = program;
  while(1){
    currentToken = nextToken(inputFile,&lastChar);
    if(currentToken.eof == true) break;

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

    //generate a TOKEN node
    //allocate a string large enough to fit the token
    char* data = malloc(sizeof(char)*(strlen(currentToken.str)+1));
    strcpy(data, currentToken.str);
    PTNode* node = newPTNode(TOKEN,data);
    codeGroupAdd(codeGroup, node);
  }
  fclose(inputFile);
  fclose(outputFile);
  printProgramTree(program,0);
}

int main(int argc, char* argv[]){
  compile("source.txt", "out.txt");
  return 0;
}
