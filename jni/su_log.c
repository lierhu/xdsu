#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <limits.h>
#include <sys/uio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "su_log.h"
void exec_log(int priority, const char* fmt, ...) {
    static int log_fd = -1;
    struct iovec vec[3];
    va_list args;
    char msg[PATH_MAX];

    if (log_fd < 0) {
        log_fd = open("/dev/log/main", O_WRONLY);
        if (log_fd < 0) {
            return;
        }
    }

    va_start(args, fmt);
    vsnprintf(msg, PATH_MAX, fmt, args);
    va_end(args);

    vec[0].iov_base   = (unsigned char *) &priority;
    vec[0].iov_len    = 1;
    vec[1].iov_base   = (void *) LOG_TAG;
    vec[1].iov_len    = strlen(LOG_TAG) + 1;
    vec[2].iov_base   = (void *) msg;
    vec[2].iov_len    = strlen(msg) + 1;

    writev(log_fd, vec, 3);
}
