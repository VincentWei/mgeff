///////////////////////////////////////////////////////////////////////////////
//
//                          IMPORTANT NOTICE
//
// The following open source license statement does not apply to any
// entity in the Exception List published by FMSoft.
//
// For more information, please visit:
//
// https://www.fmsoft.cn/exception-list
//
//////////////////////////////////////////////////////////////////////////////
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

#include <stdio.h>
#include <stdlib.h>
#include "mgeff.h"
#include "animation.h"
#include "animation-group.h"
#include "eff-common.h"
#include "garbage-collection.h"
 
static int effanimgroup_parallel_update(EffAnimation* anim, int time)
{
    if (NULL != anim) {
        EffAnimationGroup* group = (EffAnimationGroup*)anim;
        EffAnimation *animation;
        struct list_head *i;
        int find_running_one = 0;

        list_for_each(i, &group->animation_list) {
            animation = (EffAnimation *)i;
            if (__mgeffanimation_ready(animation)) {
                __mgeffanimation_setstart(animation, eff_time());
            }
            if (__mgeffanimation_running(animation)) {
                __mgeffanimation_setcurrenttime(animation, time);
                if (__mgeffanimation_running(animation)) {
                    find_running_one = 1;
                }
            }
        }
        if (!find_running_one) {
            if (anim->loopcount > 0) 
                anim->loopcount--;
            if (anim->loopcount == 0) {
                __mgeffanimation_setstate(anim, MGEFF_STATE_STOPPED);
            }
            else{
                list_for_each(i, &group->animation_list) {
                    animation = (EffAnimation *)i;
                    __mgeffanimation_restart((MGEFF_ANIMATION)animation);
                    __mgeffanimation_setstate(animation, MGEFF_STATE_RUNNING);
                }
            }
        }
        return 0;
    }
    return -1;
}

static int effanimgroup_sequential_update(EffAnimation* anim, int time)
{
    if (NULL != anim) {
        EffAnimationGroup* group = (EffAnimationGroup*)anim;
        EffAnimation *animation;
        struct list_head *i;
        int find_running_one = 0;

        list_for_each(i, &group->animation_list) {
            animation = (EffAnimation *)i;
            /* should be start next animation at this moment.*/
            if (__mgeffanimation_ready(animation)) {
                __mgeffanimation_setstart(animation, eff_time());
            }
            if (__mgeffanimation_running(animation)) {
                find_running_one = 1;   
                break;
            }
        }
        if (find_running_one) {
            __mgeffanimation_setcurrenttime(animation, time);
        }
        else {
            if (anim->loopcount > 0)
                anim->loopcount--;

            if (anim->loopcount == 0) {
                __mgeffanimation_setstate(anim, MGEFF_STATE_STOPPED);
            }
            else {
                list_for_each(i, &group->animation_list) {
                    animation = (EffAnimation *)i;
                    __mgeffanimation_restart((MGEFF_ANIMATION)animation);
                }
            }
        }
        return 0;
    }
    return -1;
}   

MGEFF_ANIMATION mGEffAnimationCreateGroup(enum EffAnimationType type)
{
    EffAnimationGroup* group = (EffAnimationGroup*) calloc(1, sizeof(EffAnimationGroup));

    group->animation.keepalive = 1;

    __mgeffanimation_init((EffAnimation*)group, -1);
    INIT_LIST_HEAD(&group->animation_list);
    group->animation.animationtype = type;
    if (type == MGEFF_PARALLEL) {
        group->animation.updateValue = effanimgroup_parallel_update;
    }
    else {
        group->animation.updateValue = effanimgroup_sequential_update;
    }

    EffGCAddAnimation((EffAnimation *)group);
    return (MGEFF_ANIMATION)group;
}

void mGEffAnimationAddToGroup(MGEFF_ANIMATION handle, MGEFF_ANIMATION anim)
{
    EffAnimationGroup* group = (EffAnimationGroup*) handle;
    EffAnimation* a= (EffAnimation*)anim;     
    CHECK_HANDLE(handle);
    CHECK_HANDLE(anim);

    list_add_tail(&a->list, (&group->animation_list));

    __mgeffanimation_start(a, 1);
}

