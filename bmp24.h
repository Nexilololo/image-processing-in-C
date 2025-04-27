#ifndef BMP24_H
#define BMP24_H

#include <stdio.h>

typedef struct {
    unsigned char header[54];
    unsigned char *data;
    unsigned int width;
    unsigned int height;
    unsigned int colorDepth;
    unsigned int dataSize;
} t_bmp24;

typedef struct {
    double y;
    double u;
    double v;
} t_yuv;

t_bmp24 * bmp24_loadImage(const char * filename);
void bmp24_saveImage(const char * filename, t_bmp24 * img);
void bmp24_free(t_bmp24 * img);
void bmp24_printInfo(const t_bmp24 * img);

void bmp24_negative(t_bmp24 * img);
void bmp24_brightness(t_bmp24 * img, int value);
void bmp24_applyFilter(t_bmp24 * img, double * kernel, int kernelSize);
void bmp24_equalize(t_bmp24 * img);

#endif //BMP24_H
