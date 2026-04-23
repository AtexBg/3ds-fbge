#ifndef TEXT_H
#define TEXT_H

#define CHAR_W 10
#define CHAR_H 8

void drawText(uint8_t *buffer, int stride, int bpp,
              int x0, int y0,
              int screen_width, int screen_height,
              const char *text,
              uint8_t fr, uint8_t fg, uint8_t fb,
              uint8_t br, uint8_t bg, uint8_t bb);

#endif
