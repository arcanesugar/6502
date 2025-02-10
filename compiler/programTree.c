#include <stdbool.h>
#include <malloc.h>

#include "programTree.h"
#include "error.h"
#include "lexer.h"

static PTNode* newPTNode(int type, void* data){
  PTNode* node = malloc(sizeof(PTNode));
  node->type = type;
  node->data = data;
  node->next = NULL;
  return node;
}

static void codeGroupAdd(PTNode* codeGroup, PTNode* node){
  if(codeGroup == NULL) raiseError("CodeGroup cannot be null\n");
  if(codeGroup->type != N_CODE_GROUP){
    raiseError(" codeGroupAdd() PTNode* codeGroup must be an N_CODE_GROUP\n");
  }

  if(codeGroup->data == NULL){
    codeGroup->data = node;
    return;
  }

  PTNode* currentNode = codeGroup->data;
  while(currentNode->next != NULL){
    currentNode = currentNode->next;
  }
  currentNode->next = node;
}

static PTNode* parseDecleration(int type, FILE* stream);
static PTNode* parseAssignmemt(char* name, FILE* stream);
static PTNode* parseFunctionCall(char* name, FILE* stream);
static PTNode* parseFunctionDefinition(int type, char* name, FILE* stream);
static PTNode* parseCodeGroup(FILE* stream);
static PTNode* parseExpression(FILE* stream);
static PTNode* parseVariableDecleration(int type, char* name, FILE* stream);

PTNode* generateProgramTree(FILE* stream){
  PTNode* root = newPTNode(N_CODE_GROUP, NULL);
  Token token = getNextToken(stream);
  while(token.type != TOK_EOF){
    PTNode* node;
    switch(token.type){
      case TOK_TYPE:
        node = parseDecleration(token.type, stream);
        break;
      default:
        raiseError("Unexpected token, \"%s\"\n", getTokenTypeName(token.type));
        break;
    }
    codeGroupAdd(root,node);
    token = getNextToken(stream);
  }
  return root;
}

PTNode* parseExpression(FILE* stream){
  PTNode* leftExpression = newPTNode(0,NULL);
  Token token = getNextToken(stream);
  switch(token.type){
    case TOK_BYTELIT:
      leftExpression->type = N_LITERAL;
      leftExpression->data = malloc(sizeof(int));
      *(int*)leftExpression->data = token.value;
    break;
    default:
      raiseError("Unexpected token in expression (\"%s\")\n", token.str);
    break;
  }
  token = getNextToken(stream);
  expectTokenType(token, TOK_CHAR, 0);
  switch(token.value){//eventially this will handle cases like operations
    //terminating tokens
    case ';':
    case ')':
      ungetToken(token);
      return leftExpression;
    default:
      raiseError("Unexpected token in expression (\"%s\")\n", token.str);
  }
  return leftExpression;
}

PTNode* parseCodeGroup(FILE* stream){
  PTNode* node = newPTNode(N_CODE_GROUP, NULL);
  Token token = getNextToken(stream);
  expectTokenType(token, TOK_CHAR, '{');
  while(token.type != TOK_EOF){
    Token token = getNextToken(stream);
    if(token.type == TOK_CHAR && token.value == '}') break;
    switch(token.type){
      case TOK_TYPE:
        codeGroupAdd(node, parseDecleration(token.value, stream));
        break;
      case TOK_NAME:
        codeGroupAdd(node, parseAssignmemt(token.str, stream));
        break;
      default:
        raiseError("Unexpected Token %s\n", token.str);
      break;
    }
  }
  return node;
}

PTNode* parseFunctionDefinition(int type, char* name, FILE* stream){
  Token currentToken;
  FunctionDefinition* fd = malloc(sizeof(FunctionDefinition));
  fd->returnType = type;
  fd->name = name;
  
  //argument
  fd->argument = NULL;
  currentToken = getNextToken(stream);
  if(currentToken.type != TOK_CHAR || currentToken.value != ')'){
    expectTokenType(currentToken, TOK_TYPE, 0);
    int argType = currentToken.value;

    currentToken = getNextToken(stream);
    expectTokenType(currentToken, TOK_NAME, 0);

    VariableDecleration* vd = malloc(sizeof(VariableDecleration));
    vd->type = argType;
    vd->name = strToHeap(currentToken.str);
    vd->startValue = NULL;
    fd->argument = newPTNode(N_VARIABLE_DECLARATION, vd);
    currentToken = getNextToken(stream);
  }
  expectTokenType(currentToken, TOK_CHAR, ')');
  //code
  fd->code = parseCodeGroup(stream);
  
  PTNode* node = newPTNode(N_FUNCTION_DEFINITION, fd);
  return node;
}

PTNode* parseFunctionCall(char* name, FILE* stream){
  FunctionCall* fc = malloc(sizeof(FunctionCall));
  Token token;
  
  //argument
  fc->argument = NULL;
  token = getNextToken(stream);
  if(token.type != TOK_CHAR || token.value != ')'){
    ungetToken(token);
    fc->argument = parseExpression(stream);
    token = getNextToken(stream);
  }
  expectTokenType(token, TOK_CHAR, ')');
  fc->name = strToHeap(name);
  PTNode* node = newPTNode(N_FUNCTION_CALL, fc);
  return node;
}

PTNode* parseAssignmemt(char* name, FILE* stream){
  Token token = getNextToken(stream);
  if(token.type == TOK_CHAR && token.value == '('){
    PTNode* n = parseFunctionCall(name, stream);
    Token token = getNextToken(stream);
    expectTokenType(token, TOK_CHAR, ';');
    return n;
  }
  expectTokenType(token, TOK_CHAR, '=');
  VariableAssignment* va = malloc(sizeof(VariableAssignment));
  va->name = strToHeap(name);
  va->value = parseExpression(stream); //todo: expression parsing
  token = getNextToken(stream);//eat ; token
  expectTokenType(token, TOK_CHAR, ';');//just in case, parseExpression should make sure this is true though
  PTNode* node = newPTNode(N_VARIABLE_ASSIGNMENT, va);
  return node;
}

PTNode* parseVariableDecleration(int type, char* name, FILE* stream){
  VariableDecleration* vd = malloc(sizeof(VariableDecleration));
  vd->type = type;
  vd->name = name;
  Token t = getNextToken(stream);
  expectTokenType(t, TOK_CHAR, 0);
  switch(t.value){
    case '=':
      vd->startValue = parseExpression(stream);
      Token t = getNextToken(stream);
      expectTokenType(t, TOK_CHAR, ';');
      break;
    case ';':
      break;
    default:
      raiseError("Unexpected token %s\n", t.str);
    break;
  }
  PTNode* node = newPTNode(N_VARIABLE_DECLARATION, vd);
  return node;
}

PTNode* parseDecleration(int type, FILE* stream){
  Token currentToken;
  currentToken = getNextToken(stream);
  expectTokenType(currentToken, TOK_NAME, 0);
  char* name = strToHeap(currentToken.str);
  
  currentToken = getNextToken(stream);
  expectTokenType(currentToken, TOK_CHAR, 0);
  PTNode* node;
  switch(currentToken.value){
    case '=':
    case ';':
      ungetToken(currentToken);
      node = parseVariableDecleration(type, name, stream);
      break;
    case '(':
      node = parseFunctionDefinition(type, name, stream);
      break;
    default:
      raiseError("Unexpected token %s, expected \";\", \"=\", or \"(\"\n", currentToken.str);
      break;
  }
  return node;
}

