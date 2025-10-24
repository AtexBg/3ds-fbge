#include "battery.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

int getBatteryLevel(void) {
    FILE *f = fopen("/sys/class/power_supply/battery/capacity", "r");
    if (!f) return -1; //Returns -1 incase of an error

    int batteryLevel = -1; //same here
    if (fscanf(f, "%d", &batteryLevel) != 1) {
        fclose(f);
        return -1;
    }

    fclose(f);
    return batteryLevel;
}

int getBatteryStatus(void) {
    //opne file for battery status 
    FILE *f = fopen("/sys/class/power_supply/battery/status", "r");
    if (!f) return -1;

    char status[32];
    bool charging = false;

    if (fgets(status, sizeof(status), f)) {
        status[strcspn(status, "\n")] = 0; //remove the "\n"

        if (strcmp(status, "Charging") == 0) {
            charging = true;
        } else { // Set charging state as a boolean value
            charging = false;
        }
    }

    fclose(f);
    return charging;
}

char * getBatteryIcon(int level) {
    // Get battery state from level to use a different icon
    // e.g. 0 = FULL; 1 = HIGH; 2 = MED; 3 = LOW; 4 = EMPTY
    if(level >= 95 && level <= 100){return "f";}
    else if(level >= 60 && level <= 94){return "h";}
    else if(level >= 30 && level <= 59){return "m";}
    else if(level >= 6 && level <= 29){return "l";}
    else if(level >= 0 && level <= 5){return "e";}
}
