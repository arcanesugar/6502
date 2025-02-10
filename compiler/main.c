#include <stdio.h>
#include <stdarg.h>
#include "programTree.h"
#include "codegen.h"

void compile(char* inputFilename, char* outputFilename){

  FILE* inputFile = fopen(inputFilename,"r");
  PTNode* program = generateProgramTree(inputFile);
  fclose(inputFile);

  FILE* outputFile = fopen(outputFilename,"w");
  codegen(outputFile, program);
  fclose(outputFile);
  
}

int main(int argc, char* argv[]){
  compile("source.txt", "source.s");
  return 0;
}
