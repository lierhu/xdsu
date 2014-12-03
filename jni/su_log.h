#ifndef __XDSU_LOG__
#define __XDSU_LOG__

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "XDSU"

#include <android/log.h>

void exec_log(int priority, const char* fmt, ...);

#ifndef LOG_NDEBUG
#define LOG_NDEBUG 1
#endif

#ifndef LOGE
#define LOGE(fmt,args...) exec_log(ANDROID_LOG_ERROR, fmt, ##args)
#endif
#ifndef LOGW
#define LOGW(fmt,args...) exec_log(ANDROID_LOG_WARN, fmt, ##args)
#endif
#ifndef LOGD
#define LOGD(fmt,args...) exec_log(ANDROID_LOG_DEBUG, fmt, ##args)
#endif
#ifndef LOGV
#if LOG_NDEBUG
#define LOGV(...)   ((void)0)
#else
#define LOGV(fmt,args...) exec_log(ANDROID_LOG_VERBOSE, fmt, ##args)
#endif
#endif

#if 0
#undef LOGE
#define LOGE(fmt,args...) fprintf(stderr, fmt, ##args)
#undef LOGD
#define LOGD(fmt,args...) fprintf(stderr, fmt, ##args)
#undef LOGW
#define LOGW(fmt,args...) fprintf(stderr, fmt, ##args)
#endif

#include <errno.h>
#include <string.h>
#define PLOGE(fmt,args...) LOGE(fmt " failed with %d: %s", ##args, errno, strerror(errno))
#define PLOGEV(fmt,err,args...) LOGE(fmt " failed with %d: %s", ##args, err, strerror(err))
#endif
