#pragma once
#include <stdbool.h>
#include <stdio.h>

#define MAX_TOKEN_LEN 100

enum TokenTypes{
  TOK_NAME,
  TOK_TYPE,
  TOK_CHAR,
  TOK_BYTELIT
};

enum DataTypes{
  TYPE_NONE,
  TYPE_BYTE
};

typedef struct Token{
  char str[MAX_TOKEN_LEN];
  int type;
  int value;
}Token;


//gets the next token from a stream, uses lastchar for storage
//on the first call for a stream, lastchar should be whitespace
//on calls to with that stream in the future, lastchar must remain the same
Token getNextToken(FILE* stream, char* lastChar);

extern int sourceLineNumber;
