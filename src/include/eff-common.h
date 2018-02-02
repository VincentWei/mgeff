/*
 *   This file is part of mGEff, a component for MiniGUI.
 * 
 *   Copyright (C) 2008~2018, Beijing FMSoft Technologies Co., Ltd.
 * 
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 * 
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 * 
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 *   Or,
 * 
 *   As this program is a library, any link to this program must follow
 *   GNU General Public License version 3 (GPLv3). If you cannot accept
 *   GPLv3, you need to be licensed from FMSoft.
 * 
 *   If you have got a commercial license of this program, please use it
 *   under the terms and conditions of the commercial license.
 * 
 *   For more information about the commercial license, please refer to
 *   <http://www.minigui.com/en/about/licensing-policy/>.
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
    DWORD GetTickCount(void);
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
#define CHECK_HANDLE_RET_INV(handle) do { if (handle == NULL) { assert(handle); return INV_PTR;} } while(0)
#define CHECK_HANDLE_RET_NIL(handle) do { if (handle == NULL) { assert(handle); return NULL;} } while(0)

#define _MGEFF_PRINTF _MG_PRINTF

#endif
