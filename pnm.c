/**
 * pnm.c
 *
 * Ce fichier contient les définitions de types et
 * les fonctions de manipulation d'images PNM.
 *
 * @author: SALEHIKATOZI SeyedPouria S192865
 * @date: 22/02/2021
 * @projet: INFO0030 Projet 1
 */
#define MAX_PIXEL_VALUE_P2 256
#define MAX_PIXEL_VALUE_P3 65536

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "pnm.h"


/**
 * Definition of opaque type
 *
 */
struct PNM_t {
   unsigned int imageFormat;
   unsigned int raw;
   unsigned int column;
   unsigned int colorValue;
   unsigned int **matrix;
};

/*
 * check_color_value
 * @pre: format > 0 && value > 0
 * @post: checking color value of P2 and P3 files
 */
static int check_color_value(unsigned int format, unsigned int value){
   assert(format > 0 && value > 0);

   if((format == 2) && (MAX_PIXEL_VALUE_P2 > value)){
     printf("File Is Compatible P2 [255]\n");
     return 0;
   }else if((format == 3) && (value < MAX_PIXEL_VALUE_P3)){
     printf("File Is Compatible P3 [65536]\n");
     return 0;
   }else{
     printf("FILE IS INCOMPATIBLE\nNOT SUPPORTED [%u] PIXELS\n", value);
     return -3;
   }
}

/*
 * detect_comment
 * @pre: opf != NULL
 * @post: detect & remove file's comment
 */
static void detect_comment(FILE *opf){
   assert(opf != NULL);

   char buffer[1000];
   char x = fgetc(opf);
   if(x != '#'){
     char character = fgetc(opf);
     while(character == '#'){
       fgets(buffer, 1000, opf);
       printf("comment line detected: %s", buffer);
       character = fgetc(opf);
     }
       fseek(opf, -2, SEEK_CUR);
   }else{
     fgets(buffer, 1000, opf);
     fscanf(opf,"\n");
   }
 }

/*
 * check_format
 * @pre: format > 0
 * @post: return format of image
 *
 * @return:
 *     0 Successfully
 *    -3 Error_Wrong image format
 */
static int check_format(unsigned int format){
   assert(format > 0);

   if((format != 1) && (format != 2) && (format != 3)){
      printf("Wrong format \n\n");
      return -3;
   }else{

   printf("FILE IS COMPATIBLE\n");
 }
 return 0;
}

/*
 * creat_matrix
 * @pre: column > 0 && raw > 0
 * @post: Allocate memory for matrix
 */
static unsigned int **creat_matrix(unsigned int raw, unsigned int column,
   unsigned int format){
   assert(raw > 0 && column > 0 && format > 0);

   if(format == 3){
      column = 3 * column;
   }

   unsigned int **matrixPointer = malloc(raw * sizeof(unsigned int *));
   if(!matrixPointer){
      printf("creat_matrix : Memory Allocation Faield For Creating Matrix\n");
      return NULL;
   }
   for(unsigned int i = 0; i < raw; i++){
      matrixPointer[i] = (unsigned int*) malloc(column * sizeof(unsigned int));
      if(!matrixPointer[i]){
         printf("creat_matrix : Memory Allocation Faield For column in line[%u]\n",i);
            for(unsigned int j = 0; j < i; j++){
               free(matrixPointer[j]);
            }
         free(matrixPointer);
         return NULL;
      }
   }
   return matrixPointer;
}

/*
 * destroy
 * @pre: destroy != NULL && column > 0 && raw > 0
 * @post: free Memory Allocated for matrix
 */
static void destroy(unsigned int **destroy, unsigned int raw){
   assert(destroy != NULL && raw > 0);

   for(unsigned int i = 0; i < raw; i++){
      free(destroy[i]);
   }
   free(destroy);
}

/*
 * scan_matrix
 * @pre: ptr != NULL && opf != NULL
 * @post: scan the matrix into structur
 */
static int scan_matrix(PNM *ptr,FILE *opf){
   assert(ptr != NULL && opf != NULL);

   unsigned int column = 0;
   if(ptr->imageFormat == 3){
     column = 3 * ptr->column;
   }else{
     column = ptr->column;
   }
   for(unsigned int i = 0; i < ptr->raw; i++){
     detect_comment(opf);
     for(unsigned int j = 0; j < column; j++){
       int checkMatrix = fscanf(opf, "%u", &ptr->matrix[i][j]);
       if(!checkMatrix){
         return -3;
       }
     }
   }
   return 0;
 }

/*
 * print_matrix
 * @pre: opf != NULL && image != NULL
 * @post: print the matrix into new file
 */
static int print_matrix(FILE *opf, PNM *image){
   assert(opf != NULL  && image != NULL);

   unsigned int column = image->column;
   if(image->imageFormat == 3){
        column = 3 * image->column;
     }
     for(unsigned int i = 0; i < image->raw; i++){
       for(unsigned int j = 0; j < column; j++){
         fprintf(opf, "%u ", image->matrix[i][j]);
       }
       fprintf(opf,"\n");
     }
   return 0;
}

void free_pnm(PNM *image){
  assert(image != NULL);

  destroy(image->matrix, image->raw);
  free(image);
}

int load_pnm(PNM **image, char* filename){
   assert(image != NULL && filename != NULL);

   PNM *ptr = malloc(sizeof(PNM));   //Allocation memory for structur
   if(!ptr){
      printf("Load Function : Structe Memory Allocation Failed \n\n");
      return -1;
   }

   FILE *opf = fopen(filename, "r");
   if(!opf){
      printf("\nOpening File Failed \n\n");
      free(ptr);
      return -2;
   }
   printf("File Opend Successfully \n\n");
   int checkFormat = fscanf(opf, "P%u", &ptr->imageFormat); //inserting the format of image into PNM structur
    if(!checkFormat){
      return -3;
    }
   printf("Format scanned P%u \n\n", ptr->imageFormat);
   int compare = check_format(ptr->imageFormat);
   if(compare != 0){
     printf("INCOMPATIBLE CONTENT \n");
     free(ptr);
     return -3;
   }
   detect_comment(opf);
   int ColumnRaw = fscanf(opf, "%u %u", &ptr->column, &ptr->raw);  //inserting number column and raw from file into structur
   if(!ColumnRaw){
     return -3;
   }
   //printf("Raw : %u  Column : %u \n\n", ptr->raw, ptr->column);
   detect_comment(opf);
   if(ptr->imageFormat == 2 || ptr->imageFormat == 3){
   int ColorValue = fscanf(opf, "%u", &ptr->colorValue);
      if(!ColorValue){
        return -3;
      }
      if((check_color_value(ptr->imageFormat, ptr->colorValue)) != 0){
        free(ptr);
        return -3;
      }
      detect_comment(opf);
      //printf("Color Value is scanned %u\n\n", ptr->colorValue);
   }
   ptr->matrix = creat_matrix(ptr->raw, ptr->column, ptr->imageFormat);  //Allocation memory for matrix
   if(!ptr->matrix){
     printf("MAtrix Allocation Failed\n");
     free(ptr->matrix);
     free(ptr);
     return -1;
   }
   printf("Espace Is Made For Matrix\n\n");
   int checkScan = scan_matrix(ptr, opf);  //inserting matrix into matrix structur
   if(checkScan != 0){
     free(ptr->matrix);
     free(ptr);
     return -3;
   }
   printf("\nMatrix Scanned \n\n");

   fclose(opf);
   *image = ptr;
   return 0;
}

int write_pnm(PNM *image, char* filename){
   assert(image != NULL && filename != NULL);
   FILE *opf = fopen(filename, "w");
   if(!opf){
      printf("\n Creating New File Faield\n");
      free(image);
      return -2;
   }

   fprintf(opf, "P%u\n", image->imageFormat);
   printf("File format is written into new file\n");
   fprintf(opf, "%u %u \n", image->column, image->raw);
   printf("Column & Raw has been copyed into new\n");
   if(image->imageFormat == 2 || image->imageFormat == 3){
      fprintf(opf, "%u \n", image->colorValue);
      printf("Color Value has been copied into new file\n\n");
   }
   print_matrix(opf, image);
   fclose(opf);
   printf("\n\nMatrix is inserted into the new file\n\n");
   //destroy(image->matrix, image->raw);  //free memory
   //free(image);
   printf("Allocated Memory is free\n\n");

   return 0;
}
