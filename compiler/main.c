#include <stdio.h>
#include <stdarg.h>
#include "programTree.h"
#include "lexer.h"

void printProgramTree(PTNode* root);
void printPTNode(PTNode* node, int indent);
void printVariableDecleration(PTNode* node, int indent);
void printVariableAssignment(PTNode* node, int indent);
void printCodeGroup(PTNode* node, int indent);
void printFunctionDefinition(PTNode* node, int indent);
void printFunctionCall(PTNode* node, int indent);

char* typeToStr(int type){
  switch(type){
    case TYPE_BYTE:
      return "BYTE";
    case TYPE_NONE:
      return "NONE";
    default:
      return "UNKNOWN_TYPE";
  }
}
void printWithIndent(int indent, char* fmt, ...){
  for(int i = 0; i<indent; i++) printf(" ");
  va_list argptr;
  va_start(argptr,fmt);
  vprintf(fmt, argptr);
  va_end(argptr);
}

void printVariableDecleration(PTNode* node, int indent){
  VariableDecleration* vd = node->data;
  printWithIndent(indent, "<variable_assignment> {\n");
  printWithIndent(indent+2, "type: %s\n", typeToStr(vd->type));
  printWithIndent(indent+2, "name: %s\n", vd->name);
  printWithIndent(indent+2, "startValue:\n");
  printPTNode(vd->startValue, indent+4);
  printWithIndent(indent, "}\n");
}

void printVariableAssignment(PTNode* node, int indent){
  VariableAssignment* va = node->data;
  printWithIndent(indent, "<variable_assignment> {\n");
  printWithIndent(indent+2, "name: %s\n", va->name);
  printWithIndent(indent+2, "value:\n");
  printPTNode(va->value, indent+4);
  printWithIndent(indent, "}\n");
}
void printFunctionCall(PTNode* node, int indent){
  FunctionCall* fc = node->data;
  printWithIndent(indent, "<function_call> {\n");
  printWithIndent(indent+2, "name: %s\n", fc->name);
  printWithIndent(indent+2, "argument:\n");
  printPTNode(fc->argument, indent+4);
  printWithIndent(indent, "}\n");
}

void printFunctionDefinition(PTNode* node, int indent){
  FunctionDefinition* fd = node->data;
  printWithIndent(indent, "<function_definition> {\n");
  printWithIndent(indent+2, "return: %s\n", typeToStr(fd->returnType));
  printWithIndent(indent+2, "name: %s\n", fd->name);
  printWithIndent(indent+2, "argument: \n");
  printPTNode(fd->argument, indent+4);
  printWithIndent(indent+2, "code:\n");
  printPTNode(fd->code, indent + 4);
  printWithIndent(indent, "}\n");
}

void printCodeGroup(PTNode* node, int indent){
  printWithIndent(indent, "<code_group> {\n");
  node = node->data;
  while(node != NULL){
    printPTNode(node, indent+2);
    node = node->next;
  }
  printWithIndent(indent, "}\n");
}

void printLiteral(PTNode* node, int indent){
  printWithIndent(indent, "%d\n", *(int*)node->data);
}

void printPTNode(PTNode* node, int indent){
  if(node == NULL){ 
    printWithIndent(indent, "(null)\n"); 
    return;
  }
  switch(node->type){
    case N_LITERAL:
      printLiteral(node, indent);
    break;
    case N_CODE_GROUP:
      printCodeGroup(node, indent);
    break;
    case N_VARIABLE_DECLARATION:
      printVariableDecleration(node, indent);
    break;
    case N_VARIABLE_ASSIGNMENT:
      printVariableAssignment(node, indent);
    break;
    case N_FUNCTION_DEFINITION:
      printFunctionDefinition(node, indent);
      break;
    case N_FUNCTION_CALL:
      printFunctionCall(node, indent);
      break;
    default:
      printWithIndent(indent, "I can't print this one\n");
    break;
  }
}

void printProgramTree(PTNode* root){
  PTNode* node = root->data;
  while(node != NULL){
    printPTNode(node, 0);
    node = node->next;
  }
}

void compile(char* inputFilename, char* outputFilename){
  FILE* inputFile = fopen(inputFilename,"r");
  PTNode* program = generateProgramTree(inputFile);
  printProgramTree(program);
  fclose(inputFile);
}

int main(int argc, char* argv[]){
  compile("source.txt", "out.txt");
  return 0;
}
