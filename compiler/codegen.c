#include <stdarg.h>
#include <string.h>

#include "error.h"

#include "codegen.h"

static void writeNode(PTNode* node);
static void writeCodeGroup(PTNode* node);
static void writeFunctionDefinition(PTNode* node);

//expression result saved to A register
static void writeExpression(PTNode* node);
static void writeFunctionCall(PTNode* node);
static void writeLiteral(PTNode* node);

static FILE* stream;

//printing labels and code in seperate functions might be helpful in the future
static void printCode(char* fmt, ...){
  fprintf(stream,"  ");
  va_list argptr;
  va_start(argptr,fmt);
  vfprintf(stream, fmt, argptr);
  fprintf(stream,"\n");
  va_end(argptr);
}

static void printLabel(char* fmt, ...){
  va_list argptr;
  va_start(argptr,fmt);
  vfprintf(stream, fmt, argptr);
  fprintf(stream,":\n");
  va_end(argptr);
}


void codegen(FILE* outputStream, PTNode* program){
  stream = outputStream;
  printCode(".org $8000");
  printCode("");
  printCode(".include \"iolib.s\"");
  printCode("");

  printLabel("reset");
  printCode("jsr main");
  printCode("");
  //once the program is done loop infinitly
  printLabel("loop");
  printCode("jmp loop");
  printCode("");
  PTNode* currentNode = program->data;
  while(currentNode != NULL){
    writeNode(currentNode);
    currentNode = currentNode->next;
  }


  printCode(".org $fffc");
  printCode(".word reset"); //reset vector
  printCode(".word $0000"); //interrupt vector
}

void writeNode(PTNode* node){
  if(node == NULL) raiseError("writeNode(): node is NULL\n");
  switch(node->type){
    case N_FUNCTION_DEFINITION:
      writeFunctionDefinition(node);
      break;
    case N_FUNCTION_CALL:
      writeFunctionCall(node);
      break;
    case N_LITERAL:
      writeLiteral(node);
      break;
    case N_CODE_GROUP:
      writeCodeGroup(node);
      break;
    default:
      raiseError("(writeNode) Cannot generate code for node\n");
  }
}

void writeCodeGroup(PTNode* node){
  PTNode* currentNode = node->data;
  while(currentNode != NULL){
    writeNode(currentNode);
    currentNode = currentNode->next;
  }
}

void writeLiteral(PTNode* node){
  printCode("lda #%d", *(int*)node->data);
}

void writeFunctionCall(PTNode* node){
  FunctionCall *fc = node->data;
  if(fc->argument != NULL) writeNode(fc->argument);
  printCode("jsr %s", fc->name);
}

void writeFunctionDefinition(PTNode* node){
  FunctionDefinition *fd = node->data;
  printLabel("%s", fd->name);
  writeCodeGroup(fd->code);
  printCode("rts");
  printCode("");
}
