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

char* getTokenTypeName(int type);
Token nextToken(FILE* stream, char* lastChar);
