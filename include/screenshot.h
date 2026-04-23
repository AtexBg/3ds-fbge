#ifndef SCREENSHOT_H
#define SCREENSHOT_H
#include <stdint.h>

struct scCnt_t{
    int topCnt;
    int btmCnt;
};

void TOP_takeScreenshot(uint8_t *buffer, struct scCnt_t *scCnt);
void BTM_takeScreenshot(uint8_t *buffer, struct scCnt_t *scCnt);
void makeDirForScreenshots();

#endif 
