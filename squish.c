#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define BUFFER_SIZE 1024 //must be at least 8 characters
//if a .include line is too long to fit in the buffer the program will crash when it tries to read the file name

struct DynamicArray{
  void** arr;
  int len;
};


static char* buffer;
static struct DynamicArray aif;//already included files

void error(char* message){
  fprintf(stderr, "Err: %s", message);
  exit(-1);
}

struct DynamicArray daCreate(){
  struct DynamicArray da;
  da.len = 0;
  da.arr = NULL;
  return da;
}

void daAdd(struct DynamicArray *da, void* element){
  da->len += 1;
  da->arr = realloc(da->arr, sizeof(void*)*da->len);
  da->arr[da->len-1] = element;
}

void daFree(struct DynamicArray da){
  for(int i = 0; i<da.len; i++){
    free(da.arr[i]);
  }
  free(da.arr);
}

bool alreadyIncluded(char* name){
  for(int i= 0; i<aif.len; i++){
    if(strcmp(name, aif.arr[i]) == 0) return true;
  }
  return false;
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

      if(alreadyIncluded(name)) continue;

      daAdd(&aif, name);
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
  char* inFileName = NULL;
  char* outFileName = NULL;
  for(int i = 1; i<argc; i++){//first argument is the name of the program
    if(strcmp(argv[i], "-i") == 0){
      i++;
      if(i>argc) error("-i option must be followed by a file\n");
      if(inFileName != NULL ) error("only 1 input file is allowed\n");
      inFileName = argv[i];
      continue;
    }
    if(strcmp(argv[i], "-o") == 0){
      i++;
      if(i>argc) error("-o option must be followed by a file\n");
      if(outFileName != NULL ) error("only 1 output file is allowed\n");
      outFileName = argv[i];
      continue;
    }
    if(inFileName != NULL ) error("only 1 input file is allowed\n");
    inFileName = argv[i];
  }
  if(inFileName == NULL) error("Input file must be provided\n");
  if(outFileName == NULL){
    outFileName = malloc(strlen(inFileName)+5+1);//does not need to be freed
    strcpy(outFileName, inFileName);
    strcat(outFileName, ".sqsh");
  };
  
  FILE* outfile = fopen(outFileName, "w");
  buffer = malloc(BUFFER_SIZE);

  aif = daCreate();
  squishFile(inFileName, outfile);
  free(buffer);
  daFree(aif);
  return 0;
}
