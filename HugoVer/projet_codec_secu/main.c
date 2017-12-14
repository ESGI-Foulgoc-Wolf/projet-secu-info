/* But du programme : cryptage/d�cryptage d'un fichier selon une matrice
    auteurs : FOULGOC Medhi, WOLF Hugo*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


int main(int argc, char **argv){
    int choice;
    int valid_matrix = 0, valid_message = 0;
    char matrix[1000];
    char *message = NULL;
    int column_nb = 0, line_nb = 1;
    int *remarkable_pos = NULL;
    int **remarkable_array = NULL;
    FILE *message_fp;
    char filename[1000];
    int length = 0;
    do{
        system("cls");
        printf("Bienvenue dans notre programme de cryptage!\n");
        printf("Veuillez choisir une action:\n1 : choisir une matrice\n2 : choisir un fichier a traiter\n3 : crypter le fichier\n4 : decrypter le fichier\n0 : quitter\n");
        if(valid_matrix == 1){
            printf("\nMatrice :");
            fputs(matrix,stdout);
            printf("\n");
        } else {
            printf("\nAucune matrice valide n'a ete entree.\n");
        }
        printf("%d",length);
        if(valid_message == 1){
            printf("\nMessage :");
            fputs(message,stdout);
            printf("\n");
        } else {
            printf("\nAucun message valide n'a ete entre.\n");
        }
        scanf("%d",&choice);
        switch(choice){
            case 1: get_matrix(matrix, &column_nb, &line_nb, &valid_matrix);
                    break;
            case 2: printf("\nVeuillez entrer le nom ou le chemin du fichier : ");
                    fflush(stdin);
                    fgets(filename,1000,stdin);
                    filename[strlen(filename)-1]='\0';
                    message_fp = fopen(filename,"r+");
                    fseek(message_fp,0,SEEK_END);
                    length = ftell(message_fp);
                    message = malloc(length+1);
                    fseek(message_fp,0,SEEK_SET);
                    fread(message,sizeof(char),length,message_fp);
                    fclose(message_fp);
                    if(strlen(message) > 0){
                        valid_message = 1;
                    } else {
                        valid_message = 0;
                    }
                    break;
            case 3:
                    if(valid_matrix == 1 && valid_message == 1){
                        hash_message(message, matrix, column_nb, line_nb, length);
                    } else {
                        printf("\nVeuillez entrer une matrice et un message valide\n");
                        system("pause");
                    }
                    break;
            case 4: if(valid_matrix == 1 && valid_message == 1){
                        decrypt_message(message, matrix, remarkable_pos, remarkable_array, column_nb, line_nb);
                    } else {
                        printf("\nVeuillez entrer une matrice et un message valide\n");
                        system("pause");
                    }
                    break;
            default: printf("Veuillez entrer une valeur valide");
        }
    }while(choice!=0);
    return 0;
}

//fonction permettant de r�cup�rer la matrice entr�e par l'utilisateur, et de v�rifier si elle correspond au format voulu
void get_matrix(char *matrix, int *column_nb, int *line_nb, int *valid_matrix){
    FILE *matrix_fp;
    char filecontent[1000];
    char filename[1000];
    int start = 0;
    int i = 0;

    printf("\nVeuillez entrer le nom ou le chemin du fichier : ");
    fflush(stdin);
    fgets(filename,1000,stdin);
    filename[strlen(filename)-1]='\0';
    matrix_fp = fopen(filename,"r+");
    fgets(filecontent,1000,matrix_fp);
    fclose(matrix_fp);
    while(filecontent[start]!='['){
        start++;
    }
    strcpy(matrix,&filecontent[start+1]);
    matrix[strlen(matrix)-1]='\0';

    while(matrix[*column_nb]!=' '){
        (*column_nb)++;
    }
    while(matrix[i]!='\0'){
        if(matrix[i]==' ') (*line_nb)++;
        i++;
    }
    if(strlen(matrix)==(*column_nb)*(*line_nb)+(*line_nb)-1) {
        *valid_matrix = 1;
    }

    else {
        *valid_matrix = 0;
    }
}

//fonction permettant de r�cup�rer le fichier entr� par l'utilisateur
void get_message(char *message, int *valid_message){
    FILE *message_fp;
    char filename[1000];
    int length;

    printf("\nVeuillez entrer le nom ou le chemin du fichier : ");
    fflush(stdin);
    fgets(filename,1000,stdin);
    filename[strlen(filename)-1]='\0';
    message_fp = fopen(filename,"r+");
    fseek(message_fp,0,SEEK_END);
    length = ftell(message_fp);
    printf("%d",length);
    message = malloc(length+1);
    fseek(message_fp,0,SEEK_SET);
    system("pause");
    fread(message,sizeof(char),length,message_fp);
    fclose(message_fp);
    if(strlen(message) > 0){
        *valid_message = 1;
    } else {
        *valid_message = 0;
    }
    fputs(message, stdout);
    system("pause");
}

//fonction permettant de crypter le fichier
void hash_message(char *message, char *matrix, int column_nb, int line_nb, int length){
    FILE *finalfp;
    int *binary= NULL;
    int *binary_message = NULL;
    char *crypted_message = NULL;
    char *final_message = NULL;
    char filename[1000];
    int i = 0, j = 0, k = 0, l = 0;

    printf("a");
    binary = malloc(column_nb*sizeof(int));
    binary_message = malloc(sizeof(int)*length*9); //les mallocs dont il faut modifier la taille pour que ça plante pas parce que CA FAIT AUCUN PUTAIN DE SENS
    crypted_message = malloc(length*9);
    final_message = malloc(length*9);

    for(i = 0; i<length*9; i++){
        binary_message[i]=0;
        crypted_message[i]=0;
        final_message[i]=0;
    }

    //transfert du code binaire des chars dans un tableau
    for(i = 0; i < length; i++){
        for(j = 7; j >= 0; j--){
            binary_message[(8 * i) + (7-j)]=((1 << j) & message[i]) >> j;
        }
    }

    //cryptage du code binaire
    for(i = 0; i < length*8/line_nb; i++){
        for(j = 0; j < column_nb; j++){
            binary[j]=0;
        }
        for(j = 0; j < line_nb; j++){
            if(binary_message[j+line_nb*i]==1){
                for(k = 0; k < column_nb; k++){
                    binary[k]= binary[k]^(matrix[k+column_nb*j+1*j]-'0');
                }
            }
        }
        for(j = 0; j < column_nb; j++){
            crypted_message[l]=binary[j];
            l++;
        }
    }
    printf("\nDebug BINARYMESSAGE : \nDebug BINARYMESSAGE strlenOfMessage : %d\n", strlen(message));
    for(j = 0; j < (length*8)*2; j++){
      if(j % 8 == 0){
        printf("\n");
      }
      printf("%d", crypted_message[j]);
    }

    free(binary);

    //reconversion du message crypt� en chars
    for(i = 0; i < strlen(message)*8/line_nb; i++){
        for(j = 0; j < 8; j++){
            final_message[i]=final_message[i]^crypted_message[j+8*i];
            if(j!=7) final_message[i] = final_message[i] << 1;
        }
    }

    /*for(i=0;i<length*8;i++){
        printf("%c",final_message[i]);
    }*/
    //cr�ation du nouveau fichier
    printf("\nVeuillez entrer le nom du fichier a creer : ");
    fflush(stdin);
    fgets(filename,1000,stdin);
    filename[strlen(filename)-1]='\0';
    finalfp=fopen(filename,"wb");
    system("pause");
    fwrite(final_message,sizeof(char),length*9,finalfp); //le fwrite dont on doit aussi changer la taille POUR LA MEME PUTAIN DE RAISOFROOFROZF?ROIZFZREFIZR
    fclose(finalfp);
    free(final_message);
    free(binary_message);
    free(crypted_message);
}

// Transformer la matrice de char => int
void translate_matrix(char *matrix, int **remarkable_array, int column_nb, int line_nb){
  int i;
  int j = 0;
  int k = 0;
  /* On lit le pointeur matrix et on le transforme en tableau de int en supprimant les espaces*/
  for(i = 0; i < column_nb*line_nb+(line_nb-1); i++){
    if(matrix[i] == 32){
      j++;
      k = 0;
    }
    else{
      if(i < 8){
        remarkable_array[j][k] = matrix[i];
        k++;
      }
      if(i > 8 && i < 17){
        remarkable_array[j][k] = matrix[i];
        k++;
      }
      if(i > 17 && i < 26){
        remarkable_array[j][k] = matrix[i];
        k++;
      }
      if(i > 26 && i < 35){
        remarkable_array[j][k] = matrix[i];
        k++;
      }
    }
  }
  /* On transforme les ASCII en vraie valeur décimale 0 ou 1*/
  for(i = 0; i < line_nb; i++){
    printf("\n");
    for(j = 0; j < column_nb; j++){
      remarkable_array[i][j] = (remarkable_array[i][j] == 48) ? 0 : 1;
      printf("%d", remarkable_array[i][j]);
    }
  }
}

//fonction permettant de d�crypter le fichier
void decrypt_message(char *message, char *matrix, int *remarkable_pos, int **remarkable_array, int column_nb, int line_nb){
    FILE *finalfp;
    int *newbinary= NULL;
    int *test = NULL;
    int binary_message[8000]={-1};
    char decrypted_message[8000]={0};
    char final_message[8000]={0};
    char *get_final_message = NULL;
    char filename[1000];
    int i = 0, j = 0, k = 0, l = 0, m = 0, n = 0, p = 0;
    int nb_m = 0;
    int count_lenght = 0;

    //transfert du code binaire des chars dans un tableau
    for(i = 0; i < strlen(message); i++){
        for(j = 7; j >= 0; j--){
            binary_message[(8 * i) + (7-j)]=((1 << j) & message[i]) >> j;
        }
    }
    /* On alloue la mémoire nécessaire à la détermination de l'identité remarquable */
    remarkable_array = malloc(sizeof(int)* line_nb);
    remarkable_pos = malloc(sizeof(int)* line_nb);
    for(i = 0; i < line_nb; i++){
      remarkable_array[i] = malloc(sizeof(int)* column_nb);
    }
    /* On appelle la fonction de traduction char -> int */
    translate_matrix(matrix, remarkable_array, column_nb, line_nb);

    printf("\nDebug matrice1\n");
    // Récupération de l'identité remarquable
    do{
      printf("Debug do\n");
      //system("pause");
      for(i = 0; i < column_nb; i++){
        printf("Debug idrem1 : %d \n", i);
        //system("pause");
        l = 0;
        for(j = 0; j < line_nb; j++){
          printf("Debug idrem2 : %d \n", j);
          //system("pause");
          if(remarkable_array[j][i] == 1 && j == nb_m){
            for(k = 0; k < line_nb; k++){
              printf("Debug idrem3 : %d \n", k);
              //system("pause");
              if(k != nb_m && remarkable_array[k][i] == 0){
                l++;
                printf("Debug idrem3 L : %d \n", l);
              }
            }
          }
        }
        if(l == line_nb-1){
          printf("Debug idrem4");
          remarkable_pos[nb_m] = i;
          nb_m++;
        }
      }

    }while (line_nb - (nb_m + 1) >= 0);

    printf("\nPosition identite remarquable :");
    for(i = 0; i < line_nb; i++){
      printf(" c%d ;", remarkable_pos[i]+1);
    }

    newbinary = malloc(column_nb*sizeof(int));
    //d�cryptage du code binaire
    k = 0;
    p = 0;
    do{
      for(j = 0; j < column_nb; j++){
          newbinary[j]=0;
      }
      m = 0;
      n = 0;
      for(int l = 0; l < line_nb; l++){
        m = k + remarkable_pos[l];
        newbinary[l] = binary_message[m];
        n++;
      }
      k = k + 8;
      for(j = 0; j < n; j++){
              decrypted_message[p]=newbinary[j];
              p++;
      }

    } while(k < strlen(message)*8);
    //reconversion du message crypt� en chars
    for(i = 0; i < column_nb*8/line_nb; i++){
        m = i;
        for(j = 0; j < 8; j++){
            final_message[i]=final_message[i]^decrypted_message[j+8*i];
            if(j!=7) final_message[i] = final_message[i] << 1;
        }
    }



    //cr�ation du nouveau fichier
    printf("\nVeuillez entrer le nom du fichier a creer : ");
    fflush(stdin);
    fgets(filename,1000,stdin);
    filename[strlen(filename)-1]='\0';

    finalfp=fopen(filename,"wb");
    fwrite(final_message,sizeof(char),m,finalfp);
    fclose(finalfp);
}



/*void get_remarkable_identity(char *matrix, int *remarkable_pos, int **remarkable_array, int column_nb, int line_nb){
  int i = 0;
  int j;
  int k;
  int l;

}*/
