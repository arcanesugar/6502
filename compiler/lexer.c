#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>

#include "lexer.h"
#include "error.h"

#define NUM_KEYWORDS 2

static Token scanNumLiteral(FILE* stream);
static Token scanCharLiteral(FILE* stream);
static Token scanWordToken(FILE* stream);//keyword or name

typedef struct Keyword{
  char str[10];
  int tokenType;
  int value;
}Keyword;

int sourceLineNumber = 1;
static Token nextToken = (Token){.type = -1};
static Keyword keywords[NUM_KEYWORDS] = {
  {.str = "none", .tokenType = TOK_TYPE, .value = TYPE_NONE},
  {.str = "byte", .tokenType = TOK_TYPE, .value = TYPE_BYTE}
};

static bool isCharToken(char c){
  char charTokens[] = "(){}-+=;";
  return strchr(charTokens,c) != NULL;
}

static bool isSeperator(char c){
  if(isCharToken(c)) return true;
  if(isspace(c)) return true;
  if(c == EOF) return true;
  return false;
}

void ungetToken(Token token){
  if(nextToken.type != -1){
    raiseError("ungetToken(): only 1 token can be 'ungotten' at a time\n");
  }
  nextToken = token;
}

Token getNextToken(FILE* stream){
  if(nextToken.type != -1){
    Token token = nextToken;
    nextToken.type = -1;
    return token;
  }
  char c;
  while(isspace(c = fgetc(stream))){if(c == '\n') sourceLineNumber++;}//eat whitespace
  if(c == EOF){
    Token token;
    token.type = TOK_EOF;
    return token;
  }
  if(isCharToken(c)){
    Token token;
    token.type = TOK_CHAR;
    token.str[0] = c;
    token.str[1] = '\0';
    token.value = c;
    return token;
  }

  ungetc(c, stream);//push c back onto the input stream
  //c is the next token that will be read from the stream
  if(c == '\'')
    return scanCharLiteral(stream);

  if(isdigit(c))
    return scanNumLiteral(stream);

  return scanWordToken(stream);
};

Token scanNumLiteral(FILE* stream){
  Token token;
  token.type = TOK_BYTELIT;
  int len = 0;
  char c;
  while(isdigit(c = fgetc(stream))){
    token.str[len++] = c;
  }
  ungetc(c,stream);
  token.str[len] = '\0';
  token.value = atoi(token.str);
  return token;
}

Token scanCharLiteral(FILE* stream){
  Token token;
  token.type = TOK_BYTELIT;
  token.str[0] = fgetc(stream);
  token.str[1] = fgetc(stream);
  token.str[2] = fgetc(stream);
  token.str[3] = '\0';
  if(token.str[0] != '\'' || token.str[2] != '\'') raiseError("char literal must be in the form '<char>'\n");
  token.value = token.str[1];
  return token;
}

void keywordMatch(Token *token){
  for(int i = 0; i<NUM_KEYWORDS; i++){
    if(strcmp(token->str, keywords[i].str) == 0){
      token->type = keywords[i].tokenType;
      token->value = keywords[i].value;
      return;
    }
  }
}

Token scanWordToken(FILE* stream){
  Token token;
  token.type = TOK_NAME;
  int len = 0;
  char c;
  while(!isSeperator(c = fgetc(stream))){
    token.str[len++] = c;
  }
  token.str[len] = '\0';
  ungetc(c,stream);
  keywordMatch(&token);
  return token;
}
