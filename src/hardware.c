#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

long getFreeMemory() {
    //meminfo contains data about RAM, including free amount
    FILE *f = fopen("/proc/meminfo", "r");
    
    char line[256];
    long freeMemory = 0;

    //parse output of the command to fetch information
    while (fgets(line, sizeof(line), f)) {
        if (strncmp(line, "MemAvailable:", 13) == 0) {
            sscanf(line + 13, "%ld", &freeMemory);break;
        }
    }

    fclose(f);
    return freeMemory; //returns amount of free RAM in kB
}

typedef struct {
    unsigned long long user, nice, system, idle, iowait, irq, softirq, steal;
} CPUStats;


//vibe-coded and broken funcs to calculate the CPU usage (always hard on Linux, especially for me because i hate parsing), it's broken for now
int read_cpu_stats(CPUStats *stats) {
    FILE *fp = fopen("/proc/stat", "r");
    if (!fp) return -1;

    int ret = fscanf(fp, "cpu  %llu %llu %llu %llu %llu %llu %llu %llu",
                     &stats->user, &stats->nice, &stats->system, &stats->idle,
                     &stats->iowait, &stats->irq, &stats->softirq, &stats->steal);
    fclose(fp);

    return (ret == 8) ? 0 : -1;
}

//Calculate CPU Usage between 2 calls
double get_cpu_usage() {
    CPUStats s1, s2;
    if (read_cpu_stats(&s1) != 0) return -1;
    usleep(100000);
    if (read_cpu_stats(&s2) != 0) return -1;

    unsigned long long idle1 = s1.idle + s1.iowait;
    unsigned long long idle2 = s2.idle + s2.iowait;

    unsigned long long total1 = s1.user + s1.nice + s1.system + s1.idle +
                                s1.iowait + s1.irq + s1.softirq + s1.steal;
    unsigned long long total2 = s2.user + s2.nice + s2.system + s2.idle +
                                s2.iowait + s2.irq + s2.softirq + s2.steal;

    unsigned long long totalDelta = total2 - total1;
    unsigned long long idleDelta = idle2 - idle1;

    return 100.0 * (totalDelta - idleDelta) / totalDelta;
}