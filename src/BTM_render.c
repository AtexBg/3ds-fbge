#include <stdio.h>
#include <sys/mman.h> //mmap
#include <string.h> //memcpy
#include <stdint.h> //uint8_t
#include <unistd.h> //open
#include <fcntl.h> //flags for open();
#include <sys/stat.h> //mkdir
#include <sys/types.h> //somehow also mkdir

#include "charset_data.c"

#define BASE_PAGE_ADDRESS 0x18119000
#define PAGE_SIZE 0x1000
#define FB_SIZE 0x38400 //size of visible bottom framebuffer
#define PADD_OFFSET 0x0400 //offset so it aligns in the page (it may just be 0x0C00 instead, idk)

//all the memory that will be mapped, added 0x800 to stay aligned in the pages
#define MAPPED_MEMORY_SIZE (FB_SIZE+PADD_OFFSET+(PAGE_SIZE*2)+0x800)

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

#define HEIGHT 240
#define WIDTH 320
#define BPP 3
#define STRIDE (HEIGHT*BPP)

uint8_t *rawFB; //declare it outside of any function so it can be used anywhere in this file

const uint8_t (*BTM_getCharSprite(char c))[10] {
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

uint8_t* initBottomFramebuffer() {
    int rawMemory = open("/dev/mem", O_RDWR | O_SYNC);
    if(rawMemory < 0){ perror("open /dev/mem"); return NULL; }

    //mmap should start at an address aligned with the size of a page
    off_t aligned_base = BASE_PAGE_ADDRESS & ~(PAGE_SIZE-1);
    off_t offset_in_page = BASE_PAGE_ADDRESS - aligned_base + PADD_OFFSET;

    rawFB = mmap(NULL, FB_SIZE + offset_in_page, PROT_READ | PROT_WRITE, MAP_SHARED, rawMemory, aligned_base);
    if(rawFB==MAP_FAILED){perror("mmap"); return NULL;}

    //in fact, this returns a uint8_t pointer to a 0x38400 bytes buffer starting at 0x18119400 in RAM
    return rawFB + offset_in_page;
}

//broken, only for some debugging
void BTM_drawPixel(uint8_t* BTM_backbuffer, int x, int y, uint8_t r, uint8_t g, uint8_t b) {
    int idx = (y * 320 + x) * BPP;
    BTM_backbuffer[idx + 0] = r;
    BTM_backbuffer[idx + 1] = g;
    BTM_backbuffer[idx + 2] = b;
}

void BTM_drawRect(uint8_t *buffer, int y, int x, int h, int w,uint8_t r, uint8_t g, uint8_t b) { //y and x are inverted, it somehow works so i don't touch it
    for(int j=0;j<h;j++){
        int py = y+j;
        if(py>=HEIGHT) break;
        for(int i=0;i<w;i++){
            int px = x+i;
        if(px>=WIDTH) break;
            int offset = py*STRIDE + px*BPP;
            buffer[offset+0] = b;
            buffer[offset+1] = g;
            buffer[offset+2] = r;
        }
    }
}

void BTM_drawSpriteAlpha(uint8_t *buffer, int x0, int y0, int img_width, int img_height, const uint8_t img[img_height][img_width][3])
{
    for (int y = 0; y < img_height; y++) {
        int py = y0 + y;
        if (py >= 400) continue;

        for (int x = 0; x < img_width; x++) {
            int px = x0 + x;
            if (px >= 240) continue; //if it somehow works, don't touch it. (AtexBg, 25/01/26)

            const uint8_t *pixel = img[y][x];

            if (pixel[0] == 255 && pixel[1] == 0 && pixel[2] == 255) {
                continue; //if color is 0xFF00FF, don't display it
            }

            int offset = py * STRIDE + px *BPP;
            buffer[offset + 0] = pixel[0]; // B
            buffer[offset + 1] = pixel[1]; // G
            buffer[offset + 2] = pixel[2]; // R
        }
    }
}

//BROKEN
void BTM_clearScreen(uint8_t* BTM_backbuffer){ //doesn't fill the touch keyboard area
    BTM_drawRect(BTM_backbuffer, 0,0,     320,144, 0xFF,0xFF,0xFF);
    BTM_drawRect(BTM_backbuffer, 144,240, 80,48,   0xFF,0xFF,0xFF);
}

void BTM_clearScreenNoKeyboard(uint8_t* BTM_backbuffer){
    memset(BTM_backbuffer, 0xFF, FB_SIZE); //fills screen in white
}

void BTM_flushToFramebuffer(uint8_t* framebuffer, uint8_t *backbuffer){
    memcpy(framebuffer, backbuffer, FB_SIZE); //copy backbuffer into real framebuffer (aka refresh the screen)
}

void munmapBottomFramebuffer(){
    munmap(rawFB, FB_SIZE + PADD_OFFSET); //simpely unmap the rawFB (the one without offsets)
}
