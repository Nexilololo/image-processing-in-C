#ifndef BMP24_H
#define BMP24_H

#include <stdio.h>
#include <stdint.h>

// Constants for BMP header
#define BITMAP_MAGIC_24     0x4D42 // 'BM'
#define BITMAP_OFFSET_VAL_24 54    // Default offset for 24-bit BMP with no palette

// BMP File Header (14 bytes)
typedef struct __attribute__((__packed__)) {
    uint16_t type;
    uint32_t size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offset;
} t_bmp_header;

// BMP Info Header (BITMAPINFOHEADER - 40 bytes)
typedef struct __attribute__((__packed__)) {
    uint32_t size;
    int32_t  width;
    int32_t  height;
    uint16_t planes;
    uint16_t bits;
    uint32_t compression;
    uint32_t imagesize;
    int32_t  xresolution;
    int32_t  yresolution;
    uint32_t ncolors;
    uint32_t importantcolors;
} t_bmp_info;

typedef struct {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
} t_pixel;

typedef struct {
    t_bmp_header header;
    t_bmp_info info;
    t_pixel **data;
} t_bmp24;

// File I/O Helper functions (as provided in PDF, now part of this module)
void file_rawRead(uint32_t position, void *buffer, uint32_t size, size_t n, FILE *file);
void file_rawWrite(uint32_t position, void *buffer, uint32_t size, size_t n, FILE *file);

// Allocation and Deallocation
t_pixel **bmp24_allocateDataPixels(int width, int height);
void bmp24_freeDataPixels(t_pixel **pixels, int height);
t_bmp24 *bmp24_allocate(int width, int height, int colorDepth);
void bmp24_free(t_bmp24 *img);

// Main Loading and Saving (will use file_rawRead/Write internally)
t_bmp24 *bmp24_loadImage(const char *filename);
void bmp24_saveImage(const char *filename, t_bmp24 *img);
void bmp24_printInfo(t_bmp24 *img);

// Image Processing
void bmp24_negative(t_bmp24 *img);
void bmp24_grayscale(t_bmp24 *img);
void bmp24_brightness(t_bmp24 *img, int value);

// Convolution
// The kernel is passed as an argument, its definition will be in main.c
void bmp24_applyFilter(t_bmp24 *img, float kernel[3][3], int kernelSize);

// Histogram Equalization for Color Images
void bmp24_equalize(t_bmp24 *img);

#endif // BMP24_H