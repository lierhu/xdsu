#ifndef __XDSU_UTIL__
#define __XDSU_UTIL__
#define DEFAULT_SHELL "/system/bin/sh"

#ifndef AID_SHELL
#define AID_SHELL (get_shell_uid())
#endif

#ifndef AID_ROOT
#define AID_ROOT  0
#endif

#ifndef AID_SYSTEM
#define AID_SYSTEM (get_system_uid())
#endif

#ifndef AID_RADIO
#define AID_RADIO (get_radio_uid())
#endif

#include <stdbool.h>
bool root_access(pid_t ppid, int *conn_flag);

#endif
