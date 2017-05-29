/*! ============================================================================
 * \file mgeff-animationgroup.c 
 * \brief  
 * \author DongKai
 * \version 1.0
 * \date 02/05/2010
 *
 *  Company: Beijing Feynman Software Technology Co., Ltd.
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
            if (effanimation_ready(animation)) {
                effanimation_setstart(animation, eff_time());
            }
            if (effanimation_running(animation)) {
                effanimation_setcurrenttime(animation, time);
                if (effanimation_running(animation)) {
                    find_running_one = 1;
                }
            }
        }
        if (!find_running_one) {
            if (anim->loopcount > 0) 
                anim->loopcount--;
            if (anim->loopcount == 0) {
                effanimation_setstate(anim, MGEFF_STATE_STOPPED);
            }
            else{
                list_for_each(i, &group->animation_list) {
                    animation = (EffAnimation *)i;
                    effanimation_restart((MGEFF_ANIMATION)animation);
                    effanimation_setstate(animation, MGEFF_STATE_RUNNING);
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
            if (effanimation_ready(animation)) {
                effanimation_setstart(animation, eff_time());
            }
            if (effanimation_running(animation)) {
                find_running_one = 1;   
                break;
            }
        }
        if (find_running_one) {
            effanimation_setcurrenttime(animation, time);
        }
        else {
            if (anim->loopcount > 0)
                anim->loopcount--;

            if (anim->loopcount == 0) {
                effanimation_setstate(anim, MGEFF_STATE_STOPPED);
            }
            else {
                list_for_each(i, &group->animation_list) {
                    animation = (EffAnimation *)i;
                    effanimation_restart((MGEFF_ANIMATION)animation);
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

    effanimation_init((EffAnimation*)group, -1);
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

    effanimation_start(a, 1);
}

