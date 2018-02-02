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
#include <string.h>
#include <stdlib.h>
#ifdef _MGEFF_THREADMODE
#include <pthread.h>
#endif
#include "mgeff.h"

#include "garbage-collection.h"

typedef struct _gc_item {
    struct list_head list;
    void *object;
}gc_item_t;

struct list_head animation_list;
struct list_head schedular_list;

#ifdef _MGEFF_THREADMODE
static pthread_mutex_t s_mutex;
#define LOCK_INIT()    pthread_mutex_init(&s_mutex, NULL)
#define LOCK_DESTROY() pthread_mutex_destroy(&s_mutex)
#define LOCK()         pthread_mutex_lock(&s_mutex)
#define UNLOCK()       pthread_mutex_unlock(&s_mutex)
#else
#define LOCK_INIT()
#define LOCK_DESTROY()
#define LOCK()
#define UNLOCK()
#endif

int EffGCInit() {
    LOCK_INIT();
    INIT_LIST_HEAD(&animation_list);
    INIT_LIST_HEAD(&schedular_list);
    return 0;
}

void EffGCDeinit() {
    LOCK_DESTROY();
}

static int gc_add_object(void *object, struct list_head *head) {
    struct list_head *i;
    gc_item_t *item;
    list_for_each(i, head) {
        item = (gc_item_t *)i;
        if (item->object == object) {
            return -1;
        }
    }

    item = (gc_item_t *)calloc(1, sizeof(gc_item_t));
    item->object = object;
    list_add_tail(&item->list, head);
    return 0;
}

static int gc_remove_object(void *object, struct list_head *head) {
    struct list_head *i;
    list_for_each(i, head) {
        gc_item_t *item = (gc_item_t *)i;
        if (item->object == object) {
            list_del(&item->list);
            free(item);
            return 0;
        }
    }
    return -1;
}

void EffGCAddAnimation(EffAnimation *animation) {
    int ret;
    LOCK();
    ret = gc_add_object(animation, &animation_list);
    UNLOCK();

    if (ret != 0) {
        fprintf(stderr, "MGEFF-GC> error: duplicate animation %p\n", animation);
    }
}

void EffGCRemoveAnimation(EffAnimation *animation) {
    int ret;
    LOCK();
    ret = gc_remove_object(animation, &animation_list);
    UNLOCK();

    if (ret != 0) {
        fprintf(stderr, "MGEFF-GC> error: double free animation %p\n", animation);
    }
}

void EffGCAddSchedular(EffSchedular *schedular) {
    int ret;
    LOCK();
    ret = gc_add_object(schedular, &schedular_list);
    UNLOCK();

    if (ret != 0) {
        fprintf(stderr, "MGEFF-GC> error: duplicate schedular %p\n", schedular);
    }
}

void EffGCRemoveSchedular(EffSchedular *schedular) {
    int ret;
    LOCK();
    ret = gc_remove_object(schedular, &schedular_list);
    UNLOCK();

    if (ret != 0) {
        fprintf(stderr, "MGEFF-GC> error: double free schedular %p\n", schedular);
    }
}

#if 0 
static int gc_count(struct list_head *head) {
    int n=0;
    struct list_head *i;
    list_for_each(i, head) {
        n++;
    }
    return n;
}
#endif

void EffGCCheckPoint() {
    int n;
    LOCK();
    n = list_count(&animation_list);
    if (n > 0) {
        struct list_head *i;
        printf("MGEFF-GC> warning: there %s %d animation definitely lossed\n", n==1 ? "is" : "are", n);
        list_for_each(i, &animation_list) {
            gc_item_t *item = (gc_item_t *)i;
            printf("MGEFF-GC> warning: %p animation lossed\n", item->object);
        }
    }
                
    n = list_count(&schedular_list);
    if (n > 0) {
        printf("MGEFF-GC> warning: there %s %d schedular definitely lossed\n", n==1 ? "is" : "are", n);
    }
    UNLOCK();
}
