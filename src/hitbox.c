#include <stdio.h>

int isInBetween(int x, int y, int x0, int y0, int img_width, int img_height){
    if(x >= x0 && x <= (x0 + img_width) &&
       y >= y0 && y <= (y0 + img_height)) {
        return 1;
    } else {
        return 0;
    }
}
