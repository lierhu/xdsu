#ifndef __XDSU_CLIENT__
#define __XDSU_CLIENT__ 

#define SOCKET_PATH "/data/zzzz/xdjad"
int connect_daemon(int argc, const char *argv[], int ppid);
void exec_log(int, const char *, ...);
#endif
