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
extern int cursor_x;
extern int cursor_y;

int initInput();     // open devices
void closeInput();   // close devices
void handleButtons(); // updates buttons
void handleMouse();   // update mouse position

#endif
