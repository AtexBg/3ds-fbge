#include <stdio.h>
#include <stdlib.h> //system

void SYS_PowerOff(){ // o = poweroff
    for(int i=0;i<50;i++){
        printf("\x1b[31mPOWERING OFF SYSTEM NOW /!\\\n");
    }
    system("echo o > /proc/sysrq-trigger");
}

void SYS_SyncFS(){ // s = sync
    system("echo s > /proc/sysrq-trigger");
}

void SYS_Crash(){ // c = crash
    system("echo c > /proc/sysrq-trigger");
}