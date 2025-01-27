#include <stdio.h>
#include "programTree.h"

void compile(char* inputFilename, char* outputFilename){
  FILE* inputFile = fopen(inputFilename,"r");
  FILE* outputFile = fopen(outputFilename,"w");
  PTNode* program = generateProgramTree(inputFile);
  fclose(inputFile);
  fclose(outputFile);
  printProgramTree(program,0);
}

int main(int argc, char* argv[]){
  compile("source.txt", "out.txt");
  return 0;
}
