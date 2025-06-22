#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define BUFFER_SIZE 1024 //must be at least 8 characters
//if a .include line is too long to fit in the buffer the program will crash when it tries to read the file name
static char* buffer;

void error(char* message){
  fprintf(stderr, "Err: %s", message);
  exit(-1);
}

int squishFile(char* filename, FILE* outfile){
  FILE* infile = fopen(filename, "r");
  
  bool newLine = true; 
  bool eof = false; 
  while(1){
    int numItems = 0;
    while(numItems<BUFFER_SIZE){
      char c = getc(infile);
      if(c == EOF) {eof = true; break;}
      buffer[numItems++] = c;
      if(c == '\n') break;
    }
    if(newLine && strncmp(buffer, ".include", 8) == 0){
      //get included file name
      int i = 8;
      while(isspace(buffer[i])){i++;}
      if(buffer[i] != '"') error(".include must be followed by a string\n");
      i++;
      int nameStart = i;
      while(buffer[i] != '"') i++;
      int nameLen = i-nameStart;
      if(nameLen == 0) error(".include must include a valid file");
      char* name = malloc(nameLen+1);
      strncpy(name, &buffer[nameStart], nameLen);
      
      size_t commentLen = nameLen+12;//";" + " " + name + " " + "included\n";
      char* comment = malloc(commentLen+1);
      strcpy(comment, "; ");
      strcat(comment, name);
      strcat(comment, " included\n");
      fwrite(comment, sizeof(char), strlen(comment), outfile);

      squishFile(name, outfile);

      strcpy(comment, "; ");
      strcat(comment, name);
      strcat(comment, " end\n");
      fwrite(comment, sizeof(char), strlen(comment), outfile);     
    }
    else{
      fwrite(buffer, sizeof(char), numItems, outfile);
    }
    if(eof) break;
    newLine = false;
    if(numItems < BUFFER_SIZE || buffer[BUFFER_SIZE] == '\n') newLine = true;
  }
  return 0;
}

int main(int argc, char* argv[]){
  if(argc != 2)//first argument is the name of the program
    error("must pass exactly 1 argument\n");
  char* filename = argv[1];
  int l = strlen(filename);
  int i = l;
  int el = 0;
  while(filename[i] != '.') {i--; el++;}
  char* fileExtention = malloc(el+1);
  strcpy(fileExtention, &filename[l-el]);
  printf("%s\n", fileExtention);
  char* outfilename = malloc(l+5+1);
  strcpy(outfilename, filename);
  strcpy(&outfilename[l-el], ".sqsh");//replace file extention
  strcat(outfilename, fileExtention);//add old file extention
  printf("%s\n", outfilename);
  FILE* outfile = fopen(outfilename, "w");
  buffer = malloc(BUFFER_SIZE);
  squishFile(filename, outfile);
  free(buffer);
  return 0;
}
