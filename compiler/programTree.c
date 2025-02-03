#include <stdbool.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

#include "error.h"
#include "programTree.h"
#include "expression.h"
#include "tokeniser.h"

PTNode* newPTNode(int type, void* data){
  PTNode* node = (PTNode*)malloc(sizeof(PTNode));
  node->type = type;
  node->data = data;
  node->next = NULL;
  return node;
};

void codeGroupAdd(PTNode* codeGroup, PTNode *node){
  if(codeGroup->type != NODE_CODE_GROUP) {
    raiseError("(codeGroupAdd) PTNode* codeGroup is not a CODE_GROUP\n");
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

PTNode* generateCodeGroup(FILE* stream, char* lastChar, Token* currentToken);

PTNode* generateExpressionNode(FILE* stream, char* lastChar, Token* currentToken){
  return newPTNode(NODE_EXPRESSION, parseExpression(stream, lastChar, currentToken));
}

PTNode* generateDeclerationNode(FILE* stream, char* lastChar, Token* currentToken){
  //assume that currentToken is a "type" token

  int type = currentToken->value;
  *currentToken = getNextToken(stream, lastChar);
  expectTokenType(currentToken, TOK_NAME, 0);
  
  char* name = malloc((strlen(currentToken->str)+1)*sizeof(char));
  strcpy(name, currentToken->str);
  
  *currentToken = getNextToken(stream, lastChar);
  expectTokenType(currentToken, TOK_CHAR, 0);// = ( and ; all valid
  
  if(currentToken->str[0] == '('){//function decleration
    PTNode* node = newPTNode(NODE_FUNCTION_DECLERATION,NULL);
    FunctionDeclerationData* data = (FunctionDeclerationData*)malloc(sizeof(FunctionDeclerationData));
    data->name = name;
    data->returnType = type;
    data->argumentType = TYPE_NONE;

    while(currentToken->str[0] != ')'){
      *currentToken = getNextToken(stream, lastChar);
      if(currentToken->type == TOK_TYPE){
        data->argumentType = currentToken->value;
      }
    }

    *currentToken = getNextToken(stream, lastChar);//eat '{' token
    expectTokenType(currentToken, TOK_CHAR, '{');
    data->code = generateCodeGroup(stream, lastChar, currentToken);
    node->data = data;
    return node;
  }
  free(name);
  return NULL;
}

PTNode* generateCodeGroup(FILE* stream, char* lastChar, Token* currentToken){
  PTNode* codeGroup = newPTNode(NODE_CODE_GROUP,NULL);
  while(1){
    *currentToken = getNextToken(stream, lastChar);
    PTNode* node;
    if(currentToken->str[0] == EOF) break;
    if(currentToken->str[0] == '}') break; // closing brace indicates the end of the group
    switch(currentToken->type){
      case TOK_TYPE://function declerations are invalid here, but variable declerations are valid
        node = generateDeclerationNode(stream, lastChar, currentToken);
      break;
      case TOK_NAME:
        node = generateExpressionNode(stream, lastChar, currentToken);
      break;
      default:
        raiseError("(generateCodeGroup) unexpected token\n");
      break;
    };
    codeGroupAdd(codeGroup,node);
  }
  return codeGroup;
}

PTNode* generateProgramTree(FILE* stream){
  PTNode* program = newPTNode(NODE_CODE_GROUP, NULL);
  Token currentToken;
  char lastChar = ' ';
  while(1){
    currentToken = getNextToken(stream,&lastChar);
    if(currentToken.str[0] == EOF) break;
    
    PTNode* node;
    switch(currentToken.type){
      case TOK_TYPE:
        node = generateDeclerationNode(stream, &lastChar, &currentToken);
        break;
      default:
        raiseError("(generateProgramTree) unexpected token\n");
    }
    codeGroupAdd(program, node);
  }
  return program;
}
