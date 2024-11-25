/**
 * main.c
 *
 * Ce fichier contient la fonction main() du programme de manipulation
 * de fichiers pnm.
 *
 * @author: SALEHIKATOZI SeyedPouria S192865
 * @date: 10/02/2021
 * @projet: INFO0030 Projet 1
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <ctype.h>
#include <getopt.h>
#include <string.h>

#include "pnm.h"

/*
 *convert_lowercase
 * @pre: string != NULL
 * @post: convert uppercase string to lowerercase string
 */
static char* convert_lowercase(char *string){
   assert(string != NULL);

   unsigned int i = 0;
   char str[3];
   while(string[i] != '\0'){
     char character = string[i];
     str[i] = tolower(character);
     i++;
   }
   return (char *)strcpy(string , str);
}

/*
 *detect_fileName
 * @pre: string != NULL
 * @post: detect if filename doesn't contain any wierd character such as ! ? / \ <>
 */
static int detect_fileName(char *string){
   assert(string != NULL);

   const char forbidenChar[] = "<>/:;<>!|{}[]()^!'\"?";
   unsigned int i = 0;
   while(forbidenChar[i] != '\0'){
       if(strchr(string, forbidenChar[i])){
           return -1;
       }
       i++;
   }
   return 0;
}

/*
 * check_inputeFormat
 * @pre: string != NULL
 * @post: check if format given by user is pgm , pbm or ppm
 */
static int check_inputeFormat(char *string){
   assert(string != NULL);

   if((!strcmp(string, "pbm")) || (!strcmp(string, "pgm")) || (!strcmp(string, "ppm"))){
     return 0;
   }else{
     return -2;
   }
}

int main(int argc, char *argv[]){

   char *optstring = "hf:i:o:";
   char *fileFormat;
   char *inputFile;
   char *outputFile;
   int  val;

   while((val = getopt(argc, argv, optstring)) != EOF){
      if(argc < 8){
         switch(val){
            case 'h':
               printf("Enter: -f <fileFormat> -i <inpute> -o <outpute>\n\n");
               break;
            case 'f':
                 fileFormat = optarg;
                 if(optarg != NULL){
                 printf("You entred : %s", fileFormat);
              }else if(fileFormat == NULL){
                 printf("Empty or wrong format\n");
                 exit(-2);
               }
               break;
            case 'i':
                inputFile = optarg;
                if(optarg != NULL){
                  printf("   %s", inputFile);
               }else if(inputFile == NULL){
                  printf("Empty -i inpute[filename]\n");
                  exit(-2);
               }
               break;
            case 'o':
                outputFile = optarg;
                if(optarg != NULL){
                  printf("  %s\n", outputFile);
               }else if(outputFile == NULL){
                  printf("Empty -o outpute[Filename]\n");
                  exit(-2);
               }
               break;
             default:
               printf("\nError : Entered Command not defined \n");
               break;
         }
      }
      else{
         printf("\n\nToo Many Argument\n\n");
         exit(-2);
      }
   }

   PNM *image;
   if((inputFile && fileFormat)){
      if(strlen(fileFormat) != 3){
        printf("Error : Format must contain only 3 character\n");
        return -2;
      }
      convert_lowercase(fileFormat);
      int formatValidation = check_inputeFormat(fileFormat);
      if(formatValidation != 0){
        return -2;
      }
      int fileValidation = detect_fileName(outputFile);
      if(fileValidation != 0){
        printf("Invalide Outpute File Name \n");
        return -1;
      }
      char *checkIn = (inputFile + strlen(inputFile) - strlen(fileFormat)); //getting inpute file format entered by user
      char *checkOut = (outputFile + strlen(outputFile) - strlen(fileFormat)); //getting outpute file format entred by user
      unsigned int checkInpute = strcmp(checkIn, fileFormat); //comparing image file format with format inpute
      unsigned int checkOutpute = strcmp(checkOut, fileFormat);// comparing outpute image format with format inpute
         if((checkInpute == 0) && (checkOutpute == 0)){
            int check_load = load_pnm(&image, inputFile);
            if(check_load == -1 || check_load == -2 || check_load == -3){
               printf("\nError \n");
               return -1;
            }
            int check_write = write_pnm(image, outputFile);
            if(check_write == -1 || check_write == -2 || check_write == -3){
              printf("\nError \n");
              return -1;
            }
         }
         else{
            printf("\nWORNG File Format\n");
            return -2;
         }
   }

    free_pnm(image);
    printf("\nEnd of program\n");
   return 0;
}
