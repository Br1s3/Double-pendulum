/* To use it:
 *
 *     #define GRAPHLIB_IMPLEMENTATION
 *     #include "graphlib.h"
 *
 * Basic usage:
 *
 *     #define GRAPHLIB_IMPLEMENTATION
 *     #include "graphlib.h"
 *
 *     #define HEIGHT (9*2)
 *     #define WIDTH (16*2)
 *
 *     int main()
 *     {
 *         GRAPHLIB_MALLOC2D(char, console, HEIGHT, WIDTH);
 *         PrintRectangle(console, WIDTH, HEIGHT, 0, 0, WIDTH, HEIGHT, '#');
 *         PrintRectangle(console, WIDTH, HEIGHT, 1, 1, WIDTH-2, HEIGHT-2, ' ');
 *         PrintCircle(console, WIDTH, HEIGHT, WIDTH/2, HEIGHT/2, 5, '@');
 *         PrintConsoleSpace(console, WIDTH, HEIGHT);
 *         GRAPHLIB_FREE2D(console, HEIGHT);
 *         return 0;
 *     }
 *
 * The result:
 * # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
 * #                                                             #
 * #                                                             #
 * #                                                             #
 * #                               @                             #
 * #                         @ @ @ @ @ @ @                       #
 * #                       @ @ @ @ @ @ @ @ @                     #
 * #                       @ @ @ @ @ @ @ @ @                     #
 * #                       @ @ @ @ @ @ @ @ @                     #
 * #                     @ @ @ @ @ @ @ @ @ @ @                   #
 * #                       @ @ @ @ @ @ @ @ @                     #
 * #                       @ @ @ @ @ @ @ @ @                     #
 * #                       @ @ @ @ @ @ @ @ @                     #
 * #                         @ @ @ @ @ @ @                       #
 * #                               @                             #
 * #                                                             #
 * #                                                             #
 * # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
 *
 *
 */

#ifndef GLIPH_H_INCLUED
#define GLIPH_H_INCLUED
#include <stdio.h>  // Used for: fprintf(), printf(), putchar()
#include <stdlib.h> // Used for: exit(), malloc(), free()
#include <string.h> // Used for: strerror()
#include <errno.h>  // Used for: errno
#include <stdint.h> // Used for: uint8_t, uint32_t


// #define max(x, y) ((x)<(y) ? (y) : (x))
// #define min(x, y) ((x)>(y) ? (y) : (x))
#define MOVETO_GLIPH(y, x) printf("\033[%d;%dH", (y), (x))
#define DEC_GLIPH(x, y) (((x)*(width+1)) + (y))
#define ABS_GLIPH(x) ((x) < 0 ? (-x) : (x))

#define GLIPH_MALLOC2D(type, name, W, H)                  \
type **name = (type **)malloc(sizeof(type *) * ((H)+1));  \
TESTMALLOC(name);                                         \
do {                                                      \
    for (ssize_t i = 0; i < ((H)); i++) {                 \
        name[i] = (type *)malloc(sizeof(type)*((W) + 1)); \
        TESTMALLOC(name[i]);                              \
    }                                                     \
    name[(H)] = NULL;                                     \
} while (0)

#define GLIPH_FREE2D(name, H)                                                    \
if (name == NULL) fprintf(stdout, "WARNING: %s is NULL, %d\n", #name, __LINE__); \
do {                                                                             \
    for (ssize_t i = 0; i < (H); i++) free(name[i]);                             \
    free(name);                                                                  \
} while (0)

#define TESTMALLOC(x)                                                          \
do                                                                             \
{                                                                              \
    if (x == NULL) {                                                           \
        fprintf(stderr, "ERROR: %s, ligne : %d\n", strerror(errno), __LINE__); \
        exit(EXIT_FAILURE);                                                    \
    }                                                                          \
} while(0)


typedef struct
{
    int x, y;
} GLIPH_COORD;

typedef struct
{
    float x, y;
} GLIPH_COORDF;


void ConsoleClear(char **pixels, short width, short height, const char clear);

void PrintRectangle(char **pixels, short width, short height, int x, int y, int largeur, int hauteur, const char fd);
void PrintCircle(char **pixels, short width, short height, int x, int y, int radius, const char fd);
void PrintLine(char **pixels, const short width, const short height, int ax, int ay, int bx, int by, const char fd);
void PrintTriangle(char **pixels, short width, short height, int ax, int ay, int bx, int by, int cx, int cy, const char fd);

void PrintConsole(char **pixels, short width, short height);
void PrintConsoleSpace(char **pixels, short width, short height);
void PrintConsolePadded(char **pixels, short width, short height);


void ClearDrawing(uint8_t ***pixels, short width, short height, const uint32_t fd);

void DrawLine(uint8_t ***pixels, short width, short height, int ax, int ay, int bx, int by, const uint32_t fd);
void DrawCircle(uint8_t ***pixels, short width, short height, int x, int y, int radius, const uint32_t fd);


# ifdef GLIPH_IMPLEMENTATION

void PrintConsole(char **pixels, short width, short height)
{
    short i, j;
    char pixels_1D[(width+1) * height];
    for (i = 0; i < height; ++i) {
	for (j = 0; j < width+1; ++j) {
	    // TODO: Use a static variable to test if &pixels[i][j] == NULL
            if      (j < width)     pixels_1D[DEC_GLIPH(i, j)] = pixels[i][j];
            else if (i < height-1)  pixels_1D[DEC_GLIPH(i, j)] = '\n';
            else                    pixels_1D[DEC_GLIPH(i, j)] = '\0';
	}
    }

    MOVETO_GLIPH(0, 0);
    puts(pixels_1D);
}

void PrintConsoleSpace(char **pixels, short width, short height)
{
    short i, j;
    MOVETO_GLIPH(0, 0);
    for (i = 0; i < height; ++i) {
	for (j = 0; j < width; ++j) {
	    // TODO: Use a static variable to test if &pixels[i][j] == NULL
	    putchar(pixels[i][j]);
	    putchar(' ');
	}
	putchar('\n');
    }
}

void PrintConsolePadded(char **pixels, short width, short height)
{
    short i, j;
    MOVETO_GLIPH(0, 0);
    for (i = 0; i < height; ++i) {
	for (j = 0; j < width; ++j) {
	    // TODO: Use a static variable to test if &pixels[i][j] == NULL
	    putchar(pixels[i][j]);
	    putchar(pixels[i][j]);
	}
	putchar('\n');
    }
}

void ConsoleClear(char **pixels, short width, short height, const char clear)
{
    short i, j;
    for (i = 0; i < height; ++i) {
	for (j = 0; j < width; j++) {
	    // TODO: Use a static variable to test if &pixels[i][j] == NULL
            pixels[i][j] = clear;
	}
    }
}

void PrintRectangle(char **pixels, short width, short height, int x, int y, int largeur, int hauteur, const char fd)
{
    short i, j;
    for (i = 0; i < height; i++) {
	for (j = 0; j < width; j++) {
	    // TODO: Use a static variable to test if &pixels[i][j] == NULL
            if (((j >= x) && (j < x+largeur)) && ((i >= y) && (i < y+hauteur)))
		pixels[i][j] = fd;
        }
    }
}


void PrintLine(char **pixels, const short width, const short height, int ax, int ay, int bx, int by, const char fd)
{
    GLIPH_COORDF a, b, AB;
    const float midH = (float)height/2.f;
    const float midW = (float)width/2.f;
    a.x = (float)ax/(midW);
    a.y = (float)ay/(-midH);
    b.x = (float)bx/(midW);
    b.y = (float)by/(-midH);
    AB.x = b.x - a.x;
    AB.y = b.y - a.y;

    double x, y;
/* TODO: Modify t step to verify this condition: (step >= 1/sqrt((by-ay)² + (bx-ax)²)) */
       // double dt = 1.f/sqrt((bx-ax)*(bx-ax) + (by-ay)*(by-ay));
       // // double dt = 1.f/((ABS_GLIPH(bx-ax) + ABS_GLIPH(by-ay))*0.7);
       // for (double t = 0; t < 1; t+=dt) {

    for (double t = 0; t < 1; t+=0.01) {
        x = (AB.x*t + a.x);
        y = (AB.y*t + a.y);
        if ((int)(ABS_GLIPH(x*midW)) > midW-1 || (int)(ABS_GLIPH(y*midH)) > midH-1) break;
	pixels[(int)((midH)*(1 - y))][(int)((midW)*(1 + x))] = fd;
    }
}

void PrintCircle(char **pixels, short width, short height, int x, int y, int radius, const char fd)
{
    /* O(n*m) avec n = height & m = width */
    short i, j;
    for (i = 0; i < height; i++) {
	for (j = 0; j < width; j++) {
	    if ((i-y)*(i-y) + (j-x)*(j-x) <= radius*radius)
		pixels[i][j] = fd;
	}
    }
}

// #include <math.h>
// void PrintDisk(char **pixels, short width, short height, int x, int y, int radius, const char fd)
// {
//     /* O(n) avec n = 8*radius */
//     for (double t = 0; t <= 2.f*M_PI; t+=M_PI/(double)(4*radius)) {
// 	int XX = (int)((double)radius*cos(t)) + x;
// 	int YY = (int)((double)radius*sin(t)) + y;

// 	if (XX < 0 || XX > width-1 || YY < 0 || YY > height-1) continue;
// 	pixels[YY][XX] = fd;
//     }
// }


#ifdef TODO
void RotateConsole(char **pixels, short width, short height, float tetha)
{
   RECT wind;
}
#endif

void PrintTriangle(char **pixels, short width, short height, int ax, int ay, int bx, int by, int cx, int cy, const char fd)
{
    GLIPH_COORDF a, b, c;
    a.x = ax;
    a.y = ay;
    b.x = bx;
    b.y = by;
    c.x = cx;
    c.y = cy;

    float gA, gB, gC;

    short x, y;
    for (y = 0; y < height; y++) {
	for (x = 0; x < width; x++) {
	    gA = ((float)x*(b.y - c.y) + b.x*(c.y - (float)y) + c.x*((float)y - b.y))/(a.x*(b.y - c.y) + b.x*(c.y - a.y) + c.x*(a.y - b.y));
	    gB = (a.x*((float)y - c.y) + (float)x*(c.y - a.y) + c.x*(a.y - (float)y))/(a.x*(b.y - c.y) + b.x*(c.y - a.y) + c.x*(a.y - b.y));
	    gC = (a.x*(b.y - (float)y) + b.x*((float)y - a.y) + (float)x*(a.y - b.y))/(a.x*(b.y - c.y) + b.x*(c.y - a.y) + c.x*(a.y - b.y));

            if(gA >= 0 && gB >= 0 && gC >= 0)
                pixels[y][x] = fd;
       }
   }
}


void ClearDrawing(uint8_t ***pixels, short width, short height, const uint32_t fd)
{
    short i, j;
    for (i = 0; i < height; ++i) {
	for (j = 0; j < width; j++) {
            pixels[i][j][0] = fd;
            pixels[i][j][1] = fd;
            pixels[i][j][2] = fd;
	}
    }
}

void DrawLine(uint8_t ***pixels, short width, short height, int ax, int ay, int bx, int by, const uint32_t fd)
{
    GLIPH_COORDF a, b, AB;
    const float midH = width/2;
    const float midW = height/2;
    a.x = (float)ax/(midH);
    a.y = (float)ay/(-midW);
    b.x = (float)bx/(midH);
    b.y = (float)by/(-midW);
    AB.x = b.x - a.x;
    AB.y = b.y - a.y;
    // TODO: Modify t step with 3 if statement if (sqrt(h² + w²) > 1000) t+=0.0001 else reduce

    double x, y;
    for (double t = 0; t < 1; t+=0.01) {
	x = (AB.x*t + a.x);
	y = (AB.y*t + a.y);
        if (ABS_GLIPH(x*(midH)) > midH-1 || ABS_GLIPH(y*(midW)) > midW-1) break;
        // pixels[(int)(-y*(midW)) + midW][(int)(x*midH) + midH][0] = fd>>(24); // 8*3
        pixels[(int)(midW*(1 - y))][(int)(midH*(1 + x))][0] = fd>>(24); // 8*3
        // pixels[(int)(-y*(midW)) + midW][(int)(x*midH) + midH][1] = fd>>(16); // 8*2
        pixels[(int)(midW*(1 - y))][(int)(midH*(1 + x))][1] = fd>>(16); // 8*2
        // pixels[(int)(-y*(midW)) + midW][(int)(x*midH) + midH][2] = fd>>(1); // 8*1
        pixels[(int)(midW*(1 - y))][(int)(midH*(1 + x))][2] = fd>>(8); // 8*1
    }
}

void DrawCircle(uint8_t ***pixels, short width, short height, int x, int y, int radius, const uint32_t fd)
{
    short i, j;
    for (i = 0; i < height; i++) {
	for (j = 0; j < width; j++) {
            if ((i-y)*(i-y) + (j-x)*(j-x) <= radius*radius) {
		pixels[i][j][0] |= fd>>(24); // 8*3
		pixels[i][j][1] |= fd>>(16); // 8*2
		pixels[i][j][2] |= fd>>(8); // 8*1
            }
	}
    }
}

# endif // GLIPH_IMPLEMENTATION
#endif // GLIPH_H_INCLUED

/***********************************
TODO:
- Use a static variable to test if &pixels[i][j] == NULL like:
    // if (&pixels[i][j] == NULL) {
        //     fprintf(stderr, "ERROR: Out of memorie\n");
        //     exit(1);
    // }
- Modify t step with 3 if statement if (sqrt(h² + w²) > 1000) t+=0.0001 else reduce
in PrintLine() and DrawLine()
- Add DrawRectangle()
- Do something with PrintDisk line: 230
***********************************/
