#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include "TOP_render.h"

#define SCREEN_WIDTH 320*2
#define SCREEN_HEIGHT 240*2
#define STRIDE 720
#define BPP 3
#define CHAR_H 8
#define CHAR_W 10
#define FB_SIZE 288000

// void drawRect(uint8_t *buffer, int stride, int bpp, //this is kinda useless, but i keep it as backup/fallback //edit 26/01/26, this is now used for bottom screen proper clearing
//                int x, int y, int w, int h,
//                uint8_t r, uint8_t g, uint8_t b,
//                int width, int height, int fb) {
//     for(int j=0;j<h;j++){
//         int py = y+j;
//         if(py>=height) break;
//         for(int i=0;i<w;i++){
//             int px = x+i;
//             if(px>=width) break;
//             int offset = py*stride + px*bpp;
//             buffer[offset+0] = b;
//             buffer[offset+1] = g;
//             buffer[offset+2] = r;
//         }
//     }

// }

void drawSprite(uint8_t *buffer, int x0, int y0, int img_width, int img_height, const uint8_t img[img_height][img_width][3])
{
    int start_y = y0 < 0 ? -y0 : 0;
    int end_y = (y0 + img_height > SCREEN_HEIGHT) ? SCREEN_HEIGHT - y0 : img_height;
    if (x0 >= SCREEN_WIDTH){return;}

    int max_width = SCREEN_WIDTH - x0;
    int copy_width = (img_width < max_width) ? img_width : max_width;
    for (int y = start_y; y < end_y; y++) {
        const uint8_t *src = (const uint8_t *)img[y];
        uint8_t *dst = buffer + (y0 + y) * STRIDE + x0 * BPP;

        memcpy(dst, src, copy_width * BPP);
    }
}

void drawSpriteAlpha(uint8_t *buffer, int x0, int y0, int img_width, int img_height, const uint8_t img[img_height][img_width][3])
{
    for (int y = 0; y < img_height; y++) {
        int py = y0 + y;
        if (py >= SCREEN_HEIGHT) continue;

        for (int x = 0; x < img_width; x++) {
            int px = x0 + x;
            if (px >= SCREEN_WIDTH) continue;

            const uint8_t *pixel = img[y][x];

            if (pixel[0] == 255 && pixel[1] == 0 && pixel[2] == 255) {
                continue; //if color is 0xFF00FF, display transparent
            }

            int offset = py * STRIDE + px * BPP;
            buffer[offset + 0] = pixel[0]; // B
            buffer[offset + 1] = pixel[1]; // G
            buffer[offset + 2] = pixel[2]; // R
        }
    }
}

//TODO: fix this when i will implement the gb emulator
// void drawGbFrame(uint8_t *backbuffer, int offset_x, int offset_y) {

//     for (int y = 0; y < LCD_HEIGHT; y++) {
//         for (int x = 0; x < LCD_WIDTH; x++) {
//             uint8_t shade = gb_fb[LCD_HEIGHT - 1 - y][x] & 3;

//             int dst_x = y + offset_x;       
//             int dst_y = x + offset_y;

//             if (dst_x < 0 || dst_x >= SCREEN_W || dst_y < 0 || dst_y >= SCREEN_H)
//                 continue;

//             uint8_t *dst = backbuffer + dst_y * STRIDE + dst_x * BYTES_PER_PIXEL;
//             dst[0] = color_map[shade][2]; // B
//             dst[1] = color_map[shade][1]; // G
//             dst[2] = color_map[shade][0]; // R
//         }
//     }
// }

const uint8_t (*getCharSprite(char c))[CHAR_W] {
    switch (c) {
        // Lookup table (kinda) to get every char
        // ADDING NON-ASCII CHARS WILL BREAK THE CODE!
        case 'A': return a;
        case 'B': return b;
        case 'C': return c_char; // not just "c" bc it will mess up with libc types
        case 'D': return d;
        case 'E': return e;
        case 'F': return f;
        case 'G': return g;
        case 'H': return h;
        case 'I': return i;
        case 'J': return j;
        case 'K': return k;
        case 'L': return l;
        case 'M': return m;
        case 'N': return n;
        case 'O': return o;
        case 'P': return p;
        case 'Q': return q;
        case 'R': return r;
        case 'S': return s;
        case 'T': return t;
        case 'U': return u;
        case 'V': return v;
        case 'W': return w;
        case 'X': return x;
        case 'Y': return y;
        case 'Z': return z;
        //numbers
        case '0': return num0;
        case '1': return num1;
        case '2': return num2;
        case '3': return num3;
        case '4': return num4;
        case '5': return num5;
        case '6': return num6;
        case '7': return num7;
        case '8': return num8;
        case '9': return num9;
        // Special chars
        case '!': return exclamation;
        case '?': return question;
        case ' ': return space;
        case '*': return star;
        case '/': return slash;
        case '_': return underscore;
        case '=': return equal;
        case ':': return colon;
        case '&': return and;
        case '\\': return backslash;
        case '#': return hashtag;
        case '^': return hat;
        case '-': return minus;
        case '(': return parh1;
        case ')': return parh2;
        case '%': return percent;
        case '|': return pipe_;
        case '+': return plus;
        case '"': return quote;
        case '.': return dot;
        case ';': return semicolon;
        case ',': return comma;
        case '~': return tilde;
        // chars ←, →, ↑ and ↓ can be used, but only as {, [, ] and }
        case '{': return left;
        case '[': return right;
        case ']': return up;
        case '}': return down;
        // battery icons
        // case 'e': return bat_empty; //empty
        // case 'l': return bat_low; //low
        // case 'm': return bat_med; //medium
        // case 'h': return bat_high; //high
        // case 'f': return bat_full; //full

        default: return NULL; // if unknown char
    }
}

void drawText(uint8_t *buffer,
              int x0, int y0,
              const char *text,
              uint8_t fb, uint8_t fg, uint8_t fr,
              uint8_t bb, uint8_t bg, uint8_t br)
{
    int txt_cursor_x = x0;
    int txt_cursor_y = y0;

    // //Pre-pack RGB888 colors
    // const uint32_t fg_rgb = fr | (fg << 8) | (fb << 16);
    // const uint32_t bg_rgb = br | (bg << 8) | (bb << 16);

    for (int i = 0; text[i] != '\0'; i++) {
        char c = text[i];

        if (c == '\n') {
            txt_cursor_x -= CHAR_W;
            txt_cursor_y = y0;
            continue;
        }

        const uint8_t (*sprite)[CHAR_W] = getCharSprite(c);
        if (!sprite) continue;

        uint8_t *dst_line =
            buffer + (txt_cursor_y * STRIDE + txt_cursor_x * BPP);

        for (int y = 0; y < CHAR_H; y++) {
            uint8_t *dst = dst_line;
            const uint8_t *src_row = sprite[y];

            for (int x = 0; x < CHAR_W; x++) { //replaced 32-bits writes to 24-bit here
                if (src_row[x]) {
                    dst[0] = fr;
                    dst[1] = fg;
                    dst[2] = fb;
                } else {
                    dst[0] = br;
                    dst[1] = bg;
                    dst[2] = bb;
                }
                dst += 3;
            }

            dst_line += STRIDE;
        }

        txt_cursor_y += CHAR_H;
    }
}

//OLD drawText before 1-bit conversion and optimisations of the text engine
// void drawText(uint8_t *buffer, int x0, int y0, const char *text, uint8_t fb, uint8_t fg, uint8_t fr, uint8_t bb, uint8_t bg, uint8_t br)
// {
//     int txt_cursor_x = x0;
//     int txt_cursor_y = y0;

//     for (int i = 0; text[i] != '\0'; i++) {
//         char c = text[i];

//         if (c == '\n') {
//             txt_cursor_x -= CHAR_W;
//             txt_cursor_y = y0;
//             continue;
//         }

//         const uint8_t (*sprite)[CHAR_W][3] = getCharSprite(c);
//         if (!sprite) continue;

//         // calculate char start in framebuffer
//         uint8_t *dst_line = buffer + (txt_cursor_y * STRIDE + txt_cursor_x * BPP);

//         for (int y = 0; y < CHAR_H; y++) {
//             uint8_t *dst = dst_line;
//             const uint8_t (*src_row)[3] = sprite[y];

//             for (int x = 0; x < CHAR_W; x++) {
//                 const uint8_t *pix = src_row[x];

//                 // test intensity instead of 3 color checks
//                 uint8_t intensity = pix[0] | pix[1] | pix[2];
//                 if (intensity) {
//                     dst[0] = fr; dst[1] = fg; dst[2] = fb;
//                 } else {
//                     dst[0] = br; dst[1] = bg; dst[2] = bb;
//                 }

//                 dst += BPP;
//             }

//             dst_line += STRIDE;
//         }

//         txt_cursor_y += CHAR_H;
//     }
// }

void TOP_flushToFramebuffer(uint8_t *fb, uint8_t *buffer) {
    memcpy(fb, buffer, FB_SIZE);
}

// //Textures stored as arrays in *include/textures.txt*
// #include "textures.txt"

// //Font stored as arrays in *include/font.txt*
// #include "font.txt"
