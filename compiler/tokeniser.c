#include <ctype.h>
#include <string.h>
#include "tokeniser.h"

bool isSeperator(char c){
  if(isspace(c)) return true;
  char seperators[] = "(){};";
  if(strchr(seperators,c) != NULL) return true;
  return false;
}

Token nextToken(FILE* stream, char* lastChar){
  while(isspace(*lastChar)){*lastChar = fgetc(stream);}//eat whitespace
  Token token;
  if(*lastChar == EOF) {token.eof = true; return token;}
  token.eof = false;
  int tokenLen = 0;
  token.str[tokenLen++] = *lastChar;
  //hacky solution to make seperators into tokens
  if(isSeperator(*lastChar)) {token.str[tokenLen] = 0; *lastChar = fgetc(stream); return token;}
  while(1){
    *lastChar = fgetc(stream);
    //EOF token is only returned if eof is immidiately recived from the stream,
    //otherwise it acts as a seperator
    if(isSeperator(*lastChar) || *lastChar == EOF) break;
    token.str[tokenLen++] = *lastChar;
  }
  token.str[tokenLen++] = 0;
  return token;
};
