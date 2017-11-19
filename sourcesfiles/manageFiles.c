#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include "headers/manageFiles.h"


// TEST FUNCTION
FILE * openMatrice(FILE * files){
  char fileName[255];

  fflush(stdin);
  fgets(fileName, 255, stdin);
  fileName[strlen(fileName)-1] = '\0';

  files = fopen(fileName, "r+t");
}

// TEST FUNCTION
FILE * writeAFile(FILE * files){
  char fileName[255];
  char content[255] = "G4C=[10001111 11000111 10100100 10010010]";

  fflush(stdin);
  fgets(fileName, 255, stdin);
  fileName[strlen(fileName)-1] = '\0';

  files = fopen(fileName, "w+t");
  fwrite(content, sizeof(char), 41, files);
  fclose(files);
}
