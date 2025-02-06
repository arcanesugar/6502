#include <stdbool.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

#include "error.h"
#include "programTree.h"
#include "expression.h"
#include "tokeniser.h"

static PTNode* parseCodeGroup(FILE* stream, Token* currentToken);
static PTNode* parseFunctionDecleration(int returnType, char* name, FILE* stream, Token* currentToken);
static PTNode* parseDecleration(FILE* stream, Token* currentToken);

static PTNode* newPTNode(int type, void* data){
  PTNode* node = (PTNode*)malloc(sizeof(PTNode));
  node->type = type;
  node->data = data;
  node->next = NULL;
  return node;
};

static void codeGroupAdd(PTNode* codeGroup, PTNode *node){
  if(codeGroup->type != NODE_CODE_GROUP) {
    raiseError("codeGroupAdd(): PTNode* codeGroup is not a CODE_GROUP\n");
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

PTNode* generateProgramTree(FILE* stream){
  PTNode* program = newPTNode(NODE_CODE_GROUP, NULL);
  Token currentToken;
  while(1){
    currentToken = getNextToken(stream);
    ungetToken(currentToken);
    currentToken = getNextToken(stream);
    if(currentToken.type == TOK_EOF) break;
    
    PTNode* node;
    switch(currentToken.type){
      case TOK_TYPE:
        node = parseDecleration(stream, &currentToken);
        break;
      default:
        raiseError("(generateProgramTree) unexpected token\n");
    }
    codeGroupAdd(program, node);
  }
  return program;
}


PTNode* parseCodeGroup(FILE* stream, Token* currentToken){
  expectTokenType(currentToken, TOK_CHAR, '{');
  
  PTNode* codeGroup = newPTNode(NODE_CODE_GROUP,NULL);
  while(currentToken->type != TOK_EOF){
    *currentToken = getNextToken(stream);
    if(currentToken->str[0] == '}') break;
    
    PTNode* node;
    switch(currentToken->type){
      case TOK_TYPE:
        node = parseDecleration(stream, currentToken);
      break;
      case TOK_NAME:
        node = newPTNode(NODE_EXPRESSION, parseExpression(stream, currentToken));
      break;
      default:
        raiseError("(generateCodeGroup) unexpected token \"%s\"\n", currentToken->str);
      break;
    };
    codeGroupAdd(codeGroup,node);
  }
  return codeGroup;
}

PTNode* parseDecleration(FILE* stream, Token* currentToken){
  expectTokenType(currentToken, TOK_TYPE, 0);

  int type = currentToken->value;
  *currentToken = getNextToken(stream);
  expectTokenType(currentToken, TOK_NAME, 0);
  
  char* name = malloc((strlen(currentToken->str)+1)*sizeof(char));
  strcpy(name, currentToken->str);
  
  *currentToken = getNextToken(stream);
  expectTokenType(currentToken, TOK_CHAR, 0);// = ( and ; all valid
  
  if(currentToken->str[0] == '('){
    return parseFunctionDecleration(type, name, stream, currentToken);
  }
  raiseError("unexpected token");
  free(name);
  return NULL;
}

PTNode* parseFunctionDecleration(int returnType, char* name, FILE* stream, Token* currentToken){
  FunctionDeclerationData* data = malloc(sizeof(FunctionDeclerationData));
  data->name = name;
  data->returnType = returnType;
  data->argumentType = TYPE_NONE;

  *currentToken = getNextToken(stream);
  expectTokenType(currentToken, TOK_CHAR, ')');

  *currentToken = getNextToken(stream);
  data->code = parseCodeGroup(stream, currentToken);
  
  return newPTNode(NODE_FUNCTION_DECLERATION, data);
}
