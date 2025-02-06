#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#include "error.h"
#include "tokeniser.h"

char* getTokenTypeName(int type){
  switch(type){
    case TOK_NAME:
      return "NAME";
    case TOK_TYPE:
      return "TYPE";
    case TOK_CHAR:
      return "CHAR";
    case TOK_BYTELIT:
      return "BYTELIT";
    default:
    return "<token type name missing>";
  }
};

char* strToHeap(char* str){
  char* copy = malloc(sizeof(char) * (strlen(str)+1));
  strcpy(copy, str);
  return copy;
}

void expectTokenType(Token* token, int type, char c){
  if(type == TOK_CHAR && c != 0){
    if(token->str[0] != c)
      raiseError("expected %c token, got \"%s\"\n",c,token->str);
    return;
  }
  if(token->type != type)
    raiseError("expected %s token, got \"%s\"\n", getTokenTypeName(type), token->str);
};

void raiseError(char* fmt, ...){
  fprintf(stderr, "[ERROR] ");
  va_list argptr;
  va_start(argptr,fmt);
  vfprintf(stderr, fmt, argptr);
  va_end(argptr);
  exit(EXIT_FAILURE);
}
