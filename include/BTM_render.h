#ifndef BTM_RENDER_H
#define BTM_RENDER_H
#include <stdint.h>

#include "textures.txt"

// #if USE_OLD_FONT
//     #include "old_font.txt"
// #else
    #include "font.txt"
// #endif

uint8_t* initBottomFramebuffer(); //initialize the bottom FB

void BTM_clearScreen(uint8_t* BTM_backbuffer); //clears the screen in white but not the touch keyboard area
void BTM_clearScreenNoKeyboard(uint8_t* BTM_backbuffer); //clears the screen in white
void BTM_flushToFramebuffer(uint8_t* framebuffer, uint8_t* backbuffer); //refrshes the display
void BTM_drawPixel(uint8_t* BTM_backbuffer, int x, int y, uint8_t r, uint8_t g, uint8_t b); //draws a single pixel on screen (unused)
void BTM_drawSpriteAlpha(uint8_t *buffer, int x0, int y0, int img_width, int img_height, const uint8_t img[img_height][img_width][3]); //experimental implementation of drawSpriteAlpha
int  BTM_drawText(uint8_t *buffer, int x0, int y0, const char *text, uint8_t fb, uint8_t fg, uint8_t fr, uint8_t bb, uint8_t bg, uint8_t br);
void BTM_drawRect(uint8_t *buffer, int y, int x, int h, int w,uint8_t r, uint8_t g, uint8_t b);

void munmapBottomFramebuffer(); //unmap and close the framebuffer memory

#endif