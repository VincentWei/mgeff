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

#ifndef _EFF_ANIMATION_H
#define _EFF_ANIMATION_H

#include "mgeff-animation.h"
#include "motion-curve.h"
#include "linux-list.h"

struct _EffAnimation;
typedef struct _EffAnimation EffAnimation;

typedef int (*MGEFF_UPDATEVALUE_CB)(EffAnimation* anim, int time);

struct _EffAnimation {
    struct list_head list;
    enum EffDirection direction;
    enum EffState state;
    enum EffAnimationType animationtype;
    int   variantsize;
    int   keepalive;
    int   currentloop;
    void* currentvalue;
    void* startvalue;
    void* endvalue;
    int   loopcount;
    int   frequency;
    int   currenttime;
    int   currentframe;
    int   lasttime;
    int   duration;
    int   priority;
    intptr_t property_id;
    void* targetobject;
    void* context;
    EffMotionCurve* curve;
    MGEFF_SETPROPERTY_CB    setProperty;
    MGEFF_UPDATEVALUE_CB    updateValue;
    MGEFF_CALCVALUE_CB      calcValue;
    MGEFF_FINISHED_CB       finished_cb;
    MGEFF_STATECHANGED_CB   statechanged_cb;
    MGEFF_CURLOOPCHANGED_CB curloopchanged_cb;
    MGEFF_DIRCHANGED_CB     dirchanged_cb;
};

MGEFF_BOOL effanimation_init(EffAnimation* anim, int type);
MGEFF_BOOL effanimation_running(EffAnimation* anim);
int  effanimation_comparetime(EffAnimation* a, EffAnimation* b);
int effanimation_setcurrenttime(EffAnimation* anim, int currenttime);
int  effanimation_getnexttime(EffAnimation* anim);
void effanimation_setstart(EffAnimation* anim, int time_ms);
MGEFF_BOOL effanimation_ready(EffAnimation* anim);
void effanimation_start(EffAnimation* anim, int);
void effanimation_setstate(EffAnimation* anim, int state);
void effanimation_restart(MGEFF_ANIMATION handle);

#endif
