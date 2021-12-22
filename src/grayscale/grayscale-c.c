#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ppm/ppm.h"

extern uint32_t R, G, B;

extern void to_grayscale(uint32_t *matrix, uint32_t rows, uint32_t columns);

void print_usage(const char *executable_name) {
    fprintf(stderr, "Usage: %s filename [r g b] \n", executable_name);
    fprintf(stderr, " - use r g b options to specify color coefficients\n");
    fprintf(stderr, " - r + g + b must be equal 256\n");
    fprintf(stderr, " - filename must point to a valid PPM file\n\n");
    fprintf(stderr, "Example: %s image.ppm 64 64 128\n\n", executable_name);
    exit(2);
}

// Changes or adds .pgm extension to filepath
char *get_output_path(const char *input_path) {
    size_t input_path_len = strlen(input_path);

    char *output_path = calloc(input_path_len + 5, sizeof(char));
    if (output_path == NULL) {
        fprintf(stderr, "%s", strerror(errno));
        exit(1);
    }

    memcpy(output_path, input_path, input_path_len + 1);

    size_t last_dot_idx = input_path_len + 1;
    while (1) {
        char c = output_path[last_dot_idx];

        if (last_dot_idx == 0 || c == '/') {
            last_dot_idx = input_path_len + 1;
            output_path[last_dot_idx] = '.';
        }
        if (c == '.') {
            break;
        }
        last_dot_idx--;
    }

    output_path[last_dot_idx + 1] = 'p';
    output_path[last_dot_idx + 2] = 'g';
    output_path[last_dot_idx + 3] = 'm';
    output_path[last_dot_idx + 4] = '\0';

    return output_path;
}

uint32_t parse_color_parameter(const char *executable_name, const char *s) {
    if (s == NULL) {
        print_usage(executable_name);
        exit(1);
    }

    uint32_t v = 0;
    unsigned long len = strlen(s);
    for (unsigned long i = 0; i < len; i++) {
        if (s[i] > '9' || s[i] < '0') {
            print_usage(executable_name);
            exit(1);
        }
        v = v * 10 + s[i] - '0';
        if (v > 256) {
            print_usage(executable_name);
            exit(1);
        }
    }

    return v;
}

void parse_and_validate_color_arguments(int argc, char *argv[]) {
    if (argc == 2) {
        R = 77;
        G = 151;
        B = 28;
    } else if (argc == 5) {
        R = parse_color_parameter(argv[0], argv[2]);
        G = parse_color_parameter(argv[0], argv[3]);
        B = parse_color_parameter(argv[0], argv[4]);
        if (R + G + B != 256) {
            print_usage(argv[0]);
            exit(1);
        }
    } else {
        print_usage(argv[0]);
        exit(1);
    }
}

void read_ppm_image(const char *filepath, image_t *image) {
    FILE *input_file = fopen(filepath, "r");
    if (input_file == NULL) {
        fprintf(stderr, "%s", strerror(errno));
        exit(1);
    }

    int result = ppm_read(input_file, image);
    if (result < 0) {
        fprintf(stderr, "Invalid PPM read");
        exit(1);
    }
    result = fclose(input_file);
    if (result < 0) {
        fprintf(stderr, "%s", strerror(errno));
        exit(1);
    }
}

void write_pgm_image(const char *filepath, image_t *image) {
    FILE *output_file = fopen(filepath, "w");

    const int maxval = 255;

    int result = fprintf(output_file, "P2\n%d %d\n%d\n", image->width, image->height, maxval);
    if (result < 0) {
        fprintf(stderr, "Invalid PGM write");
        exit(1);
    }

    for (uint32_t r = 0; r < image->height; r++) {
        for (uint32_t i = 0; i < image->width; i++) {
            uint32_t val = image->pixels[r * image->width + i].r;
            char last_char = (i < image->width - 1) ? ' ' : '\n';
            int ret = fprintf(output_file, "%d%c", val, last_char);
            if (ret < 0) {
                fprintf(stderr, "Invalid PGM write");
                exit(1);
            }
        }
    }

    result = fclose(output_file);
    if (result < 0) {
        fprintf(stderr, "%s", strerror(errno));
        exit(1);
    }
}

int main(int argc, char *argv[]) {
    parse_and_validate_color_arguments(argc, argv);
    char *input_filename = argv[1];

    image_t image;
    read_ppm_image(input_filename, &image);
    if (image.width * image.height > 0) {
        to_grayscale((uint32_t *) image.pixels, image.height, image.width);
    }

    char *output_file_path = get_output_path(input_filename);
    write_pgm_image(output_file_path, &image);

    free(output_file_path);
    ppm_free(&image);
    return 0;
}
