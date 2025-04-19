#include <stdio.h>
#include "bmp8.h"
#include <stdlib.h>
t_bmp8 * bmp8_loadImage(const char * filename){

    FILE *filePtr = NULL;
    t_bmp8 *img = NULL;
    size_t readCount;

    filePtr = fopen(filename, "rb");
    if (filePtr == NULL) {
        perror("Error opening file");
        return NULL;
    }

    img = (t_bmp8 *)malloc(sizeof(t_bmp8));
    if (img == NULL) {
        fprintf(stderr, "Error: Failed to allocate memory for t_bmp8 structure.\n");
        fclose(filePtr);
        return NULL;
    }
    img->data = NULL;

    readCount = fread(img->header, sizeof(unsigned char), 54, filePtr);
    if (readCount != 54) {
        fprintf(stderr, "Error: Failed to read BMP header.\n");
        free(img);
        fclose(filePtr);
        return NULL;
    }

    img->width = *(unsigned int *)&img->header[18];
    img->height = *(unsigned int *)&img->header[22];
    img->colorDepth = *(unsigned short *)&img->header[28];
    img->dataSize = *(unsigned int *)&img->header[34];

    if (img->header[0] != 'B' || img->header[1] != 'M') {
         fprintf(stderr, "Error: Not a valid BMP file (invalid signature).\n");
         free(img);
         fclose(filePtr);
         return NULL;
    }

    if (img->colorDepth != 8) {
        fprintf(stderr, "Error: Image is not 8-bit grayscale (colorDepth = %u).\n", img->colorDepth);
        free(img);
        fclose(filePtr);
        return NULL;
    }

    if (img->dataSize == 0) {
        img->dataSize = img->width * img->height;
        *(unsigned int *)&img->header[34] = img->dataSize;
        printf("Warning: dataSize was 0 in header, calculated as %u.\n", img->dataSize);
    } else if (img->dataSize != img->width * img->height) {
         printf("Warning: Header dataSize (%u) differs from width*height (%u). May indicate padding.\n",
                img->dataSize, img->width * img->height);
    }

    readCount = fread(img->colorTable, sizeof(unsigned char), 1024, filePtr);
     if (readCount != 1024) {
        fprintf(stderr, "Error: Failed to read BMP color table.\n");
        free(img);
        fclose(filePtr);
        return NULL;
    }

    img->data = (unsigned char *)malloc(img->dataSize * sizeof(unsigned char));
    if (img->data == NULL) {
        fprintf(stderr, "Error: Failed to allocate memory for pixel data (size %u).\n", img->dataSize);
        free(img);
        fclose(filePtr);
        return NULL;
    }

    readCount = fread(img->data, sizeof(unsigned char), img->dataSize, filePtr);
    if (readCount != img->dataSize) {
        fprintf(stderr, "Error: Failed to read pixel data (read %zu bytes, expected %u).\n", readCount, img->dataSize);
        free(img->data);
        free(img);
        fclose(filePtr);
        return NULL;
    }

    fclose(filePtr);

    printf("Image '%s' loaded successfully (%u x %u, %u-bit).\n", filename, img->width, img->height, img->colorDepth);
    return img;

}

void bmp8_saveImage(const char * filename, t_bmp8 * img){

}

void bmp8_free(t_bmp8 * img){

}

void bmp8_printInfo(t_bmp8 * img){

}