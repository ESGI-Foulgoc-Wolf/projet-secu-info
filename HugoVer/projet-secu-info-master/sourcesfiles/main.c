#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include "headers/manageFiles.h"

int main(int argc, char const *argv[]) {

  FILE * fMatrice;
  FILE * fFirstFile;
  FILE * fTransform;

  printf("Veuillez donner le chemin vers la clé matricielle : \n");
  openMatrice(fMatrice);
  printf("Veuillez ecrire un nom de fichier : \n");
  openMatrice(fFirstFile);

  exit(EXIT_SUCCESS);
}
