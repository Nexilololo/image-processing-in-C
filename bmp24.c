#include <stdio.h>
#include <stdlib.h>
#include "bmp24.h"
#include <math.h>

t_bmp24 * bmp24_loadImage(const char * filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("Error while opening file");
        return NULL;
    }

    t_bmp24 *img = (t_bmp24 *)malloc(sizeof(t_bmp24));
    if (!img) {
        printf("Error while allocating memory for image");
        fclose(file);
        return NULL;
    }

    // Read the BMP header
    fread(img->header, 1, 54, file);

    // Extract image information
    img->width = *(unsigned int *)&img->header[18];
    img->height = *(unsigned int *)&img->header[22];
    img->colorDepth = *(unsigned short *)&img->header[28];
    img->dataSize = *(unsigned int *)&img->header[34];

    // Check if it's a 24-bit image
    if (img->colorDepth != 24) {
        fprintf(stderr, "Error: Not a 24-bit image\n");
        fclose(file);
        free(img);
        return NULL;
    }

    // Allocate memory for image data
    img->data = (unsigned char *)malloc(img->dataSize);
    if (!img->data) {
        printf("Error while allocating memory for pixel data");
        fclose(file);
        free(img);
        return NULL;
    }

    // Read the pixel data
    fread(img->data, 1, img->dataSize, file);

    fclose(file);
    return img;
}

void bmp24_saveImage(const char * filename, t_bmp24 * img) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        printf("Error while opening file for writing");
        return;
    }

    // Write the BMP header
    fwrite(img->header, 1, 54, file);

    // Write the pixel data
    fwrite(img->data, 1, img->dataSize, file);

    fclose(file);
    printf("Image saved successfully to %s\n", filename);
}

void bmp24_free(t_bmp24 * img) {
    if (img) {
        if (img->data) {
            free(img->data);
        }
        free(img);
    }
}

void bmp24_printInfo(const t_bmp24 * img) {
    if (!img) {
        printf("Image is NULL\n");
        return;
    }

    printf("Width: %u\n", img->width);
    printf("Height: %u\n", img->height);
    printf("Color Depth: %u\n", img->colorDepth);
    printf("Data Size: %u\n", img->dataSize);
}

void bmp24_negative(t_bmp24 * img) {
    if (!img || !img->data) return;

    for (unsigned int i = 0; i < img->dataSize; i++) {
        img->data[i] = 255 - img->data[i];
    }
}

void bmp24_brightness(t_bmp24 * img, int value) {

}

void bmp24_applyFilter(t_bmp24 * img, double * kernel, int kernelSize) {

}

void bmp24_equalize(t_bmp24 * img) {

}