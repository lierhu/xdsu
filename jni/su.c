/*
** Licensed under the Apache License, Version 2.0 (the "License"); 
** you may not use this file except in compliance with the License. 
** You may obtain a copy of the License at 
**
**     http://www.apache.org/licenses/LICENSE-2.0 
**
** Unless required by applicable law or agreed to in writing, software 
** distributed under the License is distributed on an "AS IS" BASIS, 
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
** See the License for the specific language governing permissions and 
** limitations under the License.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <limits.h>
#include <android/log.h>
#include "bignum.h"
#include "su_client.h"
#include "su_log.h"
#include "su_util.h"


#define N 89855713
#define A 82330933

//计算整数的比特位数
int bits_of_num(unsigned long num)
{
	int i = 0;
	while(num)
	{
		i++;
		num >>= 1;
	}
	return i;
}

void square_and_multiply(char *m, char *e, char *n, char *result)
{
	long long i_e, i_e_t;
	long long i;
	long long t_e, c_e;
	char str_z[100];
	char ret_t[100];
	char ret[100];
	char remainder[100];

	i_e_t = i_e = atoll(e);
	strcpy(str_z, "1");
	long long bit_num = bits_of_num(i_e_t);

	for(i = bit_num-1; i >=0; i--)
	{
	   t_e = 1 << i;
	   c_e = t_e & i_e;
	   if(c_e != 0)
	   {
		   multiply(str_z, str_z, ret_t);
		   multiply(ret_t, m, ret);
		   divide(ret, n, ret_t, remainder);
		   strcpy(str_z, remainder);
//		   z = (z*z*m)%n;
	   }
	   else
	   {
		   multiply(str_z, str_z, ret);
		   divide(ret, n, ret_t, remainder);
		   strcpy(str_z, remainder);
		   //z = (z*z)%n;
	   }
	}
	strcpy(result, str_z);
}
#if 0
           struct tm {
               int tm_sec;         /* seconds */
               int tm_min;         /* minutes */
               int tm_hour;        /* hours */
               int tm_mday;        /* day of the month */
               int tm_mon;         /* month */
               int tm_year;        /* year */
               int tm_wday;        /* day of the week */
               int tm_yday;        /* day in the year */
               int tm_isdst;       /* daylight saving time */
           };

       The members of the tm structure are:

       tm_sec    The number of seconds after the minute, normally in the range 0 to 59, but can be up to 60 to allow for leap seconds.

       tm_min    The number of minutes after the hour, in the range 0 to 59.

       tm_hour   The number of hours past midnight, in the range 0 to 23.

       tm_mday   The day of the month, in the range 1 to 31.

       tm_mon    The number of months since January, in the range 0 to 11.

       tm_year   The number of years since 1900.

       tm_wday   The number of days since Sunday, in the range 0 to 6.

       tm_yday   The number of days since January 1, in the range 0 to 365.

       tm_isdst  A flag that indicates whether daylight saving time is in effect at the time described.  The value is positive if daylight
                 saving time is in effect, zero if it is not, and negative if the information is not available.

#endif
void getday( char *today)
{
    time_t now;
    time( &now );
    struct tm * t = localtime( &now );
    sprintf(today,"%d%02d%02d",t->tm_year + 1900, t->tm_mon + 1, t->tm_mday);
}

int check_passwd( const char *passwd )
{
    char a[100] = {0};
    char r[100] = {0};
    char n[100] = {0};
    char time[10] = {0};
    sprintf(a,"%d", A);
    sprintf(n,"%d", N);

    square_and_multiply(passwd, a, n, r);

    getday(time);

    if(strcmp(r, time) == 0)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

static void version()
{
#define __SU_VERSION__ 0x2
#define __COMPILE_TIME__ 20141021

    printf("version: %x (%d)\n",__SU_VERSION__, __COMPILE_TIME__);
}

static int executionFailure(char *context)
{
	fprintf(stderr, "su: %s. Error:%s\n", context, strerror(errno));
	return -errno;
}

static int permissionDenied()
{
	printf("su: su permission denied\n");
	return 1;
}

int main(int argc, const char *argv[])
{
	struct stat stats;
	struct passwd *pw;
	FILE *file;
	int conn_flag = 0, tmp = -1;
	char buff[128] = {0}, contextbuf[128] = {0};

	int uid = 0;
	int gid = 0;

	int ppid = getppid();
    int n;
    for (n = 0; n < argc; n++)
    {
        LOGD("argv[%d]:%s\n",n,argv[n]);
    }

    if(argc < 2)
    {
        LOGE("number of parameter must greater than 1");
        permissionDenied();
        return 0;
    }

    if ( argc == 2 && (strcmp( argv[1], "-v") == 0 || strcmp(argv[1], "-V") == 0) )
    {
        version();
        return 0;
    }

	LOGD("begin request su  ppid is  %d\n", ppid );
    if (!root_access(ppid, &conn_flag))
    {
		permissionDenied();
        LOGE("can not grant root permission");
        exit (-1);
    }
#if 0
	sprintf( buff, "/proc/%d/cmdline", ppid );
	LOGD("buff = %s\n", buff );
	file = fopen( buff, "r" );
	if( file != NULL )
	{
		LOGD("file is not null" );
		if( fgets( contextbuf, 258, file ) != NULL )
		{
			LOGD("the contextbuf is %s-----\n", contextbuf );
			if( strcmp(contextbuf, "com.xdja.mdmc") == 0 ||
			    strcmp(contextbuf, "com.xdja.seas") == 0 ||
			    strcmp(contextbuf, "xdja_app") == 0)
			{
				flag = 1;
			}
		}
	}
#endif

	LOGD("the flag is %d", conn_flag );

    if(conn_flag == 1)
    {
        return connect_daemon(argc, argv, ppid);
    }

	if( conn_flag == 0 )
	{
		if( check_passwd( argv[1] ) != 0 )
		{
			LOGD("wrong passwd!!! -- %s", argv[1] );
			permissionDenied();
			return 0;
		}
	}

	if(setgid(gid))
	{
		LOGD("setgid failed !!" );
		executionFailure("setgid");
		return 0;
	}

	if(setuid(uid))
	{
		LOGD("setuid failed !!" );
		executionFailure("setgid");
		return 0;
	}

	char *exec_args[argc];
	
	if( argc > 1 )
	{
		exec_args[argc-1] = NULL;
	}
	exec_args[0] = "sh";
	int i;
	LOGD("the argv[0] is %s", argv[0] );

	int j = 0;
	for( j = 0; j< argc; j++ )
	{
		LOGD("the argv[%d] = %s \n",j, argv[j] );
	}

	LOGD("the exec_args[0] is %s \n", exec_args[0] );
	for (i = 1; i < (argc-1); i++)
	{
		exec_args[i] = argv[i+1];
		LOGD("the exec_args is %s\n", exec_args[i] );
	}
	execv("/system/bin/sh", exec_args);
	return executionFailure("sh");
}

