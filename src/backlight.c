#include "backlight.h"
#include <stdio.h>
#include <stdlib.h>

#define defaultBrightness 100

int setBacklightLevel(int backlightLevel){
    // If brightness is higher than maximun allowed by the kernel return -1 as error
    if(backlightLevel > 192){return -1;}
    
    FILE *f = fopen("/sys/class/backlight/nintendo3ds-backlight/brightness", "w");

    if(f){
        fprintf(f, "%d", backlightLevel);  // write value to "*f"
        fflush(f);
        fclose(f);
        return 0;
    }
}

int setBacklight(int backlightState){

    //Return -1 if value isn't 0 or 1
    if(backlightState != 0 && backlightState != 1){return -1;}

    if(backlightState == 0){
        setBacklightLevel(0);
    } else if(backlightState == 1) {
        setBacklightLevel(defaultBrightness);
    }

    return 0;
}
