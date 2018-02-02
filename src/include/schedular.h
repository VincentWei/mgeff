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

#ifndef _SCHEDULAR_H
#define _SCHEDULAR_H

#include "linux-list.h"
#include "animation.h"

typedef struct _EffSchedular {
    struct list_head list;
    int status;
    struct list_head animation_list;
    int id;
} EffSchedular;

extern EffSchedular *g_schedular_default;

/**
 * \fn void schedular_add_animation(EffSchedular *schedular, EffAnimation *animation)
 *
 * schedular_add_animation() attachs the animation to schedular. The animation is going to run in the running space of the schedular.
 *
 * \sa schedular_remove_animation
 */
void effschedular_add_animation(EffSchedular *schedular, EffAnimation *animation);

/**
 * \fn void schedular_remove_animation(EffSchedular *schedular, EffAnimation *animation)
 *
 * schedular_remove_animation() dettachs the animation from the schedular.
 *
 * \sa schedular_add_animation
 */
void effschedular_remove_animation(EffSchedular *schedular, EffAnimation *animation);

void effschedular_destroy(EffSchedular* schedular);
EffSchedular* effschedular_create(void);
EffSchedular* effschedular_check_sch(void);
void effschedular_sync_run(EffSchedular *schedular, EffAnimation *animation);
int effschedular_one_step(EffSchedular *schedular, EffAnimation *animation_wanted);

#endif
