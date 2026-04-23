#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <fcntl.h>     // open()
#include <unistd.h>    // read(), close(), sleep()
#include <linux/input.h> // struct input_event, EV_KEY, etc.
#include "input.h"
#define CURSOR_W 12
#define CURSOR_H 10

static int buttons = -1;
static int mouse   = -1;
signed char data[3];

int initInput() {
    buttons = open("/dev/input/event0", O_RDONLY | O_NONBLOCK);
    mouse   = open("/dev/input/mouse0", O_RDONLY | O_NONBLOCK);

    if(buttons < 0) { perror("event0"); return -1; }
    if(mouse   < 0) { perror("mouse0"); return -1; }
    return 0;
}

void handleButtons(struct keys_t *keys) {
    struct input_event ev;
    ssize_t n = read(buttons, &ev, sizeof(ev));

    if (ev.type == EV_KEY) {
        if(n == sizeof(ev) && ev.type == EV_KEY) {
            switch(ev.code) {
                case 310: keys->L = (ev.value == 1); break;
                case 311: keys->R = (ev.value == 1); break;
                case 103: keys->UP = (ev.value == 1); break;
                case 108: keys->DOWN = (ev.value == 1); break;
                case 105: keys->LEFT = (ev.value == 1); break;
                case 106: keys->RIGHT = (ev.value == 1); break;
                case 304: keys->A = (ev.value == 1); break;
                case 305: keys->B = (ev.value == 1); break;
                case 307: keys->X = (ev.value == 1); break;
                case 308: keys->Y = (ev.value == 1); break;
                case 315: keys->START = (ev.value == 1); break;
            }
        }
    }
    sleep(0.2);
}

void handleMouse(cursor_t *c) {
    // read mouse input
    int n = read(mouse, data, 3);
    if(n == 3){
        signed char dx = data[2];
        signed char dy = data[1];
        c->x += dx;
        c->y += dy;
        if(c->x < 0) c->x = 0;
        if(c->y < 0) c->y = 0;
        if(c->x > 240 - CURSOR_W) c->x = 240 - CURSOR_W;
        if(c->y > 400 - CURSOR_H) c->y = 400 - CURSOR_H;
        fflush(stdout);
    }
}

void closeInput() {
    if(buttons >= 0) close(buttons);
    if(mouse   >= 0) close(mouse);
}
