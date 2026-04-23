#ifndef KEYBOARD_H
#define KEYBOARD_H

extern char termBuffer[4096];
extern char cmdOut[4096];

void executeCommand(const char *cmd);
void handleKeyboard();
void initKeyboard();

#endif
