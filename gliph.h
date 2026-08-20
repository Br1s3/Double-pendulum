/* To use it:
 *
 *     #define GLIPH_IMPLEMENTATION
 *     #include "gliph.h"
 *
 * Basic usage:
 *
 *     #define GLIPH_IMPLEMENTATION
 *     #include "gliph.h"
 *
 *     #define HEIGHT (9*2)
 *     #define WIDTH (16*2)
 *
 *     int main()
 *     {
 *         GLIPH_INIT(console, WIDTH, HEIGHT);
 *         ConsoleClear(console, ' ');
 *         PrintRectangle(console, 0, 0, WIDTH, HEIGHT, '#');
 *         PrintRectangle(console, 1, 1, WIDTH-2, HEIGHT-2, ' ');
 *         PrintCircle(console, WIDTH/2, HEIGHT/2, 5, '@');
 *         PrintConsole(console);
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
#include <unistd.h> // Used for: write()

// #define MAX_GLIPH(x, y) ((x)<(y) ? (y) : (x))
// #define MIN_GLIPH(x, y) ((x)>(y) ? (y) : (x))
// #define CEIL_GLIPH(x) ((x) >= 0.5f ? 1 : 0)
#define ABS_GLIPH(x) ((x) < 0 ? (-x) : (x))
#define D2TOD1_GLIPH(x, y) (((x)*(pixels.w+1)) + (y))
#define MOVETO_GLIPH(y, x) printf("\033[%d;%dH", (y), (x))
#define MOVETO0_0_GLIPH  \
    static const char tab[] = "\033[0;0H"; \
    if (write(stdout->_fileno, tab, 7) < 0) return -1
#define ARRAYCONVERTD1TOD2_GLIPH(T, W) ((char (*)[W+1])T)

// This pixels2d simplify the convertion only for this file
#ifdef pixels2d
# undef pixels2d
#endif
# define pixels2d ARRAYCONVERTD1TOD2_GLIPH(pixels._1d, pixels.w)

#define TESTALLOC(x)                                                           \
do                                                                             \
{                                                                              \
    if (x == NULL) {                                                           \
        fprintf(stderr, "ERROR: %s, ligne : %d\n", strerror(errno), __LINE__); \
        exit(EXIT_FAILURE);                                                    \
    }                                                                          \
} while(0)

/* DEPRECATED
# define GLIPH_ALLOC2D(type, name, W, H)                      \
    type **name = (type **)malloc(sizeof(type *) * ((H)+1));  \
    TESTMALLOC(name);                                         \
    do {                                                      \
        for (ssize_t i = 0; i < ((H)); i++) {                 \
            name[i] = (type *)malloc(sizeof(type)*((W) + 1)); \
            TESTMALLOC(name[i]);                              \
        }                                                     \
        name[(H)] = NULL;                                     \
    } while (0)
#define GLIPH_FREE2D(name, H)                                                        \
    if (name == NULL) fprintf(stdout, "WARNING: %s is NULL, %d\n", #name, __LINE__); \
    do {                                                                             \
        for (ssize_t i = 0; i < (H); i++) free(name[i]);                             \
        free(name);                                                                  \
    } while (0)
*/

typedef struct
{
    int x, y;
}COORD;

typedef struct
{
    float x, y;
}COORDF;

typedef struct
{
    char *_1d;
    const int w;
    const int h;
} Screen;

#define GLIPH_ALLOC(name, W, H)                                    \
    char *__screen1d = (char *)malloc(sizeof(char) * ((W+1)*H+1)); \
    TESTALLOC(__screen1d);                                         \
    Screen name = {._1d=__screen1d, .w=W, .h=H};                   \
    do {                                                           \
        for (int i = 0; i < H; i++) {                              \
            ((char (*)[W+1])name._1d)[i][W] = '\n';                \
        }                                                          \
    } while (0)
#define GLIPH_FREE(name) free(name._1d)

#define GLIPH_INIT(name, W, H)                        \
    char __screen1d[H*(W+1)+1];                       \
    Screen name = {._1d=__screen1d, .w=W, .h=H};      \
    do {                                              \
        for (int i = 0; i < H; i++) {                 \
            ((char (*)[W+1])name._1d)[i][W] = '\n';   \
        }                                             \
    } while (0)


void ConsoleClear(Screen pixels, const char clear);

void PrintRectangle(Screen pixels, int x, int y, int largeur, int hauteur, const char fd);
void PrintCircle(Screen pixels, int x, int y, int radius, const char fd);
void PrintLine(Screen pixels, int ax, int ay, int bx, int by, const char fd);
void PrintTriangle(Screen pixels, int ax, int ay, int bx, int by, int cx, int cy, const char fd);

int PrintConsole(Screen pixels);

void ClearDrawing(uint8_t ***pixels, short width, short height, const uint32_t fd);
void DrawLine(uint8_t ***pixels, short width, short height, int ax, int ay, int bx, int by, const uint32_t fd);
void DrawCircle(uint8_t ***pixels, short width, short height, int x, int y, int radius, const uint32_t fd);


# ifdef GLIPH_IMPLEMENTATION

#  if defined(GLIPH_SPACEPADDED)
// To compensate the width/height ratio this print is padded with a ' ' char
int PrintConsole(Screen pixels)
{
    MOVETO_GLIPH(0, 0);
    int i, j;
    for (i = 0; i < pixels.h; ++i) {
	for (j = 0; j < pixels.w; ++j) {
	    putchar(pixels2d[i][j]);
	    putchar(' ');
	}
	putchar('\n');
    }
    return 0;
}

# elif defined(GLIPH_CHARPADDED)
// To compensate the width/height ratio this print double the same char printed
int PrintConsole(Screen pixels)
{
    MOVETO_GLIPH(0, 0);
    int i, j;
    for (i = 0; i < pixels.h; ++i) {
	for (j = 0; j < pixels.w; ++j) {
	    putchar(pixels2d[i][j]);
	    putchar(pixels2d[i][j]);
	}
	putchar('\n');
    }
    return 0;
}

#  else
// This is raw printing, no compensation.
int PrintConsole(Screen pixels)
{
    MOVETO0_0_GLIPH;
    return write(stdout->_fileno, pixels._1d, (pixels.w+1)*pixels.h);
}
#  endif

void ConsoleClear(Screen pixels, const char clear)
{
    short i, j;
    for (i = 0; i < pixels.h; ++i) {
	for (j = 0; j < pixels.w; j++) {
            pixels2d[i][j] = clear;
	}
    }
}

void PrintRectangle(Screen pixels, int x, int y, int width, int height, const char fd)
{
    int i, j;
#ifdef GLIPH_PADDING
    // Problem with the bounderies
    x -= pixels.w/4;
    x *=2;
    width*=2;
#endif
    for (i = 0; i < pixels.h; i++) {
	for (j = 0; j < pixels.w; j++) {
            if (((j >= x) && (j < x+width)) && ((i >= y) && (i < y+height)))
		pixels2d[i][j] = fd;
        }
    }
}

void PrintLine(Screen pixels, int ax, int ay, int bx, int by, const char fd)
{
    COORDF a, b, AB;
    const float midH = (float)pixels.h/2.f;
    const float midW = (float)pixels.w/2.f;
    a.x = (float)ax/(midW);
    a.y = (float)ay/(-midH);
    b.x = (float)bx/(midW);
    b.y = (float)by/(-midH);
    AB.x = b.x - a.x;
    AB.y = b.y - a.y;
/* TODO: Modify t step to verify this condition: (step >= 1/sqrt((by-ay)² + (bx-ax)²)) */
       // double dt = 1.f/sqrt((bx-ax)*(bx-ax) + (by-ay)*(by-ay));
       // // double dt = 1.f/((ABS_GLIPH(bx-ax) + ABS_GLIPH(by-ay))*0.7);
       // for (double t = 0; t < 1; t+=dt) {

    double x, y;
    int Yaxe, Xaxe;
    for (double t = 0; t < 1; t+=0.01) {
#  ifdef GLIPH_PADDING
        x = (AB.x*t + a.x)*2;
#  else
        x = (AB.x*t + a.x);
#  endif
        y = (AB.y*t + a.y);
        if ((ABS_GLIPH(x*midW)) > midW-1 || (ABS_GLIPH(y*midH)) > midH-1) break;
	Yaxe = (midH)*(1 - y);
	Xaxe = (midW)*(1 + x);
	pixels2d[Yaxe][Xaxe] = fd;
    }
}

void PrintCircle(Screen pixels, int x, int y, int radius, const char fd)
{
    int i, j;
    for (i = 0; i < pixels.h; i++) {
	for (j = 0; j < pixels.w; j++) {
#  ifdef GLIPH_PADDING
	    if ((i-y)*(i-y) + (j-x*2+pixels.w/2)*(j-x*2+pixels.w/2)/4 <= radius*radius)
#  else
	    if ((i-y)*(i-y) + (j-x)*(j-x) <= radius*radius)
#  endif
		pixels2d[i][j] = fd;
	}
    }
}

// #include <math.h>
// void PrintDisk(char *pixels, short width, short height, int x, int y, int radius, const char fd)
// {
//     /* O(n) avec n = 8*radius */
//     for (double t = 0; t <= 2.f*M_PI; t+=M_PI/(double)(4*radius)) {
// 	int XX = (int)((double)radius*cos(t)) + x;
// 	int YY = (int)((double)radius*sin(t)) + y;

// 	if (XX < 0 || XX > width-1 || YY < 0 || YY > height-1) continue;
// 	pixels[YY][XX] = fd;
//     }
// }


void PrintTriangle(Screen pixels, int ax, int ay, int bx, int by, int cx, int cy, const char fd)
{
    COORDF a, b, c;
    a.x = ax;
    a.y = ay;
    b.x = bx;
    b.y = by;
    c.x = cx;
    c.y = cy;
#  ifdef GLIPH_PADDING
    a.x -= pixels.w/4;
    a.x *= 2;
    b.x -= pixels.w/4;
    b.x *= 2;
    c.x -= pixels.w/4;
    c.x *= 2;
#  endif

    float gA, gB, gC;

    short x, y;
    for (y = 0; y < pixels.h; y++) {
	for (x = 0; x < pixels.w; x++) {
	    gA = ((float)x*(b.y - c.y) + b.x*(c.y - (float)y) + c.x*((float)y - b.y))/(a.x*(b.y - c.y) + b.x*(c.y - a.y) + c.x*(a.y - b.y));
	    gB = (a.x*((float)y - c.y) + (float)x*(c.y - a.y) + c.x*(a.y - (float)y))/(a.x*(b.y - c.y) + b.x*(c.y - a.y) + c.x*(a.y - b.y));
	    gC = (a.x*(b.y - (float)y) + b.x*((float)y - a.y) + (float)x*(a.y - b.y))/(a.x*(b.y - c.y) + b.x*(c.y - a.y) + c.x*(a.y - b.y));

            if(gA >= 0 && gB >= 0 && gC >= 0)
                pixels2d[y][x] = fd;
	}
    }
}

#ifdef TODO
void RotateConsole(Screen pixels, short height, float tetha)
{
   RECT wind;
}
#endif

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
    COORDF a, b, AB;
    const int midH = height/2;
    const int midW = width/2;
    a.x = (float)ax/(float)(midW);
    a.y = (float)ay/(float)(-midH);
    b.x = (float)bx/(float)(midW);
    b.y = (float)by/(float)(-midH);
    AB.x = b.x - a.x;
    AB.y = b.y - a.y;
    // TODO: Modify t step with 3 if statement if (sqrt(h² + w²) > 1000) t+=0.0001 else reduce

    for (double t = 0; t < 1; t+=0.01) {
	double x = (AB.x*t + a.x);
	double y = (AB.y*t + a.y);
        if (ABS_GLIPH(x*(midW)) > midW-1 || ABS_GLIPH(y*(midH)) > midH-1) break;
        pixels[(int)(midH*(1 - y))][(int)(midW*(1 + x))][0] = fd>>(24); // 8*3
        pixels[(int)(midH*(1 - y))][(int)(midW*(1 + x))][1] = fd>>(16); // 8*2
        pixels[(int)(midH*(1 - y))][(int)(midW*(1 + x))][2] = fd>>(8);// 8*1
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

#  ifdef pixels2d
#   undef pixels2d
#  endif

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
- Line: 230:
    Change PrintDisk name by PrintCercle and vice vera
 **********************************/
