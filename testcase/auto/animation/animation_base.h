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
 *   <http://www.minigui.com/blog/minigui-licensing-policy/>.
 */
#include <assert.h>
#include <sys/time.h>
#include <time.h> 

#include <mgeff/mgeff.h>

#include <gtest/gtest.h>

#ifndef _ANIMATION_BASE_H
#define _ANIMATION_BASE_H

#define TIMER_ID 1100

#define PAUSERESUME_TIMER_ID 1110
#define SETDIRECTION_TIMER_ID 1120
#define STOP_TIMER_ID 1130

#define MAX_TIME 80

typedef struct _ANIMAPARAM {
    int loopcount;
    int duration;
    int fps;
    enum EffDirection direction; 
    void *startval;
    void *endval;
}ANIMAPARAM; 

typedef struct _TESTPRJ
{
    struct {
        int loopcount;
        int fcount;
        int reserve;
        bool bfinished;
        void *startval;
        void *endval;

        bool bstopflag;
        bool bpauseflag;

        bool bvarmalloc;
        bool bcalcvalue;
        bool bsetproperty;
        bool bdirchanged;
        bool bcurloopchanged;
        bool bstatechanged;
    }val;
    struct {
        timeval starttime;
        timeval endtime;
        timeval stoptime;
        timeval finishtime;
        timeval pausetime;
        timeval resumetime;
    }times;
}TESTPRJ;

void finished_callback (MGEFF_ANIMATION handle);
void *varmalloc_callback (MGEFF_ANIMATION handle);
void calcvalue_callback (MGEFF_ANIMATION handle, void *s, void *e, void *v, float factor);
void calcloopcount_callback (MGEFF_ANIMATION handle);
void directionchanged_callback(MGEFF_ANIMATION handle);

void animation_callback (MGEFF_ANIMATION handle, void *target, int id, void *value);

MGEFF_ANIMATION create_animation (void *target,
        ANIMAPARAM *anima_param, enum EffVariantType varianttype);
MGEFF_ANIMATION create_animation (void *target, 
        ANIMAPARAM *anima_param, enum EffVariantType varianttype,
        MGEFF_SETPROPERTY_CB setproperty_callback);

MGEFF_ANIMATION createex_animation (void *target,
        ANIMAPARAM *anima_param, enum EffVariantType varianttype, int size,
        MGEFF_VARMALLOC_CB varmalloc, MGEFF_CALCVALUE_CB calcvalue,
        MGEFF_SETPROPERTY_CB setproperty_callback);

MGEFF_ANIMATION createex_animation (void *target,
        ANIMAPARAM *anima_param, enum EffVariantType varianttype, int size,
        MGEFF_VARMALLOC_CB varmalloc, MGEFF_CALCVALUE_CB calcvalue);

#endif /*_ANIMATION_BASE_H*/

