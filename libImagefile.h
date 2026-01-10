#ifndef IMAGEFILE_INCLUED_H
#define IMAGEFILE_INCLUED_H
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
// #include <string.h>
#include "libImagefile.h"


uint8_t ***b24_color_alloc(int w, int h);
int b24_color_free(uint8_t ***tab, int w, int h);

uint8_t ***b8_color_alloc(int w, int h);
int b8_color_free(uint8_t ***tab, int w, int h);

typedef struct {
   char ID[2];
} Signature;

typedef struct {
   uint32_t SizeBMP;
   uint32_t reserved;
   uint32_t Offcet;
} BMPHeader;

typedef struct {
   uint32_t SizeDIB;
   uint32_t Width;
   uint32_t Height;
   uint16_t Plane;
   uint16_t ColorBits;
   uint32_t Compression;
   uint32_t Sizedata;
   uint32_t ResolH;
   uint32_t ResolV;
   uint32_t colorUsed;
   uint32_t colorImportant;
} DIBHeader;

void CreateImagePPM24b(const char *nFile, uint8_t ***color, uint32_t Width, uint32_t Height);
void CreateImagePPM8b(const char *nFile, uint8_t ***color, uint32_t Width, uint32_t Height);

void CreateImageBMP24b(const char *nFile, uint8_t ***color, uint32_t Width, uint32_t Height);

uint8_t ***ReadImage(const char *path, int *w, int *h, int *nb_color);

# ifdef LIBIMAGEFILE_IMPLEMENTATION

uint8_t ***b24_color_alloc(int w, int h)
{
    uint8_t ***color = (uint8_t ***)malloc(sizeof(uint8_t **)*h);
    for (int i = 0; i < h; i++) {
    	color[i] = (uint8_t **)malloc(sizeof(uint8_t *)*w);
    	for (int j = 0; j < w; j++) {
    	    color[i][j] = (uint8_t *)malloc(sizeof(uint8_t)*3);
    	}
    }
    return color;
}

int b24_color_free(uint8_t ***tab, int w, int h)
{
    if (tab == NULL) return -1;
    for (int i = 0; i < h; i++) {
	for (int j = 0; j < w; j++) {
	    free(tab[i][j]);
	}
	free(tab[i]);
    }
    free(tab);
    return 0;
}

void CreateImagePPM24b(const char *nFile, uint8_t ***color, uint32_t Width, uint32_t Height)
{
    FILE *f = fopen(nFile, "wb");
    if (f == NULL) {
	fprintf(stderr, "ERROR: could not open file %s: %m\n", nFile);
	exit(1);
    }
    
    fprintf(f, "P6\n");
    fprintf(f, "%d %d\n", Width, Height);
    fprintf(f, "255\n");

    for (uint32_t y = 0; y < Height; y++) {
	for (uint32_t x = 0; x < Width; x++) {
	    fputc(color[y][x][0], f);
	    fputc(color[y][x][1], f);
	    fputc(color[y][x][2], f);
	}
    }
    fclose(f);
}

void CreateImageBMP24b(const char *nFile, uint8_t ***color, uint32_t Width, uint32_t Height)
{
    Signature *id = (Signature *)malloc(sizeof(Signature) + 1);
    BMPHeader *bmph = (BMPHeader *)malloc(sizeof(BMPHeader) + 1);
    DIBHeader *dibh = (DIBHeader *)malloc(sizeof(DIBHeader) + 1);

    if (!dibh || !bmph) {
	fprintf(stderr, "ERROR : Allocation of dibh or bmph");
	exit(1);
    }
    uint8_t rowBytes = Width*3;
    uint8_t padding = (4 - (rowBytes % 4)) % 4;

    id->ID[0] = 'B';
    id->ID[1] = 'M';


    dibh->SizeDIB = 40;

    dibh->Width = Width;
    dibh->Height = Height;

    dibh->Plane = 1;

    dibh->ColorBits = 24;
    dibh->Compression = 0;
    dibh->Sizedata = (rowBytes + padding)*Height;
    dibh->ResolH = 0;
    dibh->ResolV = 0;
    dibh->colorUsed = 0;
    dibh->colorImportant = 0;

    bmph->SizeBMP = (rowBytes + padding)*Height + 54;

    bmph->reserved = 0;
    bmph->Offcet = 14 + 40;

    FILE *File = fopen(nFile, "wb");
    if (File == NULL) {
	fprintf(stderr, "ERROR: could not open file %s: %m\n", nFile);
	exit(1);
    }


    fwrite(id, sizeof(Signature), 1, File);
    fwrite(bmph, sizeof(BMPHeader), 1, File);
    fwrite(dibh, sizeof(DIBHeader), 1, File);


    // int ecart = Width;
    // while (ecart >= 4) {
    // 	ecart = ecart - 4;
    // }

    // uint8_t *padding = (uint8_t *)calloc(ecart, sizeof(uint8_t));

    uint32_t i, j;
    for (i = 0; i < Height; i++) {
	for (j = 0; j < Width; j++) {
	    // fwrite(color[i][j], sizeof(uint8_t), 3, File);
	    fputc(color[Height-i-1][j][2], File);
	    fputc(color[Height-i-1][j][1], File);
	    fputc(color[Height-i-1][j][0], File);
	}
	// fputc(padding, File);
	// fwrite(padding, sizeof(uint8_t), ecart, File);
    }

    fclose(File);

    // free(padding);
    free(id);
    free(bmph);
    free(dibh);
}


uint8_t ***b8_color_alloc(int w, int h)
{
    uint8_t ***color = (uint8_t ***)malloc(sizeof(uint8_t **)*h);
    for (int i = 0; i < h; i++) {
    	color[i] = (uint8_t **)malloc(sizeof(uint8_t *)*w);
    	for (int j = 0; j < w; j++) {
    	    color[i][j] = (uint8_t *)malloc(sizeof(uint8_t));
    	}
    }
    return color;
}

int b8_color_free(uint8_t ***tab, int w, int h)
{
    if (tab == NULL) return -1;
    for (int i = 0; i < h; i++) {
	for (int j = 0; j < w; j++) {
	    free(tab[i][j]);
	}
	free(tab[i]);
    }
    free(tab);
    return 0;
}


void CreateImagePPM8b(const char *nFile, uint8_t ***color, uint32_t Width, uint32_t Height)
{
    FILE *f = fopen(nFile, "wb");
    if (f == NULL) {
	fprintf(stderr, "ERROR: could not open file %s: %m\n", nFile);
	exit(1);
    }
    
    fprintf(f, "P5\n");
    fprintf(f, "%d %d\n", Width, Height);
    fprintf(f, "255\n");

    for (uint32_t y = 0; y < Height; y++) {
	for (uint32_t x = 0; x < Width; x++) {
	    fputc(color[y][x][0], f);
	}
    }
    fclose(f);
}


uint8_t ***ReadImage(const char *path, int *w, int *h, int *nb_color)
{
    FILE *f = fopen(path, "rb");
    if (f == NULL) {
	fprintf(stderr, "ERROR: To read the file: %s\n", path);
	return NULL;
    }

    char P;
    int signature;
    fscanf(f, "%c%d\n", &P, &signature);
    if (P != 'P') {
	fprintf(stderr, "ERROR: Unkown format\n");
	fclose(f);
	return NULL;
    }
    
    int f_width, f_height;
    fscanf(f, "%d %d\n", &f_width, &f_height);
    
    char canva[3];
    fscanf(f, "%s\n", canva);
    // Debug
    // printf("sig: P%d\nsize: %d %d\ncanva: %s\n", signature, f_width, f_height, canva);

    (*w) = f_width;
    (*h) = f_height;

    uint8_t ***data = NULL;
    if (signature == 6) {
	*nb_color = 3;
	data = b24_color_alloc(f_width, f_height);
    }
    else if (signature == 5) {
	data = b8_color_alloc(f_width, f_height);
	*nb_color = 1;
    }
    else {
	fprintf(stderr, "ERROR: No readable format\n");
	return NULL;
    }

    for (int i = 0; i < f_height; i++) {
	for (int j = 0; j < f_width; j++) {
	    for (int c = 0; c < *nb_color; c++) {
		uint8_t row_data;
		if (fread(&row_data, 1, sizeof(uint8_t), f) < 1) {
		    fprintf(stderr, "ERROR: To read the f\n");
		    fclose(f);
		    return NULL;
		}

		data[i][j][c] = row_data;
	    }
	}
    }
    fclose(f);
    return data;
}


# endif // LIBIMAGEFILE_IMPLEMENTATION
#endif // IMAGEFILE_INCLUED_H
