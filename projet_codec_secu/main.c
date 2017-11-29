#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int main(int argc, char **argv){
    int choice;
    int valid_matrix = 0, valid_message = 0;
    char matrix[1000];
    char message[1000];
    int column_nb = 0, line_nb = 1;
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
            case 2: get_message(message, &valid_message);
                    break;
            case 3:
                    if(valid_matrix == 1 && valid_message == 1){
                        hash_message(message, matrix, column_nb, line_nb);
                    } else {
                        printf("\nVeuillez entrer une matrice et un message valide\n");
                        system("pause");
                    }
                    break;
            case 4: if(valid_matrix == 1 && valid_message == 1){
                        decrypt_message(message, matrix, column_nb, line_nb);
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

void get_matrix(char *matrix, int *column_nb, int *line_nb, int *valid_matrix){
    FILE *matrix_fp;
    char filecontent[1000];
    char filename[1000];
    int start = 0;
    int i = 0;

    printf("\nVeuillez entrez le nom ou le chemin du fichier : ");
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

void get_message(char *message, int *valid_message){
    FILE *message_fp;
    char filename[1000];

    printf("\nVeuillez entrez le nom ou le chemin du fichier : ");
    fflush(stdin);
    fgets(filename,1000,stdin);
    filename[strlen(filename)-1]='\0';
    message_fp = fopen(filename,"r+");
    fread(message,sizeof(char),1000,message_fp);
    fclose(message_fp);
    if(strlen(message) > 0){
        *valid_message = 1;
    } else {
        *valid_message = 0;
    }
}


void hash_message(char *message, char *matrix, int column_nb, int line_nb){
    FILE *finalfp;
    int *binary= NULL;
    int binary_message[8000];
    char crypted_message[8000]={0};
    char final_message[8000]={0};
    char filename[1000];
    int i = 0, j = 0, k = 0, l = 0;

    for(i = 0; i < strlen(message); i++){
        for(j = 7; j >= 0; j--){
            binary_message[(8 * i) + (7-j)]=((1 << j) & message[i]) >> j;
        }
    }

    binary = malloc(column_nb*sizeof(int));

    for(i = 0; i < strlen(message)*8/line_nb; i++){
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

    free(binary);

    for(i = 0; i < strlen(message)*8/line_nb; i++){
        for(j = 0; j < 8; j++){
            final_message[i]=final_message[i]^crypted_message[j+8*i];
            if(j!=7) final_message[i] = final_message[i] << 1;
        }
    }

    printf("\nVeuillez entrez le nom du fichier a creer : ");
    fflush(stdin);
    fgets(filename,1000,stdin);
    filename[strlen(filename)-1]='\0';

    finalfp=fopen(filename,"wb");
    fwrite(final_message,sizeof(char),strlen(message)*8/line_nb,finalfp);
    fclose(finalfp);
}


void decrypt_message(char *message, char *matrix, int column_nb, int line_nb){
    FILE *finalfp;
    int *newbinary= NULL;
    int binary_message[8000]={-1};
    char decrypted_message[8000]={0};
    char final_message[8000]={0};
    char filename[1000];
    int i = 0, j = 0, k = 0, l = 0, m = 0;

    for(i = 0; i < strlen(message); i++){
        for(j = 7; j >= 0; j--){
            binary_message[(8 * i) + (7-j)]=((1 << j) & message[i]) >> j;
        }
    }

    newbinary = malloc(column_nb*sizeof(int));
    do{
        newbinary[l] = binary_message[k+4];
        l++;
        newbinary[l] = binary_message[k+1];
        l++;
        newbinary[l] = binary_message[k+2];
        l++;
        newbinary[l] = binary_message[k+3];
        l++;
        k = k+8;

    } while(k < strlen(message)*8);

    for(j = 0; j < l; j++){
            decrypted_message[j]=newbinary[j];
    }

    for(i = 0; i < column_nb*8/line_nb; i++){
        m = i;
        for(j = 0; j < 8; j++){
            final_message[i]=final_message[i]^decrypted_message[j+8*i];
            if(j!=7) final_message[i] = final_message[i] << 1;
        }
    }

    printf("\nVeuillez entrez le nom du fichier a creer : ");
    fflush(stdin);
    fgets(filename,1000,stdin);
    filename[strlen(filename)-1]='\0';

    finalfp=fopen(filename,"wb");
    fwrite(final_message,sizeof(char),m,finalfp);
    fclose(finalfp);
}
