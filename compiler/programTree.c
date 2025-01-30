#include <stdbool.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "programTree.h"
#include "tokeniser.h"

static void raiseError(char* fmt, ...){
  fprintf(stderr, "[ERROR - programTree] ");
  va_list argptr;
  va_start(argptr,fmt);
  vfprintf(stderr, fmt, argptr);
  va_end(argptr);
  exit(EXIT_FAILURE);
}

static inline void expectTokenType(Token* token, int type, char c){//c only used if CHAR_TOK and it != 0
  if(type == CHAR_TOK && c != 0){
    if(token->str[0] != c)
      raiseError(" expected %c token, got \"%s\"\n",c,token->str);
    return;
  }
  if(token->type != type)
    raiseError(" expected %s token, got \"%s\"\n", getTokenTypeName(type), token->str);
};

PTNode* newPTNode(int type, void* data){
  PTNode* node = (PTNode*)malloc(sizeof(PTNode));
  node->type = type;
  node->data = data;
  node->next = NULL;
  return node;
};

void codeGroupAdd(PTNode* codeGroup, PTNode *node){
  if(codeGroup->type != CODE_GROUP) {
    raiseError(" in function (codeGroupAdd) PTNode* codeGroup is not a CODE_GROUP\n");
    return;
  }
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

PTNode* generateCodeGroup(FILE* stream, char* lastChar, Token* currentToken);

PTNode* generateDeclerationNode(FILE* stream, char* lastChar, Token* currentToken){
  //assume that currentToken is a "type" token
  
  int type = currentToken->value;
  *currentToken = nextToken(stream, lastChar);
  expectTokenType(currentToken, NAME_TOK, 0);
  
  char* name = malloc((strlen(currentToken->str)+1)*sizeof(char));
  strcpy(name, currentToken->str);
  
  *currentToken = nextToken(stream, lastChar);
  expectTokenType(currentToken, CHAR_TOK, 0);// = ( and ; all valid
  
  if(currentToken->str[0] == '('){//function decleration
    PTNode* node = newPTNode(FUNCTION_DECLERATION,NULL);
    FunctionDeclerationData* data = (FunctionDeclerationData*)malloc(sizeof(FunctionDeclerationData));
    data->name = name;
    data->returnType = type;
    data->argumentType = NONE;

    while(currentToken->str[0] != ')'){
      *currentToken = nextToken(stream, lastChar);
      if(currentToken->type == TYPE_TOK){
        data->argumentType = currentToken->value;
      }
    }

    *currentToken = nextToken(stream, lastChar);//eat '{' token
    expectTokenType(currentToken, CHAR_TOK, '{');
    data->code = generateCodeGroup(stream, lastChar, currentToken);
    node->data = data;
    return node;
  }
  free(name);
  return NULL;
}

PTNode* generateNameUse(FILE* stream, char* lastChar, Token* currentToken){
  //assume that currentToken is a "name" token
  
  char* name = malloc((strlen(currentToken->str)+1)*sizeof(char));
  strcpy(name, currentToken->str);

  *currentToken = nextToken(stream, lastChar);
  expectTokenType(currentToken,CHAR_TOK, 0);

  if(currentToken->str[0] == '('){//function call 
    PTNode* node = newPTNode(FUNCTION_CALL,NULL);
    FunctionCallData* data = malloc(sizeof(FunctionCallData));
    data->name = name;

    while(currentToken->str[0] != ')'){
      *currentToken = nextToken(stream, lastChar);
      if(currentToken->type == CHARLIT_TOK || currentToken->type == NUMLIT_TOK){
        data->argument = currentToken->value;
      }
    };
    *currentToken = nextToken(stream, lastChar);//eat ';' token
    expectTokenType(currentToken,CHAR_TOK, ';');
    
    node->data = data;
    return node;
  };
  free(name);
  raiseError("unexpected token (generateNameUse)\n");
  return NULL;
}

PTNode* generateCodeGroup(FILE* stream, char* lastChar, Token* currentToken){
  PTNode* codeGroup = newPTNode(CODE_GROUP,NULL);
  while(1){
    *currentToken = nextToken(stream, lastChar);
    PTNode* node;
    if(currentToken->str[0] == EOF) break;
    if(currentToken->str[0] == '}') break; // closing brace indicates the end of the group
    switch(currentToken->type){
      case TYPE_TOK://function declerations are invalid here, but variable declerations are valid
        node = generateDeclerationNode(stream, lastChar, currentToken);
      break;
      case NAME_TOK:
        node = generateNameUse(stream, lastChar, currentToken);
      break;
      default:
        node = generateTokenNode(stream, lastChar, currentToken);
      break;
    };
    codeGroupAdd(codeGroup,node);
  }
  return codeGroup;
}

PTNode* generateProgramTree(FILE* stream){
  PTNode* program = newPTNode(CODE_GROUP, NULL);
  Token currentToken;
  char lastChar = ' ';
  while(1){
    currentToken = nextToken(stream,&lastChar);
    if(currentToken.str[0] == EOF) break;
    
    PTNode* node;
    switch(currentToken.type){
      case TYPE_TOK:
        node = generateDeclerationNode(stream, &lastChar, &currentToken);
        break;
      default:
        node = generateTokenNode(stream, &lastChar,&currentToken);
        break;
    }
    codeGroupAdd(program, node);
  }
  return program;
}

void printFunctionDecleration(PTNode* currentNode, int indentationLevel){
  FunctionDeclerationData* fdd = (FunctionDeclerationData*)currentNode->data;
  printf("FUNCTION_DECLERATION { \n");
  for(int i = 0; i<indentationLevel+2; i++) printf(" ");
  printf("name: %s\n", fdd->name);
  for(int i = 0; i<indentationLevel+2; i++) printf(" ");
  printf("return: (todo: make this work) \n");
  for(int i = 0; i<indentationLevel+2; i++) printf(" ");
  printf("code:\n");
  printProgramTree(fdd->code, indentationLevel+4);
  for(int i = 0; i<indentationLevel+2; i++) printf(" ");
  printf("}\n");
}

void printFunctionCall(PTNode* currentNode, int indentationLevel){
  FunctionCallData* fcd = (FunctionCallData*)currentNode->data;
  printf("FUNCTION_CALL{ \n");
  for(int i = 0; i<indentationLevel+2; i++) printf(" ");
  printf("name: %s\n", fcd->name);
  for(int i = 0; i<indentationLevel+2; i++) printf(" ");
  printf("argument: %d\n", fcd->argument);
  for(int i = 0; i<indentationLevel; i++) printf(" ");
  printf("}\n");
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
      case FUNCTION_DECLERATION:
        printFunctionDecleration(currentNode, indentationLevel);
      break;
      case FUNCTION_CALL:
        printFunctionCall(currentNode, indentationLevel);
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
