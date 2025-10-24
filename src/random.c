#include <stdio.h>
#include <stdint.h>
#include <sys/random.h>
#include <unistd.h>

int getRandom(int min, int max) {
    
    if (min > max) {int tmp = min;min = max;max = tmp;}
    uint32_t r;

    // method to prevent fails in randomness
    uint32_t range = (uint32_t)(max - min + 1);
    uint32_t limit = UINT32_MAX - (UINT32_MAX % range);
    while (r >= limit) {
        if (getrandom(&r, sizeof(r), 0) != sizeof(r)) {
            perror("getrandom");
            return min;
        }
    }
    return min + (r % range);
}

