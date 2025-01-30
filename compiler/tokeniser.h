#include <stdbool.h>
#include <stdio.h>

#define MAX_TOKEN_LEN 100

enum TokenTypes{
  NAME_TOK,
  TYPE_TOK,
  CHAR_TOK,
  //num and string literals
  NUMLIT_TOK,
  CHARLIT_TOK
};

enum DataTypes{
  NONE,
  NUM8
};

typedef struct Token{
  char str[MAX_TOKEN_LEN];
  int type;
  int value;
}Token;

char* getTokenTypeName(int type);
Token nextToken(FILE* stream, char* lastChar);
