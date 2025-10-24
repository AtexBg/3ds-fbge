#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include "draw.h"

int selectedSetting = 0;
int settingsX = 50;
int settingsY = 70;
int inputDelay = 32;
int bnDelay = 8;
extern int RUNNING;
extern int brightness;
extern int frameskip;
char desc[63]; //55 : Max number of chars allowed for desc (without "\n")

//NOTE: This code is shit, should be fixed asap
// ----------------------------------------
// Settings window size: 260*140
// Desc can be 11 chars per line
// Win edge is at x70,y50
// Options are at x+21 and y+112 on the bottom..
// ..with y18 of difference or each line
// Desc is at x+153 and y+77 on the top, and 14px down

void displaySettings(uint8_t *buffer, int UP, int DOWN, int LEFT, int RIGHT, int A) {
    char setting[128];
    inputDelay--; //Decrease delay used to prevent button spamming
    bnDelay--; //Decrease delay for brightness changing
    //draw settings background
    drawSprite(buffer, 720, 3, 50, 70, 240, 400, 140, 260, settingsBg);
    //Move in settings
    if(DOWN && inputDelay < 0){selectedSetting += 1; inputDelay = 32;}
    if(UP && inputDelay < 0){selectedSetting -= 1; inputDelay = 32;}

    if(selectedSetting == 0){ //BRIGHTNESS
        snprintf(setting, sizeof(setting), "CHANGE THE\nBRIGHTNESS\nOF THIS\nSCREEN\nVALUE: %d", brightness);
        drawText(buffer,720,3, settingsX+112, settingsY+21, 240,400, "BRIGHTNESS", 0, 0,0,0, 255,255,87);
        drawText(buffer,720,3, settingsX+77, settingsY+153, 240,400, setting, 3, 0,0,0, 255,255,255);
        if(LEFT && brightness > 0){brightness -= 1;} else if(RIGHT && brightness < 190){brightness +=1;}
    } else if(selectedSetting == 1){ //FRAMESKIP
        snprintf(setting, sizeof(setting), "AMOUNT OF\nMILLISECOND\nTO SKIP AT\nEACH FRAME.\nVALUE : %d", frameskip);
        drawText(buffer,720,3, settingsX+95, settingsY+21, 240,400, "FRAMESKIP", 0, 0,0,0, 255,255,87);
        drawText(buffer,720,3, settingsX+77, settingsY+153, 240,400, setting, 3, 0,0,0, 255,255,255);
        if(LEFT && frameskip > 0){frameskip -= 1;} else if(RIGHT && frameskip < 64){frameskip +=1;}
    } else if(selectedSetting == 2){ //WHATEVER
        drawText(buffer,720,3, settingsX+78, settingsY+21, 240,400, "SYS INFO", 0, 0,0,0, 255,255,87);
        drawText(buffer,720,3, settingsX+77, settingsY+153, 240,400, "PRESS A TO\nSHOW SYSTEM\nINFORMATION", 3, 0,0,0, 255,255,255);
        // if(A){showSystemInfo();}
    } else if(selectedSetting == 3){ //EXIT
        drawText(buffer,720,3, settingsX+61, settingsY+21, 240,400, "EXIT", 0, 0,0,0, 250,10,10);
        drawText(buffer,720,3, settingsX+77, settingsY+153, 240,400, "EXIT THE\nDESKTOP\nENVIRONNE-\n-MENT.", 3, 0,0,0, 255,255,255);
        if(A){RUNNING = 0;}
    }
}
