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
 *   <http://www.minigui.com/en/about/licensing-policy/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#ifdef _MGEFF_THREADMODE
#   error
#   include <pthread.h>
#endif

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

#include "mgeff.h"
#include "eff-common.h"
#include "schedular.h"
#include "animation.h"
#ifdef _MGEFF_EFFECTOR
#include "effector.h"
#endif
#include "garbage-collection.h"

#define MGEFF_TIMER_ID 0x76540000

static int s_serial;
#ifdef _MGEFF_THREADMODE
static pthread_mutex_t s_mutex;
static pthread_cond_t s_cond;
static pthread_key_t s_threadinfo_key;
#endif

/******************************************
 * Internal implementations
 */

int effschedular_one_step(EffSchedular *schedular, EffAnimation *animation_wanted) {
    // TODO: optimization
    struct list_head *i;
    struct list_head *n;
    int now_ms;
    int found = 0;
    int nActive=0;
    now_ms = eff_time();
    //list_for_each(i, &schedular->animation_list) {
    list_for_each_safe(i, n, &schedular->animation_list) {
        EffAnimation *animation = (EffAnimation *)i;
        if (animation_wanted && !found && animation_wanted == animation) {
            found = 1;
        }
        switch (animation->state) {
            case MGEFF_STATE_RUNNING:
                effanimation_setcurrenttime(animation, now_ms);
                ++nActive;
                break;
            case MGEFF_STATE_READY:
                effanimation_setstart(animation, now_ms);
                effanimation_setcurrenttime(animation, now_ms);
                ++nActive;
                break;
            case MGEFF_STATE_PAUSED:
                ++nActive;
                break;
            case MGEFF_STATE_STOPPED: /* remove from the list, but not delete */
                {
                    struct list_head *j = i->prev;
                    list_del(&animation->list);
                    i = j;
                }
                break;
            case MGEFF_STATE_REMOVE: /* remove and delete */
                {
                    struct list_head *j = i->prev;
                    list_del(&animation->list);
                    i = j;
                    mGEffAnimationDelete((MGEFF_ANIMATION)animation);
                }
                break;
            default:
                assert(0);
                break;
        }
    }
    return found ? 1 : -1;
}

void effschedular_sync_run(EffSchedular *schedular, EffAnimation *animation) {
    int ret;
    for (;;) {
        ret = effschedular_one_step(schedular, animation);
        if (ret > 0) {
            eff_ms_sleep(0); /* XXX: */
            continue;
        } else if (ret == 0) {
            eff_ms_sleep(10);
        } else {
            break;
        }
    }
}

#ifdef _MGEFF_THREADMODE
static void *eff_schedular_thread(void *_schedular) {
    EffSchedular *schedular;
    pthread_mutex_lock(&s_mutex);
    schedular = (EffSchedular *)_schedular;
    pthread_cond_signal(&s_cond);
    pthread_mutex_unlock(&s_mutex);
    effschedular_run_blocked(schedular);
    return NULL;
}

int effschedular_run_in_thread(EffSchedular *schedular) {
    pthread_t th;
    pthread_mutex_lock(&s_mutex);
    pthread_create(&th, NULL, eff_schedular_thread, schedular);
    pthread_cond_wait(&s_cond, &s_mutex);
    pthread_mutex_unlock(&s_mutex);
    return 0;
}

static inline MGEFF_BOOL effschedular_create_threadkey (void)
{
    if (pthread_key_create (&s_threadinfo_key, NULL))
        return MGEFF_FALSE;
    return MGEFF_TRUE;
}

static inline void effschedular_delete_threadkey (void)
{
    pthread_key_delete (s_threadinfo_key);
}
#endif

#ifndef _MG_MINIMALGDI 
static MGEFF_BOOL effschedular_ontimer(HWND hwnd, LINT id, DWORD tickcount) {
    EffSchedular *schedular = (EffSchedular *)hwnd;
    effschedular_one_step(schedular, NULL);
    return MGEFF_TRUE;
}
#endif

int effschedular_setup_timer(EffSchedular *schedular) 
{
#ifndef _MG_MINIMALGDI 
    if (schedular->status == 0) {
        if (SetTimerEx((HWND)schedular, (MGEFF_TIMER_ID | schedular->id), 2, effschedular_ontimer)) {
            schedular->status = 1;
            return 0;
        } else {
            assert(0);
            return -1;
        }
    }else{
        return 0;
    }
#else
    return 0;
#endif
}

int effschedular_kill_timer(EffSchedular *schedular) 
{
#ifndef _MG_MINIMALGDI 
    return KillTimer((HWND)schedular, (MGEFF_TIMER_ID | schedular->id));
#endif
}

/******************************************
 * Internal interfaces
 */
void effschedular_add_animation(EffSchedular *schedular, EffAnimation *animation) {
    struct list_head *i;
    list_for_each(i, &schedular->animation_list) {
        if ((EffAnimation *)i == animation) {
            return;
        }
    }
    list_add_tail(&animation->list, &schedular->animation_list);
}

void effschedular_remove_animation(EffSchedular *schedular, EffAnimation *animation) {
    struct list_head *i;
    list_for_each(i, &schedular->animation_list) {
        if ((EffAnimation *)i == animation) {
            //list_del(&animation->list);
            list_del(i);
            break;
        }
    }
}

/******************************************
 * Exported interfaces
 */
EffSchedular *g_schedular_default = NULL;
static struct list_head schedular_list;
EffSchedular* effschedular_check_sch(void)
{
#ifdef _MGEFF_THREADMODE
    EffSchedular* schedular;
    if ((schedular = pthread_getspecific (s_threadinfo_key)) == NULL) {
        schedular = effschedular_create();
        pthread_setspecific (s_threadinfo_key, schedular);
    }
    return schedular;
#else
    return g_schedular_default;
#endif
}

static int m_initRefrent = 0;

int mGEffInit (void) 
{
    m_initRefrent++;
    if (! g_schedular_default) {
        EffGCInit();
#ifdef _MGEFF_THREADMODE
        pthread_mutex_init(&s_mutex, NULL);
        pthread_cond_init(&s_cond, NULL);
        effschedular_create_threadkey();
#endif
        INIT_LIST_HEAD(&schedular_list);
        g_schedular_default = (EffSchedular *)effschedular_create();
        /* start global timer schedular.*/
        effschedular_setup_timer(g_schedular_default);
        /* init and register default effector.*/
#ifdef _MGEFF_EFFECTOR
        effeffector_init();
#endif
    }
    return 0;
}

void mGEffDeinit (void) 
{
    if (--m_initRefrent > 0) {
        return;
    }

    if (g_schedular_default) {
#ifdef _MGEFF_THREADMODE
        pthread_mutex_destroy(&s_mutex);
        pthread_cond_destroy(&s_cond);
#endif
        effschedular_kill_timer(g_schedular_default);
#ifdef _MGEFF_EFFECTOR
        effeffector_deinit();
#endif
        /* destroy all create schedular.*/
        {
            EffSchedular *child;
            struct list_head *i, *j;
            list_for_each(i, &schedular_list) {
                j = i->prev;
                child = (EffSchedular *)i;
                effschedular_destroy(child);
                i = j;
            }
        }
#ifdef _MGEFF_THREADMODE
        effschedular_delete_threadkey();
#endif
        g_schedular_default = NULL;
        EffGCCheckPoint();
    }
}

EffSchedular* effschedular_create(void) 
{
    EffSchedular *sch;
    sch = (EffSchedular *)malloc(sizeof(*sch));
    memset(sch, 0, sizeof(*sch));
    INIT_LIST_HEAD(&sch->animation_list);
    sch->id = s_serial++;
    EffGCAddSchedular(sch);

    list_add_tail(&sch->list, (&schedular_list));

    return sch;
}

void effschedular_destroy(EffSchedular* schedular) 
{
    EffGCRemoveSchedular(schedular);
    assert(schedular);

    /* remove from schedular_list. */
    {
        struct list_head *i;
        list_for_each(i, &schedular_list) {
            if (schedular == (EffSchedular *)i) {
                list_del(i);
                break;
            }
        }
    }

    // TODO: kill timer, destroy thread, ...
    free(schedular);
}
