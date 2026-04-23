#include "times.h"
#include <time.h>

Time getCurrentTime(void) {
    time_t now = time(NULL);
    struct tm *local = localtime(&now);

    Time t;
    t.hours = local->tm_hour;
    t.mins  = local->tm_min;
    t.secs  = local->tm_sec;

    return t;
}
