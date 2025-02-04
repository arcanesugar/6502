#pragma once
#include <stdbool.h>
#include <stdio.h>

#define MAX_TOKEN_LEN 100

enum TokenTypes{
  TOK_NAME,
  TOK_TYPE,
  TOK_CHAR,
  TOK_BYTELIT,
  TOK_EOF
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


//gets the next token from a stream
Token getNextToken(FILE* stream);

extern int sourceLineNumber;
