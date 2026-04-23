#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

#include "TOP_render.h"
#include "backlight.h"
#include "background.h"
#include "input.h"

int selectedSetting = 0;
int settingsX = 50;
int settingsY = 70;
int inputDelay = 32;
extern int RUNNING;
int brightness = 90;
int backgroundIndex = 0;
char desc[63]; //55 : Max number of chars allowed for desc (without "\n")

//to display text at a specific line, starting from line 0
#define LINE(n) (153 - (18*n))

//NOTE: This code is shit, should be fixed asap
// ----------------------------------------
// Settings window size: 260*140
// Desc can be 11 chars per line
// Win edge is at x70,y50
// Options are at x+21 and y+112 on the bottom..
// ..with y18 of difference or each line
// Desc is at x+153 and y+77 on the top, and 14px down

void displaySettings(uint8_t *buffer, struct keys_t *keys) {
    char setting[128];
    inputDelay--; //Decrease delay used to prevent button spamming

    //draw settings background
    drawSprite(buffer, 50, 70, 140, 260, settingsBg);

    //Move in settings
    if(keys->DOWN && inputDelay < 0 && selectedSetting != 5){
        selectedSetting ++; inputDelay = 6;
    }
    if(keys->UP && inputDelay < 0 && selectedSetting !=0){
        selectedSetting --; inputDelay = 6;
    }

    switch(selectedSetting){
    case 0: //BRIGHTNESS
        snprintf(setting, sizeof(setting), "CHANGE THE\nBRIGHTNESS\nOF THE\nSCREENS\n\nVALUE: %d", brightness);
        drawText(buffer, LINE(0), settingsY+21, "BRIGHTNESS", 0,0,0, 255,255,87);
        drawText(buffer, settingsX+77, settingsY+153, setting, 0,0,0, 255,255,255);
        if(keys->LEFT && brightness > 0){
            brightness--;
        } else if (keys->RIGHT && brightness < 190){
            brightness++;
        }
        setBacklightLevel(brightness);
        break;

    case 1: //BACKGROUND
        char* bgName = getNameFromBackgroundIndex();
        snprintf(setting, sizeof(setting), "CHANGE THE\nBACKGROUND\nIMAGE\n\n- %s", bgName);
        drawText(buffer, LINE(1), settingsY+21, "BACKGROUND", 0,0,0, 255,255,87);
        drawText(buffer, settingsX+77, settingsY+153, setting, 0,0,0, 255,255,255);
        if(keys->LEFT && backgroundIndex > 0 && inputDelay < 0){
            inputDelay = 8;
            backgroundIndex--;
        } else if (keys->RIGHT && backgroundIndex < 3 && inputDelay < 0){
            inputDelay = 8;
            backgroundIndex++;
        }
        setBackgroundFromIndex(backgroundIndex);
        break;


    case 2:
        drawText(buffer, LINE(2), settingsY+21, "PLACEHOLDER", 0,0,0, 255,255,87);
        drawText(buffer, settingsX+77, settingsY+153, "THIS IS\nPLACEHOLDER\nTEXT", 0,0,0, 255,255,255);
        
        // if(keys->A && inputDelay < 0){
        //     inputDelay = 8;
        //     *ptr = !*ptr; //toggle a pointer, i've seen worse...
        // }
        break;
    case 3:
        drawText(buffer, LINE(3), settingsY+21, "PLACEHOLDER", 0,0,0, 255,255,87);
        break;
    case 4:
        drawText(buffer, LINE(4), settingsY+21, "PLACEHOLDER", 0,0,0, 255,255,87);
        break;
    case 5: //EXIT THE DE
        drawText(buffer, LINE(5), settingsY+21, "EXIT", 0,0,0, 250,10,10);
        drawText(buffer, settingsX+77, settingsY+153, "EXIT THE\nDESKTOP\nENVIRONNE-\n-MENT.", 0,0,0, 255,255,255);
        if(keys->A){RUNNING = 0;}
        break;
    }
}

