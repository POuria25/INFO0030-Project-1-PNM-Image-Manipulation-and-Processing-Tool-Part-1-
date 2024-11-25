/**
 * pnm.h
 *
 * This file contiens declaration of PNM structure and functions for 
 * image manipulation. 
 *
 * @author: KATOUZIAN Pouria S192865
 * @date: 10/02/2021
 * @projet: INFO0030 Projet 1
 */

#ifndef __PNM__
#define __PNM__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/**
 * PNM_t structure for PNM images 
 *
 */
typedef struct PNM_t PNM;


/**
 * load_pnm
 * 
 * Load an image from a file.
 * 
 * @param image a pointer to PNM.
 * @param filename the path to the file.
 * 
 * @pre: image != NULL, filename != NULL
 * @post: image contains the image from the file.
 * 
 * @return:
 *    0 Success
 *   -1 Malformed file name
 */
int load_pnm(PNM **image, char* filename);

/**
 * write_pnm
 * 
 * Write an image to a file.
 * 
 * @param image a pointer to PNM.
 * @param filename the path to the file.
 * 
 * @pre: image != NULL, filename != NULL
 * @post: image is written to the file.
 * 
 * @return:
 *    0 Success
 *   -2 Error Opening File
 */
int write_pnm(PNM *image, char* filename);

/**
 * free_pnm
 * 
 * Free the memory allocated for the image.
 * 
 * @param image a pointer to PNM.
 * 
 * @pre: image != NULL
 * @post: image is freed.
 * 
 * @return: -
 */
void free_pnm(PNM *image);
#endif // __PNM__
