#include "backlight.h"
#include <stdio.h>
#include <stdlib.h>

#define defaultBrightness 100

void setBacklightLevel(int backlightLevel){
    FILE *f = fopen("/sys/class/backlight/nintendo3ds-backlight/brightness", "w");

    if(f){
        fprintf(f, "%d", backlightLevel);  // write value to "*f"
        fflush(f);
        fclose(f);
    }
}

void setBacklight(int backlightState){
    if(backlightState == 0){
        setBacklightLevel(0);
    } else if(backlightState == 1) {
        setBacklightLevel(defaultBrightness);
    }
}
