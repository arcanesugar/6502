#include <stdbool.h>
#include <stdio.h>

#define MAX_TOKEN_LEN 100
typedef struct Token{
  char str[MAX_TOKEN_LEN];
  bool eof;
}Token;

Token nextToken(FILE* stream, char* lastChar);
