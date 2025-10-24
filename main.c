#include <stdio.h>
#include <unistd.h> //sleep
#include <stdlib.h>
#include <stdint.h>
#include <string.h> //memcpy
#include <fcntl.h> //open

//custom includes
#include "draw.h"
#include "battery.h"
#include "input.h"
#include "fps.h"
#include "times.h"
#include "hitbox.h"
#include "settings.h"
#include "backlight.h"
#include "screenshot.h"
#include "keyboard.h"

int RUNNING = 1;
int SETTINGS = 0;
int cursor_x = 120;
int cursor_y = 200;
int width = 240;
int height = 400;
int bpp = 3;
int stride = 720;
int win1_x = 50;
int win1_y = 50;
int cursor_delta_x = 0;
int cursor_delta_y = 0;
int lastCursorX = 0;
int lastCursorY = 0;
int cooldown = 0;
int brightness = 100;
int frameskip = 0;
int isTermOpened = 1;

int main() {
    system("stty -echo"); //silent terminal
    uint8_t *buffer = malloc(width * height * bpp); //Allocate buffer
    int fb = open("/dev/fb0", O_RDWR); //Open "/dev/fb0" as fb 
    initInput();  //---|
    initKeyboard(); //-- Some init functions

    while(RUNNING) {
        cooldown--;
        handleButtons();
        if(!SETTINGS){handleMouse();}
        if(!SETTINGS && isTermOpened){handleKeyboard();}
        memcpy(buffer, background0, 288000); //write background
        int fps = getCurrentFPS();
        char buf[16];
        Time t = getCurrentTime();

        //check for window to stay on screen to avoid segfault
        if(win1_x < 0) win1_x = 0;
        if(win1_y < 0) win1_y = 0;
        if(win1_x > width - 121) win1_x = width - 121;
        if(win1_y > height - 166) win1_y = height - 166;

        //init delta
        int delta_x = cursor_x - lastCursorX;
        int delta_y = cursor_y - lastCursorY;

        // WINDOW REPLACED WITH TERMINAL HERE
        // drawSprite(buffer, stride, bpp, win1_x, win1_y, width, height, WIN_W, WIN_H, window);
        // drawText(buffer, stride, bpp, win1_x + 98, win1_y + 3, width, height, "WINDOW", 0, 255,255,255, 99,130,181);
        if(isTermOpened){drawSprite(buffer, stride, bpp, win1_x, win1_y, width, height, TERM_W, TERM_H, termWin);}
        if(isTermOpened){drawText(buffer, stride, bpp, win1_x + 109, win1_y + 3, width, height, "TERMINAL", 0, 255,255,255, 1,119,13);}
        if(isTermOpened){drawText(buffer, stride, bpp, win1_x + 94, win1_y + 3, width, height, termBuffer, 0, 255,255,255, 0,0,0);}

        //check for betweenness
        if(isInBetween(cursor_x, cursor_y, win1_x, win1_y, 121, 166)){
            drawText(buffer, stride, bpp, 210,8, width, height, "FOCUSED", 0, 255,255,255, 245,10,10);
        }

        //draw time
        snprintf(buf, sizeof(buf), "%02d:%02d,%02d", t.hours, t.mins, t.secs);
        drawText(buffer, stride, bpp, 230, 336, width, height, buf, 0, 250,250,250, 40,40,40);

        if (isInBetween(cursor_x, cursor_y, win1_x + 154, win1_y + 109, 11, 11)){
            drawText(buffer, stride, bpp, 200,8, width, height, "FOCUSED", 0, 255,255,255, 10,255,10);
        } else if(isKeyPressed_L && isInBetween(cursor_x, cursor_y, win1_x + 154, win1_y + 109, 11, 11)){
            isTermOpened = 0;
        }

        //draw cursor and manage delta
        if(isTermOpened && isKeyPressed_L && isInBetween(cursor_x, cursor_y, win1_x + 85, win1_y - 5, 25, 140)){ 
            //(weird values in isInBetween for error correction and to avoid fails)
            drawSpriteAlpha(buffer, stride, bpp, cursor_x, cursor_y, width, height, 12, 10, cursor1);
            //update window position depending on delta
            win1_x += delta_x;
            win1_y += delta_y;
        } else {
            drawSpriteAlpha(buffer, stride, bpp, cursor_x, cursor_y, width, height, 12, 10, cursor);
        }

        //draw fps
        snprintf(buf, sizeof(buf), " FPS: %d ", fps);
        drawText(buffer, stride, bpp, 220, 8, width, height, buf, 0, 255,255,255, 0,0,0);
        
        //update last cursor position
        lastCursorX = cursor_x;
        lastCursorY = cursor_y;

        if(isKeyPressed_Y && cooldown < 0){isTermOpened = !isTermOpened; cooldown = 75;}

        if(isKeyPressed_START && isKeyPressed_R && cooldown < 0){SETTINGS = !SETTINGS; cooldown = 120;}
        if(SETTINGS){displaySettings(buffer, isKeyPressed_UP, isKeyPressed_DOWN, isKeyPressed_LEFT, isKeyPressed_RIGHT, isKeyPressed_A);}
        setBacklightLevel(brightness);
        
        if(isKeyPressed_X){takeScreenshot(buffer);}

        writeToBuffer(buffer, bpp, width, height, fb);
        usleep(frameskip * 1000); //should be set to 0 for less lag, only for debugging
    }
 
    system("stty echo");
    closeInput();
    return 0;
}
