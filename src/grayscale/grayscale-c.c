#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ppm/ppm.h"

extern void grayscale(uint32_t *matrix, uint32_t rows, uint32_t columns);

extern uint32_t R_, G_, B_;

/* Checks whether a file path ends with .ppm extension */
bool ends_with_ppm(char *path) {
    size_t s = strlen(path);
    if (s < 4) return false;

    char* ppm = ".ppm";

    for (int i = 1; i <= 4; i++) 
        if (ppm[4 - i] != path[s - i]) return false;

    return true;
}

/* Validates input. If necessary overwrites R_, G_, B_ coefficients */
void validate_input(int argc, char *argv[])
{
    if (argc == 2) {
        R_ = 77;
        G_ = 151;
        B_ = 28;
    }
    else if (argc == 5) {
        R_ = atoi(argv[2]);
        G_ = atoi(argv[3]);
        B_ = atoi(argv[4]);
    }
    else {
        printf("Invalid usage. Correct usage is ./grayscale 'image'\n");
        exit(1);
    }

    if (!ends_with_ppm(argv[1])) {
        printf("Invalid usage. Image path should end with '.ppm'\n");
        exit(1);
    }
    
}

/* Creates path to .pgm file based on path to a .ppm file */
char *create_pgm(char *ppm)
{
    size_t s = strlen(ppm);

    char *pgm = malloc(sizeof(char) * s);
    strcpy(pgm, ppm);

    /* Change .ppm to .pgm */
    pgm[s - 2] = 'g';
    return pgm;
}

/** 
 * This function is based on ppm_write but instead of printing RGB values it prints only one (R)
 */
int pgm_write(FILE *file, image_t *image)
{
    int tmp = fprintf(file, "P2\n%d %d\n%d\n", image->width, image->height, 255);

    if (tmp < 0)
        return tmp;

    for (int row = 0; row < image->height; row++) {
        for (int col = 0; col < image->width; col++) {
            pixel_t p = image->pixels[row * image->width + col];

            if (col == image->width - 1) 
                tmp = fprintf(file, "%d\n", p.r);
            else 
                tmp = fprintf(file, "%d ", p.r);

            if (tmp < 0) 
                return tmp;
        }
    }

    return 0;
}

int main(int argc, char *argv[])
{
    image_t image;
    validate_input(argc, argv);
    char *ppm_image_path = argv[1];
    FILE *ppm_file = fopen(ppm_image_path, "r");
    ppm_read(ppm_file, &image);

    grayscale((uint32_t *)image.pixels, image.height, image.width);
    printf("Values:\nR = %d\nG = %d\nB = %d\n", R_, G_, B_);

    char *pgm_image_path = create_pgm(ppm_image_path);
    FILE *pgm_file = fopen(pgm_image_path, "w");
    pgm_write(pgm_file, &image);
    
    ppm_free(&image);
    free(pgm_image_path);
    return 0;
}
