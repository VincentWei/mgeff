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
