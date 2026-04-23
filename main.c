#include <stdio.h>
#include <unistd.h> //sleep
#include <stdlib.h> //system
#include <stdint.h> //uint8_t
#include <string.h> //memcpy
#include <fcntl.h> //open flags
#include <sys/mman.h> //mmap
#include <stdbool.h> //bool type

//WARNING: This code is soo messy, some refactoring is needed (10/02/26)

//custom includes
#include "TOP_render.h"
#include "BTM_render.h"
#include "battery.h"
#include "input.h"
#include "fps.h"
#include "led.h"
#include "times.h"
#include "hitbox.h"
#include "settings.h"
#include "backlight.h"
#include "screenshot.h"
#include "keyboard.h"
#include "hardware.h"
#include "sysrq.h"

#define TOP_WIDTH 400
#define BOTTOM_WIDTH 320
#define COMMON_HEIGHT 240
#define STRIDE 720
#define BPP 3

#define TOP_FB_SIZE TOP_WIDTH*COMMON_HEIGHT*BPP
#define BTM_FB_SIZE BOTTOM_WIDTH*COMMON_HEIGHT*BPP

//executes something every f frames
#define EVERY(f) if(frames % f == 0) //PS: modulo isn't good for performaces

//i still need to replace that with structs/WM/AppState
int RUNNING = 1;
int SETTINGS = 0;
int win1_x = 50;
int win1_y = 50;
int isTermOpened = 1;
int focused = 0;
int bgIndex = 0;

//temporary update handler
typedef struct updateState_t{
    bool TOP_needsUpdate;
    bool BTM_needsUpdate;
} updateState_t;

//main frame counter
int frames = 0;

int main() {
    system("stty -echo"); //silent terminal
    char buf[50]; //multi-purpose buffer used with snprintf and drawText
    uint8_t *backgrounds[4] = {(uint8_t*)background0, (uint8_t*)background1, (uint8_t*)background2, (uint8_t*)background3};

    //INIT TOP SCREEN
    int TOP_FB_DEV = open("/dev/fb0", O_RDWR); //Open framebuffer device as TOP_FB_DEV
    //(this then maps the file descriptor of /dev/fb0 to a pointer in memory for faster write speeds)
    uint8_t *TOP_FB = mmap(NULL, TOP_FB_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, TOP_FB_DEV, 0); 
    
    //INIT BOTTOM SCREEN
    uint8_t *BTM_FB = initBottomFramebuffer(); //no /dev/fb1, i had to make my own memory mapping and custom lib
    
    //ALLOCATE BACKBUFFERS
    uint8_t *TOP_BACKBUFFER = malloc(TOP_WIDTH * COMMON_HEIGHT * BPP); //Allocate buffer
    uint8_t *BTM_BACKBUFFER = malloc(BOTTOM_WIDTH * COMMON_HEIGHT * BPP);

    //copy bottom framebuffer to backbuffer once to keep the touch keyboard displayed
    memcpy(BTM_BACKBUFFER, BTM_FB, BTM_FB_SIZE);

    initInput();  //---|
    initKeyboard(); //-- Some init functions
    makeDirForScreenshots();

    //init structs for I/O data
    struct cursor_t cursorState = {.x=120, .y=200};
    //struct scCnt_t sc; //counter for screenshots
    struct keys_t keys; //buttons for input handler
    struct updateState_t updateState;

    BTM_clearScreen(BTM_BACKBUFFER);

    //PRE-RENDER EVERYTHING ON THE BOTTOM SCREEN TO AVOID I/O BOTTLENECK
    //drawText(BTM_BACKBUFFER, 134,1, "CPU USAGE:     %", 0,0,0, 0xFF,0xFF,0xFF);
    drawText(BTM_BACKBUFFER, 134,1, "CPU USAGE: N/A", 0,0,0, 0xFF,0xFF,0xFF);
    drawText(BTM_BACKBUFFER, 124,176, "FPS: ", 0,0,0, 0xFF,0xFF,0xFF);
    drawText(BTM_BACKBUFFER, 124,1, "FREE RAM:      MB", 0,0,0, 0xFF,0xFF,0xFF);
    drawText(BTM_BACKBUFFER, 114,1, "BATTERY:", 0,0,0, 0xFF,0xFF,0xFF);

    //add project name and logo
    drawText(BTM_BACKBUFFER, 5,250, "3DS-FBGE", 0xFF,0xFF,0xFF, 0,0,0);
    drawSprite(BTM_BACKBUFFER, 256,256, 48,48, fbge_logo);

    while(RUNNING) {
        handleButtons(&keys);
        if(!SETTINGS){handleMouse(&cursorState);}
        if(!SETTINGS && isTermOpened){handleKeyboard();}
        memcpy(TOP_BACKBUFFER, backgrounds[bgIndex], TOP_FB_SIZE); //draw background
        int fps = getCurrentFPS();
        Time t = getCurrentTime();
        frames++;

        //check for window to stay on screen to avoid segfault
        //THIS NEEDS TO BE REIMPLEMENTED IN A PROPER WINDOW MANAGER 
        if(win1_x < 0) win1_x = 0;
        if(win1_y < 0) win1_y = 0;
        if(win1_x > COMMON_HEIGHT - 121) win1_x = COMMON_HEIGHT - 121;
        if(win1_y > TOP_WIDTH - 166) win1_y = TOP_WIDTH - 166;

        if(isTermOpened){drawSprite(TOP_BACKBUFFER, win1_x, win1_y,TERM_W, TERM_H, termWin);}
        if(isTermOpened){drawText(TOP_BACKBUFFER, win1_x + 109, win1_y + 3, "TERMINAL", 255,255,255, 1,119,13);}
        if(isTermOpened){drawText(TOP_BACKBUFFER, win1_x + 94, win1_y + 3, termBuffer, 255,255,255, 0,0,0);}

        if(fps >= 50){ //if fps is more than 50
            SetLED(0x00, 0xFF, 0x00); //light up green
        } else if(fps < 50 && fps >= 20){ //if fps is between 50 and 20
            SetLED(0xFF, 0xCE, 0x00); //light up yellow
        } else if(fps < 20){ //if fps is under 20
            SetLED(0xFF, 0x00, 0x00); //light up red
        }

        drawSprite(TOP_BACKBUFFER, 0, 0, 20, 400, taskbar); //draw taskbar

        //draw info on bottom screen
        EVERY(75){ //draw info on bottom screen every 75 frames
            // snprintf(buf, sizeof(buf), "%.1f", get_cpu_usage()); //cpu usage
            // drawText(BTM_BACKBUFFER, 134,88, buf, 0,0,0, 0xFF,0xFF,0xFF);
            snprintf(buf, sizeof(buf), "%.1f", (getFreeMemory()/1024.0)); //free ram
            drawText(BTM_BACKBUFFER, 124,80, buf, 0,0,0, 0xFF,0xFF,0xFF);
            
            int batteryLevel = getBatteryLevel();
            if(batteryLevel == 255 || -1){ //failsafe in case of SysFs read failure
                snprintf(buf, sizeof(buf), "--%%");
            } else {
                snprintf(buf, sizeof(buf), "%02d%%", batteryLevel); //bat level
            }

            drawText(BTM_BACKBUFFER, 114,72, buf, 0,0,0, 0xFF,0xFF,0xFF);
            snprintf(buf, sizeof(buf), "%02d:%02d,%02d", t.hours, t.mins, t.secs); //time
            drawText(BTM_BACKBUFFER, 134,176, buf, 250,250,250, 40,40,40); //will be desync bc the DE runs under 75FPS, but it's fine
            updateState.BTM_needsUpdate = true;
        }

        EVERY(20){ //FPS
            snprintf(buf, sizeof(buf), "%d", fps); //fps
            drawText(BTM_BACKBUFFER, 124,216, buf, 0,0,0, 0xFF,0xFF,0xFF);
            updateState.BTM_needsUpdate = true;
        }
        
        //update last cursor position
        //x delta = x pos - last x pos, and so on...
        cursorState.dX = cursorState.x - cursorState.lastX;
        cursorState.dY = cursorState.y - cursorState.lastY;
        cursorState.lastX = cursorState.x;
        cursorState.lastY = cursorState.y;

        //draw cursor and manage delta
        if(keys.L){ 
            drawSpriteAlpha(TOP_BACKBUFFER, cursorState.x, cursorState.y, 12, 10, cursor1); //grabbing cursor
            //update window position depending on delta
            win1_x += cursorState.dX;
            win1_y += cursorState.dY;
            updateState.TOP_needsUpdate = true;
        } else {
            drawSpriteAlpha(TOP_BACKBUFFER, cursorState.x, cursorState.y, 12, 10, cursor); //normal cursor
            updateState.TOP_needsUpdate = true;
        }

        //SETTINGS is a boolean flag to see whether we should display the settings
        if(keys.START && keys.R){
            EVERY(10){ //every 10 frames to avoid spamming it 
                SETTINGS = !SETTINGS;
            }
        }

        if(SETTINGS){
            displaySettings(TOP_BACKBUFFER, &keys);
            updateState.TOP_needsUpdate = true;
        }
        if(keys.START && keys.X){ //take screenshot of both screens if START and X are pressed
            //somehow this make the kernel I/O handler crash after 2 or 3 screenshots, bruhh
            //TOP_takeScreenshot(TOP_BACKBUFFER, &sc);
            //BTM_takeScreenshot(BTM_BACKBUFFER, &sc);
        }
        if(keys.A && keys.START){break;} //quit if A+START pressed
        if(keys.A && keys.B && keys.L && keys.R){
            SYS_SyncFS();
            SYS_PowerOff();
        } //sync and poweroff console when A+B+L+R are pressed

        //update the framebuffers if needed
        if(updateState.TOP_needsUpdate){
            TOP_flushToFramebuffer(TOP_FB, TOP_BACKBUFFER);  //refresh top screen
            updateState.TOP_needsUpdate = false;
        }
        if(updateState.BTM_needsUpdate){
            BTM_flushToFramebuffer(BTM_FB, BTM_BACKBUFFER); //refresh bottom screen
            updateState.BTM_needsUpdate = false;
        }
    }
 
    //free backbuffers to avoid memory leaks
    free(BTM_BACKBUFFER);
    free(TOP_BACKBUFFER);

    munmapBottomFramebuffer(); //unmap BTM_FB to avoid again memory leaks
    system("stty echo"); //un-silent terminal
    SYS_SyncFS(); //syncs the filesystem buffer so written files aren't getting corrupted
    clearLED();
    closeInput();
    return 0;
}