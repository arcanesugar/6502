#pragma once
#include "tokeniser.h"
#include "programTree.h"
#include "expression.h"

void raiseError(char* fmt, ...);

//raises an error if tokens type does not match type.
//if type == TOK_CHAR and c != 0, an error is also raised if token.str[0] != c
void expectTokenType(Token* token, int type, char c);
char* strToHeap(char* str);//copys a string to the heap
