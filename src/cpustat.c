#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/times.h>
void monitor_pid(int target_pid) {
    char path[64];
    char buffer[1024];
    
    // Construct the path: /proc/[pid]/stat
    sprintf(path, "/proc/%d/stat", target_pid);

    int fd = syscall(SYS_open, path, O_RDONLY);
    if (fd < 0) {
        printf("Error: Could not open stats for PID %d. Permission denied or PID dead.\n", target_pid);
        return;
    }

    long bytes = syscall(SYS_read, fd, buffer, sizeof(buffer) - 1);
    buffer[bytes] = '\0';
    syscall(SYS_close, fd);

    // parse the string. 
    // Field 14 is the 14th space-separated value.
    char *ptr = buffer;
    int space_count = 0;
    int hz = sysconf(_SC_CLK_TCK);
    long utime = 0, stime = 0,starttime = 0;

    // Skip the first 13 fields
    while (*ptr && space_count < 13) {
        if (*ptr == ' ') space_count++;
        ptr++;
    }

    // Read Field 14 (utime) and 15 (stime)
    sscanf(ptr, "%ld %ld", &utime, &stime);

    printf("PID %d Usage: User=%ld ticks, System=%ld ticks\n", target_pid, utime, stime);
    while (*ptr && space_count < 21) if (*ptr++ == ' ') space_count++;
    sscanf(ptr, "%ld", &starttime); // Field 22

    // Get current system uptime in ticks
    struct tms t;
    long uptime_ticks = syscall(SYS_times, &t);

    // Calculate Age and Usage
    long total_ticks = utime + stime;
    long age_ticks = uptime_ticks - starttime;
    
    // Safety check for div by zero
    double usage = (age_ticks > 0) ? ((double)total_ticks / age_ticks) * 100.0 : 0;

    printf("PID: %d\n", target_pid);
    printf("Started at: %.2fs after boot\n", (double)starttime / hz);
    printf("Process Age: %.2fs\n", (double)age_ticks / hz);
    printf("Lifetime CPU Usage: %.2f%%\n", usage);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("cant use without pid!!!\n");
        return 1;
    }
    monitor_pid(atoi(argv[1]));
    return 0;
}