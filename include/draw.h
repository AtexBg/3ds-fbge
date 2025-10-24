#ifndef DRAW_H
#define DRAW_H
#include <stdint.h>

#define CURSOR_W 12
#define CURSOR_H 10
#define CHAR_W 10
#define CHAR_H 8
#define BG_W 240
#define BG_H 400
#define WIN_H 100
#define WIN_W 110
#define SETT_H 260
#define SETT_W 140
#define TERM_H 166
#define TERM_W 121

//TEXTURES
extern uint8_t cursor[CURSOR_H][CURSOR_W][3];
extern uint8_t cursor1[CURSOR_H][CURSOR_W][3];
extern uint8_t background0[BG_H][BG_W][3];
extern uint8_t window[WIN_H][WIN_W][3];
extern uint8_t settingsBg[SETT_H][SETT_W][3];
extern uint8_t termWin[TERM_H][TERM_W][3];

//CHARS
extern uint8_t a[CHAR_H][CHAR_W][3];
extern uint8_t b[CHAR_H][CHAR_W][3];
extern uint8_t c_char[CHAR_H][CHAR_W][3];
extern uint8_t d[CHAR_H][CHAR_W][3];
extern uint8_t e[CHAR_H][CHAR_W][3];
extern uint8_t f[CHAR_H][CHAR_W][3];
extern uint8_t g[CHAR_H][CHAR_W][3];
extern uint8_t h[CHAR_H][CHAR_W][3];
extern uint8_t i[CHAR_H][CHAR_W][3];
extern uint8_t j[CHAR_H][CHAR_W][3];
extern uint8_t k[CHAR_H][CHAR_W][3];
extern uint8_t l[CHAR_H][CHAR_W][3];
extern uint8_t m[CHAR_H][CHAR_W][3];
extern uint8_t n[CHAR_H][CHAR_W][3];
extern uint8_t o[CHAR_H][CHAR_W][3];
extern uint8_t p[CHAR_H][CHAR_W][3];
extern uint8_t q[CHAR_H][CHAR_W][3];
extern uint8_t r[CHAR_H][CHAR_W][3];
extern uint8_t s[CHAR_H][CHAR_W][3];
extern uint8_t t[CHAR_H][CHAR_W][3];
extern uint8_t u[CHAR_H][CHAR_W][3];
extern uint8_t v[CHAR_H][CHAR_W][3];
extern uint8_t w[CHAR_H][CHAR_W][3];
extern uint8_t x[CHAR_H][CHAR_W][3];
extern uint8_t y[CHAR_H][CHAR_W][3];
extern uint8_t z[CHAR_H][CHAR_W][3];

//NUMBERS
extern uint8_t num0[CHAR_H][CHAR_W][3];
extern uint8_t num1[CHAR_H][CHAR_W][3];
extern uint8_t num2[CHAR_H][CHAR_W][3];
extern uint8_t num3[CHAR_H][CHAR_W][3];
extern uint8_t num4[CHAR_H][CHAR_W][3];
extern uint8_t num5[CHAR_H][CHAR_W][3];
extern uint8_t num6[CHAR_H][CHAR_W][3];
extern uint8_t num7[CHAR_H][CHAR_W][3];
extern uint8_t num8[CHAR_H][CHAR_W][3];
extern uint8_t num9[CHAR_H][CHAR_W][3];

//SPECIAL CHARS
extern uint8_t exclamation[CHAR_H][CHAR_W][3];
extern uint8_t question[CHAR_H][CHAR_W][3];
extern uint8_t space[CHAR_H][CHAR_W][3];
extern uint8_t star[CHAR_H][CHAR_W][3];
extern uint8_t slash[CHAR_H][CHAR_W][3];
extern uint8_t underscore[CHAR_H][CHAR_W][3];
extern uint8_t equal[CHAR_H][CHAR_W][3];
extern uint8_t colon[CHAR_H][CHAR_W][3];
extern uint8_t and[CHAR_H][CHAR_W][3];
extern uint8_t backslash[CHAR_H][CHAR_W][3];
extern uint8_t hashtag[CHAR_H][CHAR_W][3];
extern uint8_t hat[CHAR_H][CHAR_W][3];
extern uint8_t minus[CHAR_H][CHAR_W][3];
extern uint8_t parh1[CHAR_H][CHAR_W][3];
extern uint8_t parh2[CHAR_H][CHAR_W][3];
extern uint8_t percent[CHAR_H][CHAR_W][3];
extern uint8_t pipe_[CHAR_H][CHAR_W][3];
extern uint8_t plus[CHAR_H][CHAR_W][3];
extern uint8_t quote[CHAR_H][CHAR_W][3];
extern uint8_t dot[CHAR_H][CHAR_W][3];
extern uint8_t semicolon[CHAR_H][CHAR_W][3];
extern uint8_t comma[CHAR_H][CHAR_W][3];
extern uint8_t left[CHAR_H][CHAR_W][3];
extern uint8_t right[CHAR_H][CHAR_W][3];
extern uint8_t up[CHAR_H][CHAR_W][3];
extern uint8_t down[CHAR_H][CHAR_W][3];
extern uint8_t tilde[CHAR_H][CHAR_W][3];

//SYMBOLS
extern uint8_t bat_empty[CHAR_H][CHAR_W][3];
extern uint8_t bat_low[CHAR_H][CHAR_W][3];
extern uint8_t bat_med[CHAR_H][CHAR_W][3];
extern uint8_t bat_high[CHAR_H][CHAR_W][3];
extern uint8_t bat_full[CHAR_H][CHAR_W][3];

void drawRect(uint8_t *buffer, int stride, int bpp,
               int x, int y, int w, int h,
               uint8_t r, uint8_t g, uint8_t b,
               int screen_width, int screen_height, int fb);

void drawSprite(uint8_t *buffer, int stride, int bpp,
                   int x0, int y0,
                   int screen_width, int screen_height,
                   int img_width, int img_height,
                   const uint8_t img[img_height][img_width][3]);

void drawSpriteAlpha(uint8_t *buffer, int stride, int bpp,
                   int x0, int y0,
                   int screen_width, int screen_height,
                   int img_width, int img_height,
                   const uint8_t img[img_height][img_width][3]);

void drawText(uint8_t *buffer, int stride, int bpp,
              int x0, int y0,
              int screen_width, int screen_height,
              const char *text, int newlineSkip,
              uint8_t fr, uint8_t fg, uint8_t fb,
              uint8_t br, uint8_t bg, uint8_t bb);

void drawCursor(); //what do u think this does, drawing a fucking cursor???

void writeToBuffer(uint8_t *buffer, int bpp, int width, int height, int fb);

#endif
