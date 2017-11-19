#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include "headers/manageFiles.h"

FILE * openMatrice(FILE * files){
  char fileName[255];

  fflush(stdin);
  fgets(fileName, 255, stdin);
  fileName[strlen(fileName)-1] = '\0';

  files = fopen(fileName, "r+t");
  return files;
}
