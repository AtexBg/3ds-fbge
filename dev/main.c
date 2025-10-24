#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "led.h"

int main() {
    system("clear");
    printf("testing R\n");
    setLED_asRGB(255,0,0);
    sleep(1);
    printf("testing G\n");
    setLED_asRGB(0,255,0);
    sleep(1);
    printf("testing B\n");
    setLED_asRGB(0,0,255);
    sleep(1);
    printf("testing White\n");
    setLED_asRGB(255,255,255);
    sleep(1);
    printf("clearing LED\n");
    clearLED();
    sleep(1);

    system("clear");
    printf("Fade of R, G, B and White LEDs for 255 iterations...\n");
    for(int i=0;i<255;i++){
        setLED_asRGB(i,0,0);
        usleep(16600);
    }

    for(int i=0;i<255;i++){
        setLED_asRGB(0,i,0);
        usleep(16600);
    }

    for(int i=0;i<255;i++){
        setLED_asRGB(0,0,i);
        usleep(16600);
    }

    for(int i=0;i<255;i++){
        setLED_asRGB(i,i,i);
        usleep(16600);
    }

    //de-init
    clearLED();
    return 0;
}
