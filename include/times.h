#ifndef TIMES_H
#define TIMES_H

typedef struct {
    int hours;
    int mins;
    int secs;
} Time;

Time getCurrentTime(void);

#endif
