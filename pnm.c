/**
 * @file pnm.c
 *
 * @brief Manipulation d'images PNM
 * @author: KATOUZIAN Pouria S192865
 * @date: 10/02/2021
 * @projet: INFO0030 Projet 1 
 *
 */

#include "pnm.h"

/**
 * Data structure for PNM images
 */
struct PNM_t
{
    unsigned int imageFormat;
    unsigned int raw;
    unsigned int column;
    unsigned int colorValue;
    unsigned int **matrix;
};

/**
 * check_color_value
 *
 * check if the color value is compatible with the image format.
 *
 * @param format the format of the image.
 * @param colorValue the color value.
 * @pre: format == 2 || format == 3
 * @post: -
 * @return:
 *    0 The color value is compatible.
 */
static int check_color_value(unsigned int format, unsigned int colorValue)
{
    if (format == 2 && colorValue < 255)
    {
        fprintf(stdout, "File Is Compatible P2 [255]\n");
        return 0;
    }
    else if (format == 3 && colorValue < 65535)
    {
        fprintf(stdout, "File Is Compatible P3 [65535]\n");
    }
    else
    {
        fprintf(stderr, "File Is Not Compatible\n");
        return -3;
    }
    return 0;
}

/**
 * remove_comments
 *
 * remove comments from the file.
 *
 * @param file the file
 * @pre: file != NULL
 * @post: -
 * @return: -
 */
static void remove_comments(FILE *file)
{
    assert(file != NULL);
    char buffer[1000];
    char c = fgetc(file);
    if (c != '#')
    {
        char character = fgetc(file);
        while (character == '#')
        {
            fgets(buffer, 1000, file);
            fprintf(stdout, "Comment Removed %s\n", buffer);
            character = fgetc(file);
        }
        fseek(file, -2, SEEK_CUR);
    }
    else
    {
        fgets(buffer, 1000, file);
        fscanf(file, "\n");
    }
}

/**
 * check_format
 *
 * Check if the format of the image is compatible.
 *
 * @param format the format of the image.
 * @pre: format == 1 || format == 2 || format == 3
 * @post: -
 * @return:
 *   0 The format of the image is compatible.
 *  -3 The format of the image is not compatible.
 */
static int check_format(unsigned int format)
{
    if (format == 1 || format == 2 || format == 3)
    {
        fprintf(stdout, "File Is Compatible\n");
        return 0;
    }
    else
    {
        fprintf(stderr, "File Is Not Compatible\n");
        return -3;
    }
}

/**
 * creat_matrix
 *
 * Create a matrix for the image.
 *
 * @param row the number of rows.
 * @param column the number of columns.
 * @param format the format of the image.
 * @pre: row > 0, column > 0, format > 0
 * @post: -
 * @return:
 *    NULL Memory Allocation Faild
 *    matrix the matrix of the image.
 */
static unsigned int **creat_matrix(unsigned int row, unsigned int column, unsigned int format)
{
    assert(row > 0 && column > 0 && format > 0);
    if (format == 3)
    {
        column = 3 * column;
    }

    unsigned int **matrix = (unsigned int **)malloc(row * sizeof(unsigned int *));
    if (!matrix)
    {
        fprintf(stderr, "Error Allocating Memory\n");
        return NULL;
    }
    for (unsigned int i = 0; i < row; i++)
    {
        matrix[i] = (unsigned int *)malloc(column * sizeof(unsigned int));
        if (!matrix[i])
        {
            fprintf(stderr, "Memory Allocation Faild For Column in Line[%u]\n", i);
            for (unsigned int j = 0; j < i; j++)
            {
                free(matrix[j]);
            }
            free(matrix);
            return NULL;
        }
    }
    return matrix;
}

/**
 * scan_matrix
 *
 * Read matrix from file.
 *
 * @param image image.
 * @param file file.
 * @pre: image != NULL, file != NULL
 *
 * @return:
 *    0 Success
 *    -1 Error Reading Matrix
 */
static int scan_matrix(PNM *image, FILE *file)
{
    assert(image != NULL && file != NULL);
    unsigned int column = (image->imageFormat == 3) ? 3 * image->column : image->column;

    for (unsigned int i = 0; i < image->raw; i++)
    {
        for (unsigned int j = 0; j < column; j++)
        {
            if (fscanf(file, "%u", &image->matrix[i][j]) != 1)
            {
                fprintf(stderr, "Error Reading Matrix\n");
                return -1;
            }
        }
    }
    return 0;
}

/**
 * print_matrix
 *
 * Write matrix to file.
 *
 * @param image image.
 * @param file file.
 * @pre: image != NULL, file != NULL
 *
 * @return:
 *    0 Success
 *    -1 Error Writing Matrix
 */
static int print_matrix(PNM *image, FILE *file)
{
    assert(image != NULL && file != NULL);
    unsigned int column = (image->imageFormat == 3) ? 3 * image->column : image->column;
    for (unsigned int i = 0; i < image->raw; i++)
    {
        for (unsigned int j = 0; j < column; j++)
        {
            if (fprintf(file, "%u ", image->matrix[i][j]) < 0)
            {
                fprintf(stderr, "Error writing matrix at row %u, column %u\n", i, j);
                return -1;
            }
        }
        if (fprintf(file, "\n") < 0)
        {
            fprintf(stderr, "Error writing newline after row %u\n", i);
            return -1;
        }
    }

    return 0;
}


int load_pnm(PNM **image, char *filename)
{
    assert(image != NULL && filename != NULL);
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        fprintf(stderr, "Error Opening File\n");
        return -2;
    }
    *image = (PNM *)malloc(sizeof(PNM));
    if (!*image)
    {
        fprintf(stderr, "Error Allocating Memory\n");
        fclose(file);
        return -1;
    }
    if (fscanf(file, "P%u\n", &(*image)->imageFormat) != 1)
    {
        fprintf(stderr, "Error Reading Format\n");
        fclose(file);
        free(*image);
        return -3;
    }
    fprintf(stdout, "Format: P%u\n", (*image)->imageFormat);
    int format = check_format((*image)->imageFormat);
    fprintf(stdout, "Format: %d\n", format);
    if (format != 0)
    {
        fclose(file);
        free(*image);
        return -3;
    }
    if((*image)->imageFormat == 1){
        remove_comments(file);
    }
    if (fscanf(file, "%u %u\n", &(*image)->column, &(*image)->raw) != 2)
    {
        fprintf(stderr, "Error Reading Column and Row\n");
        fclose(file);
        free(*image);
        return -3;
    }
    if((*image)->imageFormat == 2 || (*image)->imageFormat == 3){
        remove_comments(file);
    }
    fprintf(stdout, "Column: %u\nRow: %u\n", (*image)->column, (*image)->raw);
    if ((*image)->imageFormat == 3)
    {
        if (fscanf(file, "%u\n", &(*image)->colorValue) != 1)
        {
            fprintf(stderr, "Error Reading Color Value\n");
            fclose(file);
            free(*image);
            return -3;
        }
        if (check_color_value((*image)->imageFormat, (*image)->colorValue) == -3)
        {
            fclose(file);
            free(*image);
            return -3;
        }
    }
    (*image)->matrix = creat_matrix((*image)->raw, (*image)->column, (*image)->imageFormat);
    if (!(*image)->matrix)
    {
        fclose(file);
        free(*image);
        return -1;
    }
    if (scan_matrix(*image, file) == -1)
    {
        fclose(file);
        free(*image);
        return -3;
    }
    fclose(file);
    return 0;
}

int write_pnm(PNM *image, char *filename)
{
    assert(image != NULL && filename != NULL);
    FILE *file = fopen(filename, "w");
    if (!file)
    {
        fprintf(stderr, "Error Opening File\n");
        return -2;
    }
    fprintf(file, "P%u\n", image->imageFormat);
    fprintf(file, "%u %u\n", image->column, image->raw);
    if (image->imageFormat == 3 || image->imageFormat == 2)
    {
        fprintf(file, "%u\n", image->colorValue);
    }
    if (print_matrix(image, file) == -1)
    {
        fclose(file);
        return -2;
    }
    fclose(file);
    return 0;
}

void free_pnm(PNM *image)
{
    if (image)
    {
        for (unsigned int i = 0; i < image->raw; i++)
        {
            free(image->matrix[i]);
        }
        free(image->matrix);
        free(image);
    }
}