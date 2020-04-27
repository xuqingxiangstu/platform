/*
 *
 * os.h - A brief description goes here.
 *
 */

#ifndef _HEAD_OS_03192BC4_17DC8D5E_2207A466_H
#define _HEAD_OS_03192BC4_17DC8D5E_2207A466_H

#define LCH_OS_HEADER

#if defined(__linux) || defined(__linux__)
#define OS_LINUX
#if defined(__linux__)
#define OS_LINUX_ELF
#endif
#endif

#if defined(_WIN32) || defined(WIN32)
#define OS_WINDOWS
#if defined(__BORLANDC__) || defined(__BCPLUSPLUS__)
#define OS_WINDOWS_BC
#else
#define OS_WINDOWS_VC
#endif
#endif

#if defined(__hpux)
#define OS_HP
#endif

#if defined(__sun)
#define OS_SUN
#endif

#if !defined(OS_LINUX) && !defined(OS_WINDOWS) && !defined(OS_HP) && !defined(OS_SUN)
#error Unknown operating system.
#endif

#if !defined(INT_MAX)
#define	INT_MAX		2147483647
#endif
#if !defined(UINT_MAX)
#define	UINT_MAX	4294967295U
#endif

#if !defined(OS_WINDOWS)

#define AFX_EXT_CLASS
#define DLL_EXP_API
#else
#if defined(OS_WINDOWS_BC)
#define AFX_EXT_CLASS	__declspec(dllexport)
#endif

#if defined(_USRDLL)
#define DLL_EXP_API __declspec(dllexport)
#else
#ifdef _LIB
#define DLL_EXP_API
#else
#define DLL_EXP_API
#endif
#endif



#endif /* #if !defined(OS_WINDOWS) */


#if !defined(OS_WINDOWS)
typedef unsigned char * LPCSTR;
typedef unsigned int    DWORD;
typedef unsigned int    UINT;
typedef unsigned int    BOOL;
typedef unsigned short  WORD;
typedef unsigned short  SHORT;
typedef unsigned char   BYTE;
typedef unsigned char   UCHAR;
typedef void 		VOID;
typedef char            TCHAR;
typedef void *          LPVOID;
typedef void * (*LPTHREAD_START_ROUTINE)(void *);
#else
typedef int             key_t;
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include "windows.h"

#include "winsock2.h"
#endif /* #if !defined(OS_WINDOWS) */

#if !defined(OS_WINDOWS)
#define DEF_CDECL_FUNPOINT
#else
#define DEF_CDECL_FUNPOINT __cdecl
#endif

#if !defined(SLEEP_S)
#if defined(OS_WINDOWS)
#define SLEEP_S(x)	Sleep((x) * 1000)
#define SLEEP_MS(x)	Sleep(x)
#define SLEEP_US(x)	Sleep((x) / 1000)
#else
#define SLEEP_S(x)	sleep(x)
#if defined(DRIVE_USLEEP)
#define SLEEP_MS(x)	usleep((x) * 1000)
#define SLEEP_US(x)	usleep(x)
#else
#define SLEEP_MS(x) {\
    struct timespec nReplaceuSleeptv;\
	nReplaceuSleeptv.tv_sec = 0;\
	nReplaceuSleeptv.tv_nsec = x * 1000000;\
	pselect(0, NULL, NULL, NULL, &nReplaceuSleeptv,NULL);\
}
#define SLEEP_US(x) {\
    struct timespec nReplaceuSleeptv;\
	nReplaceuSleeptv.tv_sec = 0;\
	nReplaceuSleeptv.tv_nsec = x * 1000;\
	pselect(0, NULL, NULL, NULL, &nReplaceuSleeptv,NULL);\
}
#endif
#endif
#endif /* #if !defined(SLEEP_S) */

#if defined(OS_WINDOWS)
#if defined(OS_WINDOWS_VC)
#define STRCASECMP(caseSensitive, A, B) ((caseSensitive) ? strcmp((A), (B)) : _stricmp((A), (B)))
#else
#define STRCASECMP(caseSensitive, A, B) ((caseSensitive) ? strcmp((A), (B)) : stricmp((A), (B)))
#endif
#else
#define STRCASECMP(caseSensitive, A, B) ((caseSensitive) ? strcmp((A), (B)) : strcasecmp((A), (B)))
#endif

#if defined(OS_WINDOWS) && !defined(IPC_CREAT)
#define IPC_CREAT	01000
#define IPC_EXCL	02000
#define IPC_NOWAIT	04000
#endif /* #if defined(OS_WINDOWS) && !defined(IPC_CREAT) */

#if !defined(MAX_PATH)
#if defined(OS_LINUX)
#include <linux/limits.h>
#endif
#define MAX_PATH	PATH_MAX
#endif

#if !defined(OS_WINDOWS)
#define FILENAME_SEPERATOR	'/'
#else
#define FILENAME_SEPERATOR	'\\'
#endif

#if !defined(TRUE)
#define TRUE	1
#endif

#if !defined(FALSE)
#define FALSE	0
#endif

#if !defined(OS_WINDOWS)
typedef int SOCKET;
#endif

#if !defined(CLOSESOCKET)
#if defined(OS_WINDOWS)
#define CLOSESOCKET	closesocket
#else
#define CLOSESOCKET	close
#endif
#endif /* #if !defined(CLOSESOCKET) */

#if !defined(INVALID_SOCKET)
#define INVALID_SOCKET	-1
#endif

#if !defined(SOCKET_ERROR)
#define SOCKET_ERROR	-1
#endif

#define MIN_SOCKET_PORT  7000
#define MAX_SOCKET_PORT  8000

#if !defined(INVALID_HANDLE_VALUE)
#define INVALID_HANDLE_VALUE	-1
#endif

#if !defined(INVALID_TAG)
#define INVALID_TAG	255
#define INVALID_SUBTAG	255
#define INVALID_DEVID	65535
#define INVALID_PCM	255
#define INVALID_TS	32
#endif

#if defined(OS_LINUX)
#if !defined(__tile__)
#define gettext_noop(str) (str)
#define _(str)            gettext(str)
#define N_(str)           gettext_noop(str)
#else
#define gettext_noop(str) (str)
#define _(str)            (str)
#define N_(str)           (str)
#endif
#else
#define gettext_noop(str) (str)
#define _(str)            (str)
#define N_(str)           (str)
#endif

#if defined(_FILE_OFFSET_BITS)
#define FTELL ftello
#define FSEEK fseeko
#else
#define FTELL ftell
#define FSEEK fseek
#endif

#if defined(__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)
	/* union semun is defined by including <sys/sem.h> */
#else
	/* according to X/OPEN we have to define it ourselves */
	union semun {
		int                 val;
		struct semid_ds    *buf;
		unsigned short int *array;
		struct seminfo     *__buf;
	};
#endif

#endif /* #ifndef _HEAD_OS_03192BC4_17DC8D5E_2207A466_H */
