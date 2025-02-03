#include <stdio.h>
#include "programTree.h"
#include "debug.h"

void compile(char* inputFilename, char* outputFilename){
  FILE* inputFile = fopen(inputFilename,"r");
  PTNode* program = generateProgramTree(inputFile);
  fclose(inputFile);
  printProgramTree(program);
}

int main(int argc, char* argv[]){
  compile("source.txt", "out.txt");
  return 0;
}
