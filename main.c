#include <linux/fb.h>
#include <sys/ioctl.h>
#include <linux/input.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

#include "headers/graphics.h"
#include "headers/coordinates.h"

// -----------------------------------------
// *main.c* file for the "3ds-fbge" project
// 07/09/2025: Codebase created 
// Hours spent: 31
// Made to run on Linux3DS, unusable on PC
// -----------------------------------------
// TODO:
// -add screenshots function (and make a converter on PC)
// -add item selection and detection 
// -fill imported headers
// -Add more graphical content
// -And many more...
// -----------------------------------------

int point_between(int x, int y, int x0, int y0, int img_width, int img_height){
    if(x >= x0 && x <= (x0 + img_width) &&
       y >= y0 && y <= (y0 + img_height)) {
        return 1;
    } else {
        return 0;
    }
}

void drawSprite(uint8_t *buffer, int stride, int bpp,
                   int x0, int y0,
                   int screen_width, int screen_height,
                   int img_width, int img_height,
                   const uint8_t img[img_height][img_width][3])
{
    for (int y = 0; y < img_height; y++) {
        int py = y0 + y;
        if (py >= screen_height) continue;

        for (int x = 0; x < img_width; x++) {
            int px = x0 + x;
            if (px >= screen_width) continue;

            const uint8_t *pixel = img[y][x];

            if (pixel[0] == 255 && pixel[1] == 0 && pixel[2] == 255) {
                continue; //if color is 0xFF00FF, display transparent
            }

            int offset = py * stride + px * bpp;
            buffer[offset + 0] = pixel[0]; // B
            buffer[offset + 1] = pixel[1]; // G
            buffer[offset + 2] = pixel[2]; // R
        }
    }
}

// function to draw a simple rectangle, almost unused except for testing
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
    // initialzing some variables
    int def_x0_win = 50;
    int def_y0_win = 50;
    int running = 1;
    bool isKeyPressed_L = false;
    bool isKeyPressed_R = false;
    for (int it = 1; it <= 31; it++) {
        printf("\n"); //prints 31 newline chars to clear the tty and avoid artefacts
        printf("\033[?251]"); //xippo's fix for terminal cursor refreshing
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

    // read data from buttons (event0)
    const char *device_path = "/dev/input/event0";
    int fd = open(device_path, O_RDONLY | O_NONBLOCK); //O_NONBLOCK so it doesn't freeze the whole script
    struct input_event ev;

    // read data from circle pad device
    int mouse = open("/dev/input/mouse0", O_RDONLY | O_NONBLOCK);
    if(mouse < 0){ perror("mouse"); free(buffer); close(fb); return 1; }

    int cursor_x = width/2;
    int cursor_y = height/2;
    int last_cursor_x = 0;
    int last_cursor_y = 0;
    signed char data[3];

    while(running){
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
        
        ssize_t n2 = read(fd, &ev, sizeof(ev)); //idk but smth for event0

        draw_rect(buffer,stride,bpp,0,0,240,400,58,111,166,width,height); // Draws flat color background before image, as fallback if file isn't found

        //--------------------------------------
        //------- DRAWING FUNCTIONS HERE -------
        //--------------------------------------
        memcpy(buffer, background, BG_W * BG_H * 3); //write bg

        if(n2 == sizeof(ev)) {
        if(ev.type == EV_KEY) {
            isKeyPressed_L = (ev.code == 310 && ev.value == 1);
            }
        if(ev.type == EV_KEY) {
            isKeyPressed_R = (ev.code == 311 && ev.value == 1);
            }
        }
        
        drawSprite(buffer, stride, bpp, 0, 0, width, height, TASKBAR_W, TASKBAR_H, taskbar); //draw taskbar
        
        if(isKeyPressed_L && point_between(cursor_x, cursor_y, def_x0_win, def_y0_win, WIN1_H, WIN1_W) == 1){

            def_x0_win = def_x0_win + (cursor_x - last_cursor_x);
            def_y0_win = def_y0_win + (cursor_y - last_cursor_y);

            //per evitare crash se va fuori dallo schermo (aka to avoid off-screen crashes)
            if(def_x0_win < 0) def_x0_win = 0;
            if(def_y0_win < 0) def_y0_win = 0;
            if(def_x0_win > width - 110) def_x0_win = width - 110;
            if(def_y0_win > height - 100) def_y0_win = height - 100;

            drawSprite(buffer, stride, bpp, def_x0_win, def_y0_win, width, height, WIN1_W, WIN1_H, window1); //If L is clicked, display cursor1 and drag window
            drawSprite(buffer, stride, bpp, cursor_x, cursor_y, width, height, CURSOR_W, CURSOR_H, cursor1);
        } else {
            drawSprite(buffer, stride, bpp, def_x0_win, def_y0_win, width, height, WIN1_W, WIN1_H, window1);
            drawSprite(buffer, stride, bpp, cursor_x, cursor_y, width, height, CURSOR_W, CURSOR_H, cursor);
        }

        //--------------------------------------
        //------- ENDS DRAWING FUNCTIONS -------
        //--------------------------------------
        // draw_rect : Draws a rectangle at defined coordinates
        // drawSprite : Draws an image asset from *graphics.h*

        last_cursor_x = cursor_x;
        last_cursor_y = cursor_y;

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
