/**
 * 郑州信大捷安linux平台标准引入头文件
 *
 * Copyright (c) 2012 XDJA Inc.
 *
 * @file xj_linux_common.h
 * @brief 1, 包含linux平台下经常会使用到的头文件 2, 常用宏定义 3, 常规数据类型定义
 * @author niujiuru <njr@xdja.com>
 * @date 2012/05/07
 */

#ifndef __XDJA_LINUX_COMMON_H__
#define __XDJA_LINUX_COMMON_H__

// 1, check linux platform
#if !defined linux && !defined __linux && !defined __linux__
#error sorry, this file can be included only on "linux" platform!!!
#endif

// 2, c language common header files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <time.h>
#include <math.h>
#include <locale.h>
#include <assert.h>

// 3, include "linux" common header files
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include <inttypes.h>
#include <unistd.h>
#include <signal.h>
#include <wchar.h>
#include <fcntl.h>
#include <getopt.h>
#include <termios.h>
#include <netdb.h>
#include <stdint.h>
#include <dirent.h>
#include <arpa/inet.h>

#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>

#include <net/if.h>
#include <net/if_arp.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/timeb.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/param.h>
#include <sys/reboot.h>

// 4, define "linux" common macros
#ifndef max
#define max(a, b) (a > b ? a : b)
#endif

#ifndef min
#define min(a, b) (a < b ? a : b)
#endif

#define MAKEWORD(a, b)      ((uint16_t)(((uint8_t)(a)) | ((uint16_t)((uint8_t)(b))) << 8))
#define MAKELONG(a, b)      ((uint32_t)(((uint16_t)(a)) | ((uint32_t)((uint16_t)(b))) << 16))
#define LOWORD(l)           ((uint16_t)(l))
#define HIWORD(l)           ((uint16_t)(((uint32_t)(l) >> 16) & 0xFFFF))
#define LOBYTE(w)           ((uint8_t)(w))
#define HIBYTE(w)           ((uint8_t)(((uint16_t)(w) >> 8) & 0xFF))

#ifndef MAX_PATHLEN
#define MAX_PATHLEN 260
#endif

#ifndef MAX_LINELEN
#define MAX_LINELEN 128
#endif

#ifndef MAXCHARS_LOGLINE
#define MAXCHARS_LOGLINE 512
#endif

#ifndef INFINITE
#define INFINITE 0xFFFFFFFF
#endif

#ifndef WAIT_FOREVER
#define WAIT_FOREVER INFINITE
#endif

// IP address is the multicast address?
#define IS_MULTICAST_IP(ip) (0xE0<=(ip&0xFF) && (ip&0xFF)<0xF0)

// 5, define "linux" common data type
#ifndef __cplusplus
#ifndef bool
#define bool unsigned char
#endif

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif
#endif

#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void*)0)
#endif
#endif

#ifndef null
#define null NULL
#endif

#ifndef handle
#define handle void*
#endif

#endif /* __XDJA_LINUX_COMMON_H__ */
