#include <stdio.h>
#include <stdlib.h>
#include "bmp8.h"
#include <math.h>

t_bmp8 * bmp8_loadImage(const char * filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Error opening file");
        return NULL;
    }

    t_bmp8 *img = (t_bmp8 *)malloc(sizeof(t_bmp8));
    if (!img) {
        perror("Error allocating memory for image");
        fclose(file);
        return NULL;
    }

    fread(img->header, 1, 54, file);

    img->width = *(unsigned int *)&img->header[18];
    img->height = *(unsigned int *)&img->header[22];
    img->colorDepth = *(unsigned short *)&img->header[28];
    img->dataSize = *(unsigned int *)&img->header[34];

    if (img->colorDepth != 8) {
        fprintf(stderr, "Error: Not an 8-bit grayscale image\n");
        fclose(file);
        free(img);
        return NULL;
    }

    fread(img->colorTable, 1, 1024, file);

    img->data = (unsigned char *)malloc(img->dataSize);
    if (!img->data) {
        perror("Error allocating memory for pixel data");
        fclose(file);
        free(img);
        return NULL;
    }

    fread(img->data, 1, img->dataSize, file);

    fclose(file);
    return img;
}

void bmp8_saveImage(const char * filename, t_bmp8 * img) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Error opening file for writing");
        return;
    }

    fwrite(img->header, 1, 54, file);

    fwrite(img->colorTable, 1, 1024, file);

    fwrite(img->data, 1, img->dataSize, file);

    fclose(file);
    printf("Image saved successfully to %s\n", filename);
}

void bmp8_free(t_bmp8 * img) {
    if (img) {
        if (img->data) {
            free(img->data);
        }
        free(img);
    }
}

void bmp8_printInfo(t_bmp8 * img) {
    if (!img) {
        printf("Image is NULL\n");
        return;
    }
    printf("Width: %u\n", img->width);
    printf("Height: %u\n", img->height);
    printf("Color Depth: %u\n", img->colorDepth);
    printf("Data Size: %u\n", img->dataSize);
}

void bmp8_negative(t_bmp8 * img) {
    if (!img || !img->data) {
        return;
    }
    for (unsigned int i = 0; i < img->dataSize; i++) {
        img->data[i] = 255 - img->data[i];
    }
}

void bmp8_brightness(t_bmp8 * img, int value) {
    if (!img || !img->data) return;

    for (unsigned int i = 0; i < img->dataSize; i++) {
        int newValue = img->data[i] + value;
        if (newValue < 0) {
            img->data[i] = 0;
        } else if (newValue > 255) {
            img->data[i] = 255;
        } else {
            img->data[i] = (unsigned char)newValue;
        }
    }
}

void bmp8_threshold(t_bmp8 * img, int threshold) {
    if (!img || !img->data) {
        return;
    }
    for (unsigned int i = 0; i < img->dataSize; i++) {
        img->data[i] = (img->data[i] >= threshold) ? 255 : 0;
    }
}

void bmp8_applyFilter(t_bmp8 * img, double * kernel, int kernelSize) {
    if (!img || !img->data || !kernel || kernelSize <= 0 || kernelSize % 2 == 0) {
        return;
    }
    unsigned int width = img->width;
    unsigned int height = img->height;
    unsigned char *newData = (unsigned char *)malloc(img->dataSize);
    if (!newData) {
        perror("Error allocating memory for filtered data");
        return;
    }

    int kernelOffset = kernelSize / 2;

    for (unsigned int y = 0; y < height; y++) {
        for (unsigned int x = 0; x < width; x++) {
            double sum = 0.0;
            for (int ky = -kernelOffset; ky <= kernelOffset; ky++) {
                for (int kx = -kernelOffset; kx <= kernelOffset; kx++) {
                    int sampleX = x + kx;
                    int sampleY = y + ky;

                    if (sampleX < 0) {
                        sampleX = 0;
                    }
                    if (sampleX >= width) {
                        sampleX = width - 1;
                    }
                    if (sampleY < 0) {
                        sampleY = 0;
                    }
                    if (sampleY >= height) {
                        sampleY = height - 1;
                    }

                    sum += img->data[sampleY * width + sampleX] * kernel[(ky + kernelOffset) * kernelSize + (kx + kernelOffset)];
                }
            }
            newData[y * width + x] = (unsigned char)fmax(0.0, fmin(255.0, sum));
        }
    }

    // To update the image data
    free(img->data);
    img->data = newData;
    newData = NULL; // To prevent freeing two times
}