#include <stdio.h>
#include <unistd.h>
#include "led.h"

void setLED_asRGB(int r, int g, int b) {
    FILE *led; //Using "led" as pointer for LED devices

    led = fopen("/sys/class/leds/nintendo3ds:red:notification/brightness","w");
    if(led){ fprintf(led,"%d",r); fclose(led); } //Writing to RED

    led = fopen("/sys/class/leds/nintendo3ds:green:notification/brightness","w");
    if(led){ fprintf(led,"%d",g); fclose(led); } //Writing to GREEN

    led = fopen("/sys/class/leds/nintendo3ds:blue:notification/brightness","w");
    if(led){ fprintf(led,"%d",b); fclose(led); } //Writing to BLUE
}

void clearLED(void) {
    for(int i=0;i<3;i++){ //3 times so it won't glitch
        setLED_asRGB(0,0,0);
        usleep(1000);
    }
}
