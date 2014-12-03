#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "su_util.h"
#include "su_log.h"

static char white_list[][256] = {
    "com.xdja.mdmc",
    "com.xdja.seas",
    "xdja_app",
};

static int readline(int fd, char *str, int n)
{
    if(str == NULL)
    {
        return -1;
    }

    char buf;
    int i = 0;
    while(read(fd, &buf, 1) > 0)
    {
        str[i++] = buf;
        if (buf == '\n' || i >= n)
        {
            break;
        }
    }

    return i;
}

bool root_access(pid_t ppid, int *conn_flag)
{
    int fd;
    char filename[1024] = {'\0'};
    char cmdline[1024] = {'\0'};

    sprintf(filename, "/proc/%d/cmdline", ppid);

    fd = open(filename, O_RDONLY);
    if (-1 == fd)
    {
        PLOGE("unable to open %s", filename);
        return false;
    }

    if ( -1 == readline(fd, cmdline, sizeof(cmdline)-1) )
    {
        LOGE("can not read %s", cmdline);
        close(fd);
        return false;
    }

    if (strcmp(DEFAULT_SHELL, cmdline) == 0)
    {
        LOGD("===>>> it is started from shell.");
        *conn_flag = 0;
        return true;
    }
    else
    {
        int n = sizeof(white_list) / sizeof(white_list[0]);
        int i;
        for (i = 0; i < n; i++)
        {
            if (strcmp(cmdline, white_list[i]) == 0)
            {
                *conn_flag = 1;
                return true;
            }
        }
    }

    return false;
}

