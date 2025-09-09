#include <linux/fb.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

#include "headers/graphics.h"
#include "headers/coordinates.h"

// -----------------------------------------
// *main.c* file for the "3ds-fbge" project
// 07/09/2025: Codebase created 
// Hours spent: 17
// Made to run on Linux3DS, unusable on PC
// -----------------------------------------
// TODO:
// -Add wallpaper setting
// -add screenshots function (and make a converter on PC)
// -add item selection and detection 
// -fill imported headers
// -fix issue where the coordinates are rotated (mybe rotate the images instead?)
// -Add mouse click detection with L/R triggers
// -Add more graphical content
// -Properly define separate drawing layers
// -----------------------------------------

int loadWallpaper(uint8_t *buffer, int width, int height, int bpp) {
    const char *path = "/mnt/wallpaper.bin"; // hardcoded path, may change that later
    FILE *f = fopen(path, "rb");

    size_t fbSize = width * height * bpp;
    size_t read = fread(buffer, 1, fbSize, f);
    fclose(f);

    return 0;
}


void drawImg(uint8_t *buffer, int stride, int bpp,
                 int x0, int y0,
                 int screen_width, int screen_height) {
    for(int y=0; y<CURSOR_H; y++){
        int py = y0 + y;
        if(py >= screen_height) continue;
        for(int x=0; x<CURSOR_W; x++){
            int px = x0 + x;
            if(px >= screen_width) continue;
            int color = cursor[y][x];
            uint8_t r=0,g=0,b=0;
            // Implementation of a 4-Bit Color Palette to use with *graphics.h*
            if(color==0){ r=g=b=255; }                 // White
            else if(color==1){ r=g=b=0; }              // Black
            else if(color==2) continue;                // Do nothing, transparency
            else if(color==7){ r=g=0; b=255; }         // Blue
            else if(color==8){ r=b=0; g=255; }         // Green
            else if(color==3){ b=g=0; r=255; }         // Red
            else if(color==4){ r=126; g=253; b=0; }    // Fluo green
            else if(color==5){ r=127; g=b=0; }         // Brown
            else if(color==6){ r=127; g=0; b=125; }    // Magenta
            else if(color==9){ r=255; g=127; b=0; }    // Orange
            else if(color==10){ r=254; g=128; b=129; } // Pink
            else if(color==11){ r=127; g=128; b=0; }   // Olive
            else if(color==12){ r=g=255; b=0; }        // Yellow
            else if(color==13){ r=g=b=234; }           // Gray
            else if(color==14){ r=g=b=126; }           // Darker gray            
            else if(color==15){ r=g=b=80; }            // Silver
            int offset = py*stride + px*bpp;
            buffer[offset+0] = b;
            buffer[offset+1] = g;
            buffer[offset+2] = r;
        }
    }
}

//draw function
void draw_rect(uint8_t *buffer, int stride, int bpp,
               int x, int y, int w, int h,
               uint8_t r, uint8_t g, uint8_t b,
               int screen_width, int screen_height) {
    for(int j=0;j<h;j++){
        int py = y+j;
        if(py>=screen_height) break;
        for(int i=0;i<w;i++){
            int px = x+i;
            if(px>=screen_width) break;
            int offset = py*stride + px*bpp;
            buffer[offset+0] = b;
            buffer[offset+1] = g;
            buffer[offset+2] = r;
        }
    }
}

int main() {
    for (int it = 1; it <= 31; it++) {
        printf("\n"); //prints 31 newline chars to clear the tty and avoid artefacts
    }
    // prepare framebuffer for write operations
    int fb = open("/dev/fb0", O_RDWR);
    if(fb < 0){ perror("fb"); return 1; }

    struct fb_var_screeninfo vinfo;
    if(ioctl(fb, FBIOGET_VSCREENINFO, &vinfo)){ perror("ioctl"); close(fb); return 1; }

    int width = vinfo.xres;
    int height = vinfo.yres;
    int bpp = vinfo.bits_per_pixel / 8;
    int stride = width * bpp;

    uint8_t *buffer = malloc(width * height * bpp);
    if(!buffer){ perror("malloc"); close(fb); return 1; }

    // read data from circle pad device
    int mouse = open("/dev/input/mouse0", O_RDONLY);
    if(mouse < 0){ perror("mouse"); free(buffer); close(fb); return 1; }

    int cursor_x = width/2;
    int cursor_y = height/2;
    signed char data[3];

    // main loop inside loop();
    while(1){
        // read mouse input
        int n = read(mouse, data, 3);
        if(n == 3){
            signed char dx = data[2];
            signed char dy = data[1];
            cursor_x += dx;
            cursor_y += dy;

            if(cursor_x < 0) cursor_x = 0;
            if(cursor_y < 0) cursor_y = 0;
            if(cursor_x > width - CURSOR_W) cursor_x = width - CURSOR_W;
            if(cursor_y > height - CURSOR_H) cursor_y = height - CURSOR_H;

            fflush(stdout);
        }

        draw_rect(buffer,stride,bpp,0,0,240,400,58,111,166,width,height); // Draws flat color background before image, as fallback if file isn't found
        
        uint8_t *buffer = malloc(width * height * bpp);
        if (!buffer) { perror("malloc"); return 1; }

            // load wallpaper
        if (loadWallpaper(buffer, width, height, bpp) != 0) {
            // if fails, fills with black
        memset(buffer, 0, width * height * bpp);
        }

        //------- DRAWING FUNCTIONS HERE -------
        draw_rect(buffer,stride,bpp,160,50,11,100,48,73,104,width,height);
        draw_rect(buffer,stride,bpp,50,50,110,100,212,208,200,width,height);

        drawImg(buffer,stride,bpp,cursor_x,cursor_y,width,height);
        //------- ENDS DRAWING FUNCTIONS -------
        // draw_rect : Draws a rectangle at defined coordinates
        // drawImg : Draws an image asset from *graphics.h*

        // write in fb0
        lseek(fb, 0, SEEK_SET);
        write(fb, buffer, width*height*bpp);

        usleep(16600); // delay of 1 frame (16,6 ms) to reduce lag
    }

    free(buffer);
    close(mouse);
    close(fb);
    return 0;
}
