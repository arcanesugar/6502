#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "tokeniser.h"
#include "error.h"

#define NUM_KEYWORDS 2

int sourceLineNumber = 1;

typedef struct Keyword{
  char str[10];
  int tokenType;
  int value;
}Keyword;

Keyword keywords[NUM_KEYWORDS] = {
  {.str = "none", .tokenType = TOK_TYPE, .value = TYPE_NONE},
  {.str = "byte", .tokenType = TOK_TYPE, .value = TYPE_BYTE}
};

bool isSeperator(char c){//seperators become single character tokens
  if(isspace(c)){
    if(c == '\n') sourceLineNumber++;
    return true;
  }
  char seperators[] = "(){};+-";
  if(strchr(seperators,c) != NULL) return true;
  return false;
}

enum literalTypes{
  CHARLIT,
  NUMLIT
};

Token getNextToken(FILE* stream, char* lastChar){
  while(isspace(*lastChar)){if(*lastChar == '\n') sourceLineNumber++; *lastChar = fgetc(stream);}//eat whitespace
  Token token;
  token.value = -1;
  int tokenLen = 0;
  if(isSeperator(*lastChar) || *lastChar == EOF){
    token.str[0] = *lastChar;
    token.str[1] = 0;
    token.type = TOK_CHAR;
    *lastChar = fgetc(stream);//eat the token
    return token;
  }
  
  if(*lastChar == '\''){
    token.str[0] = *lastChar;
    token.str[1] = fgetc(stream);
    token.str[2] = fgetc(stream);
    token.str[3] = 0;
    *lastChar = fgetc(stream);//eat ' character
    
    if((strlen(token.str) != 3) || (token.str[0] != '\'') || (token.str[2] != '\''))
      raiseError("char token must be in the form '<char>'\n");
    token.value = token.str[1];
    token.type = TOK_BYTELIT;
    return token;
  }

  token.type = -1;//unknown
  int literalType = -1;//not a literal
  if(isdigit(*lastChar)){
    literalType = NUMLIT;
  }
  while(1){
    token.str[tokenLen++] = *lastChar;
    *lastChar = fgetc(stream);
    if(isSeperator(*lastChar) || *lastChar == EOF){if(*lastChar == '\n') sourceLineNumber++; break;}
    //error checking
    if(literalType == NUMLIT){
      if(!(isdigit(*lastChar) || *lastChar == '.')){
        raiseError("num token can only contain 0-9 and .\n");
      }
    }
  }
  token.str[tokenLen++] = 0;
  
  //determine token type if unknown
  if(token.type == -1){
    token.type = TOK_NAME;
    for(int i = 0; i<NUM_KEYWORDS; i++){
      if(strcmp(token.str, keywords[i].str) == 0){
        token.type = keywords[i].tokenType;
        token.value = keywords[i].value;
      }
    }

  }
  
  //assign token value if token is a literal
  switch(literalType){
    case NUMLIT:
      token.value = atoi(token.str);
      token.type = TOK_BYTELIT;
      break;
    default:
      break;
  }
  
  return token;  
};
