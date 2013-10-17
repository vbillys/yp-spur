/* high level I/O */
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <strings.h>
#include <pthread.h>
#include <stdarg.h>

/* low level I/O */
#include <fcntl.h>
#include <sys/time.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <param.h>
#include <utility.h>
#include <yprintf.h>

#ifdef __WIN32
#include <windows.h>
#endif

/* get time stamp */
double get_time( void )
{
	struct timeval current;

	gettimeofday( &current, NULL );

	return current.tv_sec + current.tv_usec / 1000000.0;
}

void yp_usleep( int usec )
{
#if defined(HAVE_NANOSLEEP)
	// nanosleepが利用可能
	struct timespec request;
	request.tv_sec = usec / 1000000;
	request.tv_nsec = ( usec - request.tv_sec * 1000000 ) * 1000;

	nanosleep( &request, NULL );
#elif defined(__MINGW32__)
	// MinGWのusleepには1ms以下切り捨ての問題があるためWindows環境ではWinAPIのSleepを使う
	// 1ms以下は切り上げ
	Sleep( ( usec + 999 ) / 1000 );
#else
	// 古いシステム用
	usleep( usec );
#endif
}

void hook_pre_global()
{
	// Windows環境で標準出力がバッファリングされないように設定
	setvbuf( stdout, 0, _IONBF, 0 );
	setvbuf( stderr, 0, _IONBF, 0 );
	
#if defined(__MINGW32__)
#endif
}


#if !defined(HAVE_STRTOK_R)

/* 
 * public domain strtok_r() by Charlie Gordon
 *   from comp.lang.c  9/14/2007
 *      http://groups.google.com/group/comp.lang.c/msg/2ab1ecbb86646684
 *     (Declaration that it's public domain):
 *      http://groups.google.com/group/comp.lang.c/msg/7c7b39328fefab9c
 */

char* strtok_r( char *str, const char *delim, char **nextp )
{
    char *ret;

    if( str == NULL ) str = *nextp;

    str += strspn(str, delim);

    if( *str == '\0' ) return NULL;

    ret = str;
    str += strcspn( str, delim );

    if( *str ) *str++ = '\0';

    *nextp = str;

    return ret;
}

#endif
