/*! ============================================================================
 * \file animation-group.h 
 * \brief Head file for animation group. 
 * \author DongKai
 * \version 1.0
 * \date 02/08/2010 10:08:16 AM 
 *
 *  Company: Beijing Feynman Software Technology Co., Ltd.
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
    int   property_id;
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
