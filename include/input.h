#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>

extern bool isKeyPressed_L;
extern bool isKeyPressed_R;
extern bool isKeyPressed_A;
extern bool isKeyPressed_B;
extern bool isKeyPressed_X;
extern bool isKeyPressed_Y;
extern bool isKeyPressed_UP;
extern bool isKeyPressed_DOWN;
extern bool isKeyPressed_LEFT;
extern bool isKeyPressed_RIGHT;
extern bool isKeyPressed_START;

typedef struct cursor_t {
    int x;
    int y;
    int dX;
    int dY;
    int lastX;
    int lastY;
} cursor_t;

typedef struct keys_t {
    bool L;
    bool R;
    bool A;
    bool B;
    bool X;
    bool Y;
    bool UP;
    bool DOWN;
    bool LEFT;
    bool RIGHT;
    bool START;
} keys_t;

int initInput();     // open devices
void closeInput();   // close devices
void handleButtons(keys_t *keys); // updates buttons
void handleMouse(cursor_t *c);   // update mouse position

#endif
