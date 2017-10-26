/*!============================================================================
 * \file eff-common.h 
 * \brief The head file include some common symbols. 
 * \author DongKai
 * \version 1.0
 * \date 02/03/2010 04:46:38 PM 
 *
 *  Company: Beijing Feynman Software Technology Co., Ltd.
 */

#ifndef _EFF_COMMON_H
#define _EFF_COMMON_H

#include <assert.h>
#ifndef WIN32
#   include <unistd.h>
#else
#   include "eff-win32.h"
#endif

static inline void eff_ms_sleep(int ms) {
#ifdef WIN32
    void __stdcall Sleep (unsigned long);
    Sleep (ms);
#else
    usleep(ms * 1000);
#endif
}

static inline int eff_time(void) {
#ifndef WIN32
    /* MiniGUI API */
    unsigned int GetTickCount(void);
    return GetTickCount() * 10;
#else /* WIN32 */
    return eff_time_win32();
#endif
}

#define IF_NOT_EQUAL_RETURN(EXPECTED, ACTUAL) \
    do { \
        assert(EXPECTED == ACTUAL); \
        if (EXPECTED != ACTUAL) return;} while(0)

#define IF_EQUAL_RETURN(EXPECTED, ACTUAL) \
    do { \
        assert(EXPECTED != ACTUAL); \
        if (EXPECTED == ACTUAL) return;} while(0)

#define IF_NOT_EQUAL_RETURN_VALUE(EXPECTED, ACTUAL, VALUE) \
    do { \
        assert(EXPECTED == ACTUAL); \
        if (EXPECTED != ACTUAL) return VALUE;} while(0)

#define IF_EQUAL_RETURN_VALUE(EXPECTED, ACTUAL, VALUE) \
    do { \
        assert(EXPECTED != ACTUAL); \
        if (EXPECTED == ACTUAL) return VALUE;} while(0)

#define IF_NULL_RETURN_VALUE(CONDITION, VALUE) IF_EQUAL_RETURN_VALUE(NULL, CONDITION, VALUE)
#define IF_NULL_RETURN(CONDITION) IF_EQUAL_RETURN(NULL, CONDITION)

#define CHECK_HANDLE(handle)     do { if (handle == NULL) { assert(handle); return;} } while (0)
#define CHECK_HANDLE_RET(handle) do { if (handle == NULL) { assert(handle); return -1;} } while(0)
#define CHECK_HANDLE_RET_NIL(handle) do { if (handle == NULL) { assert(handle); return NULL;} } while(0)

#ifdef DEBUG
#   define _MGEFF_PRINTF(fmt, ...) fprintf (stderr, fmt, ##__VA_ARGS__)
#else
#   define _MGEFF_PRINTF(fmt, ...) /* NULL */
#endif

#endif
