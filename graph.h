#ifndef CONS_H_INCLUED
#define CONS_H_INCLUED

#define max(x, y) ((x)<(y) ? y : x)
#define min(x, y) ((x)>(y) ? y : x)
#define moveto(y, x) printf("\033[%d;%dH", (y), (x));
#define dec(x, y) ((x*(width+1)) + y)
#define ERRALLOC(x)                                                                          \
do                                                                                           \
{                                                                                            \
    if (x == NULL) {                                                                         \
        fprintf(stderr, "Erreur d'allocation : %s, ligne : %d\n", strerror(errno), __LINE__);\
        exit(EXIT_FAILURE);                                                                  \
    }                                                                                        \
} while(0)                                                                                   \


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
   COORD A;
   COORD B;
}RECT;

typedef struct
{
   COORDF A;
   COORDF B;
   COORDF C;
}TRIG;


char **mem_alloc(int H, int W);
void mem_free(char **ptr, int H);
void cons_clear(char **pixels, short width, short height, const char clear);
void cons_rect(char **pixels, short width, short height, int x, int y, int largeur, int hauteur, const char fd);
void cons_cercle(char **pixels, short width, short height, int x, int y, int radius, const char fd);
void cons_ligne(char **pixels, short width, short height, int ax, int ay, int bx, int by, int ep, const char fd);
void cons_triangle(char **pixels, short width, short height, int ax, int ay, int bx, int by, int cx, int cy, const char fd);

void print_cons(char **pixels, short width, short height);

void set_console();

#include <stdint.h>
void draw_clear(uint8_t ***pixels, short width, short height);
void draw_ligne(uint8_t ***pixels, short width, short height, int ax, int ay, int bx, int by, int ep, const uint32_t fd);
void draw_cercle(uint8_t ***pixels, short width, short height, int x, int y, int radius, const uint32_t fd);

#endif //CONS_H_INCLUED
