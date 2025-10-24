#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <fcntl.h>     // open()
#include <unistd.h>    // read(), close(), sleep()
#include <linux/input.h> // struct input_event, EV_KEY, etc.
#include "input.h"
#define CURSOR_W 12
#define CURSOR_H 10

bool isKeyPressed_L = false;
bool isKeyPressed_R = false;
bool isKeyPressed_A = false;
bool isKeyPressed_B = false;
bool isKeyPressed_X = false;
bool isKeyPressed_Y = false;
bool isKeyPressed_UP = false;
bool isKeyPressed_DOWN = false;
bool isKeyPressed_LEFT = false;
bool isKeyPressed_RIGHT = false;
bool isKeyPressed_START = false;

static int buttons = -1;
static int mouse   = -1;
signed char data[3];

extern int cursor_x;
extern int cursor_y;

int initInput() {
    buttons = open("/dev/input/event0", O_RDONLY | O_NONBLOCK);
    mouse   = open("/dev/input/mouse0", O_RDONLY | O_NONBLOCK);

    if(buttons < 0) { perror("event0"); return -1; }
    if(mouse   < 0) { perror("mouse0"); return -1; }
    return 0;
}

void handleButtons() {
    struct input_event ev;
    ssize_t n = read(buttons, &ev, sizeof(ev));

    if (ev.type == EV_KEY) {
        if(n == sizeof(ev) && ev.type == EV_KEY) {
            switch(ev.code) {
                case 310: isKeyPressed_L = (ev.value == 1); break;
                case 311: isKeyPressed_R = (ev.value == 1); break;
                case 103: isKeyPressed_UP = (ev.value == 1); break;
                case 108: isKeyPressed_DOWN = (ev.value == 1); break;
                case 105: isKeyPressed_LEFT = (ev.value == 1); break;
                case 106: isKeyPressed_RIGHT = (ev.value == 1); break;
                case 304: isKeyPressed_A = (ev.value == 1); break;
                case 305: isKeyPressed_B = (ev.value == 1); break;
                case 307: isKeyPressed_X = (ev.value == 1); break;
                case 308: isKeyPressed_Y = (ev.value == 1); break;
                case 315: isKeyPressed_START = (ev.value == 1); break;
            }
        }
    }
    sleep(0.2);
}

void handleMouse() {
    // read mouse input
    int n = read(mouse, data, 3);
    if(n == 3){
        signed char dx = data[2];
        signed char dy = data[1];
        cursor_x += dx;
        cursor_y += dy;
        if(cursor_x < 0) cursor_x = 0;
        if(cursor_y < 0) cursor_y = 0;
        if(cursor_x > 240 - CURSOR_W) cursor_x = 240 - CURSOR_W;
        if(cursor_y > 400 - CURSOR_H) cursor_y = 400 - CURSOR_H;
        fflush(stdout);
    }
}

void closeInput() {
    if(buttons >= 0) close(buttons);
    if(mouse   >= 0) close(mouse);
}
