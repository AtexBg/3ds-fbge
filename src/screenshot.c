#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/stat.h>

int ss_cooldown = 16;
int ss_count = 0;

void takeScreenshot(uint8_t *buffer) {
    mkdir("/mnt/screenshots", 0777); //make screenshots directory
    ss_cooldown--; //Decrement cooldown

    if(ss_cooldown < 0) {// ss_cooldown < 0
        ss_cooldown = 120;  //Reset delay counter
        char filename[256];
        snprintf(filename, sizeof(filename), "/mnt/screenshots/screenshot_%d.bin", ss_count);
        ss_count++; //increment screenshot counter

        FILE *f = fopen(filename, "wb"); //open file
        // framebuffer size (width*height*bpp, always 288K)
        size_t size = 288000;

        // write buffer to file
        fwrite(buffer, 1, size, f);
        printf("Screenshot %d written to file\n", ss_count);
        fclose(f);
    }
}
