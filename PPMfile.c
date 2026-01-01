#include "PPMfile.h"


uint8_t ***color_alloc(int w, int h)
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

int color_free(uint8_t ***tab, int w, int h)
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

void CreateImage(const char *nFile, uint8_t ***color, uint32_t Width, uint32_t Height)
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
