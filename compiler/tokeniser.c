#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "tokeniser.h"

#define NUM_KEYWORDS 2

typedef struct Keyword{
  char str[10];
  int tokenType;
  int value;
}Keyword;

Keyword keywords[NUM_KEYWORDS] = {
  {.str = "none", .tokenType = TYPE_TOK, .value = NONE},
  {.str = "num8", .tokenType = TYPE_TOK, .value = NUM8}
};

bool isSeperator(char c){
  if(isspace(c)) return true;
  char seperators[] = "(){};";
  if(strchr(seperators,c) != NULL) return true;
  return false;
}

Token nextToken(FILE* stream, char* lastChar){
  while(isspace(*lastChar)){*lastChar = fgetc(stream);}//eat whitespace
  Token token;
  token.value = -1;
  int tokenLen = 0;
  if(isSeperator(*lastChar) || *lastChar == EOF){
    token.str[0] = *lastChar;
    token.str[1] = 0;
    token.type = CHAR_TOK;
    *lastChar = fgetc(stream);//eat the token
    return token;
  }

  token.type = -1;//unknown
  if(isdigit(*lastChar)){
    token.type = NUMLIT_TOK;
  }else if(*lastChar == '\''){
    token.type = CHARLIT_TOK;
  }
  while(1){
    token.str[tokenLen++] = *lastChar;
    *lastChar = fgetc(stream);
    if(isSeperator(*lastChar) || *lastChar == EOF) break;

    //error checking
    switch(token.type){
      case NUMLIT_TOK:
        if(!(isdigit(*lastChar) || *lastChar == '.'))
          printf("ERROR: num token can only contain 0-9 and .\n");
        break;
      default:
        break;
    }
  }
  token.str[tokenLen++] = 0;
  //determine token type if unknown
  if(token.type == -1){
    token.type = NAME_TOK;
    for(int i = 0; i<NUM_KEYWORDS; i++){
      if(strcmp(token.str, keywords[i].str) == 0){
        token.type = keywords[i].tokenType;
        token.value = keywords[i].value;
      }
    }
  }else{
    switch(token.type){
      case NUMLIT_TOK:
        token.value = atoi(token.str);
      break;
      case CHARLIT_TOK:
        token.value = token.str[1];
      break;
      default:
      break;
    }
  }
  return token;
};
