#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/stat.h>
#include "screenshot.h"

// char logBuffer[40];

#define TOP_HEIGHT 400
#define BTM_HEIGHT 320
#define COMMON_WIDTH 240

//OLD RAW BIN IMPLEMENTATION REMOVED
// void takeScreenshot(uint8_t *buffer) {
//     mkdir("/mnt/screenshots", 0777); //make screenshots directory
//     ss_cooldown--; //Decrement cooldown

//     if(ss_cooldown < 0) {// ss_cooldown < 0
//         ss_cooldown = 120;  //Reset delay counter
//         char filename[256];
//         snprintf(filename, sizeof(filename), "/mnt/screenshots/screenshot_%d.bin", ss_count);
//         ss_count++; //increment screenshot counter

//         FILE *f = fopen(filename, "wb"); //open file
//         // framebuffer size (width*height*bpp, always 288K)
//         size_t size = 288000;

//         // write buffer to file
//         fwrite(buffer, 1, size, f);
//         printf("Screenshot %d written to file\n", ss_count);
//         fclose(f);
//     }
// }

#define TOP_WIDTH 400

void makeDirForScreenshots(){
    mkdir("/mnt/screenshots", 0777);
}

void TOP_takeScreenshot(uint8_t *buffer, struct scCnt_t *scCnt) {
    char filename_top[256];
    snprintf(filename_top, sizeof(filename_top), "/mnt/screenshots/top_%d.bmp", scCnt->topCnt);
    scCnt->topCnt++;

    FILE *imageFile = fopen(filename_top, "wb");
    if(!imageFile) return;

    // BMP file header (14 bytes)
    uint8_t fileHeader[14] = { 
        0x42, 0x4d, 0x36, 0x65, 0x04, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x36, 0x00, 0x00, 0x00
    };

    // DIB header (40 bytes)
    uint8_t DIBHeader[40] = {
        0x28,0x00,0x00,0x00,   // header size
        0x90,0x01,0x00,0x00,   // width = 400
        0xF0,0x00,0x00,0x00,   // height = 240 (bottom-up)
        0x01,0x00,             // planes
        0x18,0x00,             // 24 bits per pixel
        0x00,0x00,0x00,0x00,   // compression
        0x00,0x65,0x04,0x00,   // image size (dummy)
        0x13,0x0b,0x00,0x00,   // X pixels/meter
        0x13,0x0b,0x00,0x00,   // Y pixels/meter
        0x00,0x00,0x00,0x00,   // colors in palette
        0x00,0x00,0x00,0x00    // important colors
    };

    fwrite(fileHeader, 1, 14, imageFile);
    fwrite(DIBHeader, 1, 40, imageFile);

    // padding pour aligner chaque ligne à 4 bytes
    uint32_t row_bytes = ((TOP_WIDTH * 3 + 3) / 4) * 4;
    uint8_t padding[3] = {0,0,0};

    uint8_t rowBuffer[TOP_WIDTH*3]; // buffer d’une ligne

    for(int y = TOP_HEIGHT-1; y >= 0; y--) { // bottom-up
        for(int x = 0; x < TOP_WIDTH; x++) {
            uint32_t src_idx = (y * TOP_WIDTH + x) * 3;
            rowBuffer[x*3 + 0] = buffer[src_idx];   // B
            rowBuffer[x*3 + 1] = buffer[src_idx+1]; // G
            rowBuffer[x*3 + 2] = buffer[src_idx+2]; // R
        }
        // écrire la ligne entière d’un coup
        fwrite(rowBuffer, 1, TOP_WIDTH*3, imageFile);

        // écrire le padding si nécessaire
        fwrite(padding, 1, row_bytes - TOP_WIDTH*3, imageFile);
    }

    fclose(imageFile);
}

// void TOP_takeScreenshot(uint8_t *buffer, struct scCnt_t *scCnt) {
    
//     char filename_top[256];
//     snprintf(filename_top, sizeof(filename_top), "/mnt/screenshots/top_%d.bmp", scCnt->topCnt);
//     scCnt->topCnt++; //increment top screenshot coutner
//         FILE *imageFile = fopen(filename_top, "wb");
//         if(!imageFile) return;

//         //headers are hardcoded, no need to change them
//         uint32_t row_bytes = ((3 * TOP_HEIGHT + 3) / 4) * 4; //if it works, don't touch it
//         //init file header
//         uint8_t fileHeader[14] = { 
//             0x42, 0x4d, 0x36, 0x65, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00
//         };
//         //init BITMAPINFOHEADER
//         uint8_t DIBHeader[40] = {
//             0x28, 0x00, 0x00, 0x00, 0x90, 0x01, 0x00, 0x00, 0x10, 0xff, 0xff, 0xff, 0x01, 0x00, 0x18, 0x00, //0x00009001 = 400px in little-endian
//             0x00, 0x00, 0x00, 0x00, 0x00, 0x65, 0x04, 0x00, 0x13, 0x0b, 0x00, 0x00, 0x13, 0x0b, 0x00, 0x00,
//             0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//         };

//         //write headers to file
//         fwrite(fileHeader, 1, 14, imageFile);
//         fwrite(DIBHeader, 1, 40, imageFile);

//         uint8_t padding[3] = {0,0,0};
//         for(int y = COMMON_WIDTH-1; y >=0; y--) { // bottom-up
//             for(int x = 0; x < TOP_HEIGHT; x++) {
//                 uint32_t src_idx = (x * COMMON_WIDTH + y) * 3;
//                 uint8_t b = buffer[src_idx];
//                 uint8_t g = buffer[src_idx+1];
//                 uint8_t r = buffer[src_idx+2];
//                 fwrite(&b,1,1,imageFile);
//                 fwrite(&g,1,1,imageFile);
//                 fwrite(&r,1,1,imageFile);
//             }
//             fwrite(padding,1,(row_bytes - TOP_HEIGHT*3),imageFile);
//         }

//         fclose(imageFile);

//         //logs not implemented yet 
//         //snprintf(logBuffer, sizeof(logBuffer), "[+] TOOK SCREENSHOT top_%d.bmp", ss_count);
//         //BTM_addInBottomTerminal(logBuffer);
// }

void BTM_takeScreenshot(uint8_t *buffer, struct scCnt_t *scCnt) { //IT WORKS (sometimes)
    
        char filename_btm[256];
        snprintf(filename_btm, sizeof(filename_btm), "/mnt/screenshots/bottom_%d.bmp", scCnt->btmCnt);
        scCnt->btmCnt++; //increment top screenshot coutner
        FILE *imageFile = fopen(filename_btm, "wb");
        if(!imageFile) return;

        //headers are hardcoded, no need to change them
        uint32_t row_bytes = ((3 * BTM_HEIGHT + 3) / 4) * 4; //if it works, don't touch it
        //init file header
        uint8_t fileHeader[14] = { 
            0x42, 0x4d, 0x36, 0x65, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00
        };
        //init BITMAPINFOHEADER
        uint8_t DIBHeader[40] = {
            0x28, 0x00, 0x00, 0x00, 0x40, 0x01, 0x00, 0x00, 0x10, 0xff, 0xff, 0xff, 0x01, 0x00, 0x18, 0x00, //0x00004001 = 320px in little-endian
            0x00, 0x00, 0x00, 0x00, 0x00, 0x65, 0x04, 0x00, 0x13, 0x0b, 0x00, 0x00, 0x13, 0x0b, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        };

        //write headers to file
        fwrite(fileHeader, 1, 14, imageFile);
        fwrite(DIBHeader, 1, 40, imageFile);

        uint8_t padding[3] = {0,0,0};
        for(int y = COMMON_WIDTH-1; y >=0; y--) { // bottom-up
            for(int x = 0; x < BTM_HEIGHT; x++) {
                uint32_t src_idx = (x * COMMON_WIDTH + y) * 3;
                uint8_t b = buffer[src_idx];
                uint8_t g = buffer[src_idx+1];
                uint8_t r = buffer[src_idx+2];
                fwrite(&b,1,1,imageFile);
                fwrite(&g,1,1,imageFile);
                fwrite(&r,1,1,imageFile);
            }
            fwrite(padding,1,(row_bytes - BTM_HEIGHT*3),imageFile);
        }

        fclose(imageFile);
        //logs not implemented yet 
        //snprintf(logBuffer, sizeof(logBuffer), "[+] TOOK SCREENSHOT top_%d.bmp", ss_count);
        //BTM_addInBottomTerminal(logBuffer);
}