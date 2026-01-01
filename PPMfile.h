#ifndef PPMFILE_INCLUED_H
#define PPMFILE_INCLUED_H
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

uint8_t ***color_alloc(int w, int h);
int color_free(uint8_t ***tab, int w, int h);
void CreateImage(const char *nFile, uint8_t ***color, uint32_t Width, uint32_t Height);

#endif // PPMFILE_INCLUED_H
