/**
 * @file main.c
 * @brief Main file for the project.
 *
 * This file contains the main function of the project.
 *
 * @author: KATOUZIAN Pouria S192865
 * @date: 10/02/2021
 * @projet: INFO0030 Projet 1
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <getopt.h>
#include <stdbool.h>
#include "pnm.h"

static char *convert_to_lowercases(const char *string)
{
    if (!string)
    {
        fprintf(stderr, "Error: String is NULL\n");
        return NULL;
    }

    char *lowercase = malloc(strlen(string) + 1); // +1 for the null terminator
    if (!lowercase)
    {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return NULL;
    }

    for (unsigned int i = 0; i < strlen(string); i++)
    {
        lowercase[i] = tolower(string[i]);
    }

    lowercase[strlen(string)] = '\0';
    return lowercase;
}

static int detect_filename(const char *filename)
{
    assert(filename != NULL);
    const char forbidden[] = "<>/:;<>!|{}[]()^!'\"?";
    for (unsigned int i = 0; i < strlen(forbidden); i++)
    {
        if (strchr(filename, forbidden[i]))
        {
            fprintf(stderr, "Error: Forbidden character in filename\n");
            return -1;
        }
    }
    return 0;
}

static int check_inputeFormat(const char *string)
{
    assert(string != NULL);
    return (!strcmp(string, "pbm") || !strcmp(string, "pgm") || !strcmp(string, "ppm")) ? 0 : -2;
}

int main(int argc, char *argv[])
{
    char *optstring = "hf:i:o:";
    char *input = NULL;
    char *output = NULL;
    char *fileFormat = NULL;
    int option;
    bool flag = false;

    while ((option = getopt(argc, argv, optstring)) != -1)
    {
        switch (option)
        {
        case 'h':
            fprintf(stdout, "Usage: %s -f <format> -i <input> -o <output>\n", argv[0]);
            flag = true;
            break;
        case 'f':
            fileFormat = convert_to_lowercases(optarg);
            if (check_inputeFormat(fileFormat) == -2)
            {
                fprintf(stderr, "Error: Invalid format\n");
                free(fileFormat);
                return -2;
            }
            break;
        case 'i':
            input = optarg;
            if (detect_filename(input) == -1)
            {
                free(fileFormat);
                return -2;
            }
            break;
        case 'o':
            output = optarg;
            if (output == NULL)
            {
                fprintf(stderr, "Error: Invalid output\n");
                free(fileFormat);
                return -2;
            }
            else
            {
                fprintf(stdout, "Output file: %s\n", output);
            }
            break;
        default:
            fprintf(stderr, "Error: Invalid option\n");
            free(fileFormat);
            return -2;
        }
    }

    if ((input == NULL || output == NULL || fileFormat == NULL) && flag == false)
    {
        fprintf(stderr, "Error: Missing required arguments (-f, -i, -o).\n");
        free(fileFormat);
        return -2;
    }
    else if (flag == true)
    {
        free(fileFormat);
        return -2;
    }

    if (strlen(fileFormat) > strlen(input) || strlen(fileFormat) > strlen(output))
    {
        fprintf(stderr, "Error: File format length exceeds file name length.\n");
        free(fileFormat);
        return -2;
    }

    char *checkInput = input + strlen(input) - strlen(fileFormat);
    char *checkOutput = output + strlen(output) - strlen(fileFormat);

    PNM *image = NULL;
    if (!strcmp(checkInput, fileFormat) && !strcmp(checkOutput, fileFormat))
    {
        if (load_pnm(&image, input) == 0)
        {
            fprintf(stdout, "Image loaded successfully\n");

            if (write_pnm(image, output) == 0)
            {
                fprintf(stdout, "Image written successfully\n");
            }
            else
            {
                fprintf(stderr, "Error: Image writing failed\n");
                free_pnm(image);
                free(fileFormat);
                return -2;
            }
        }
        else
        {
            fprintf(stderr, "Error: Image loading failed\n");
            free(fileFormat);
            return -2;
        }
    }
    else
    {
        fprintf(stderr, "Error: Mismatched file formats\n");
        free(fileFormat);
        return -2;
    }

    free_pnm(image);
    free(fileFormat);
    fprintf(stdout, "Program executed successfully\n");
    return 0;
}
