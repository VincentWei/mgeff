#define dbg() printf("%s %d\n", __FUNCTION__, __LINE__)
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <minigui/common.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

#include "mgeff.h"
#include "animation-group.h"
#include "motion-curve.h"
#include "schedular.h"
#include "eff-common.h"
#include "garbage-collection.h"

#define MGEFF_NORMAL 0

typedef void (*EFF_RECURSION_CB) (EffAnimation*, int);

static int  effanimation_getdelta (EffAnimation* anim, int currenttime);
static int  effanimation_get_totaltime(EffAnimation* anim);
static void effanimation_setdirection(EffAnimation* anim, int direction);
static void effanimation_recursion(EffAnimation* anim, EFF_RECURSION_CB cb, int param);

#define IS_GROUP(a) (a->animationtype == MGEFF_PARALLEL \
        || a->animationtype == MGEFF_SEQUENTIAL)

#define IS_PARGROUP(a) \
    (a->animationtype == MGEFF_PARALLEL)

#define IS_SEQGROUP(a) (\
        a->animationtype == MGEFF_SEQUENTIAL)

#define IS_ANIMATION(a) (\
        a->animationtype == MGEFF_NORMAL)

#define CHECK_PROPID(id) \
        if (id < MGEFF_PROP_DURATION  || id >= MGEFF_PROP_MAX) { \
                   return; \
                }
#define CHECK_PROPID_RET(id, ret) \
        if (id < MGEFF_PROP_DURATION  || id >= MGEFF_PROP_MAX) { \
                   return ret; \
                }

#define MALLOC_VAR(type) (type*) malloc(sizeof(type))

#define  ANIM_VAR_INIT(anim, t) do {     \
    anim->startvalue = MALLOC_VAR(t);    \
    anim->endvalue   = MALLOC_VAR(t);    \
    anim->currentvalue = MALLOC_VAR(t);  \
    anim->variantsize = sizeof(t);       \
} while (0)

#define ANIM_VAR_DEINIT(anim) do {       \
    free(anim->startvalue);              \
    anim->startvalue = NULL;             \
    free(anim->endvalue);                \
    anim->endvalue= NULL;                \
    free(anim->currentvalue);            \
    anim->currentvalue= NULL;            \
    if (anim->curve) {                   \
        free(anim->curve);               \
    }                                    \
} while (0)

#define FACTOR(v1, v2, factor)        (v1) + ((v2) - (v1)) * (factor)
#define INTERPOLATE(type, s, e, v, factor)   *v = (type)(*s + (*e - *s) * factor)
#define INTERPOLATOR_TYPE(type) \
    INTERPOLATE(type, (type*)start, (type*)end, (type*)value, factor)

static void effanimation_interpolator_int(MGEFF_ANIMATION handle, void* start, void* end, 
        void* value, float factor)
{
    INTERPOLATOR_TYPE(int);
}

static void effanimation_interpolator_float(MGEFF_ANIMATION handle, void* start, void* end, 
        void* value, float factor)
{
    INTERPOLATOR_TYPE(float);
}

static void effanimation_interpolator_double(MGEFF_ANIMATION handle, void* start, void* end, 
        void* value, float factor)
{
    INTERPOLATOR_TYPE(double);
}

static void effanimation_interpolator_point2d(MGEFF_ANIMATION handle, void* start, void* end, 
        void* value, float factor)
{
    EffPoint* pt1 = (EffPoint*)start;
    EffPoint* pt2 = (EffPoint*)end;
    EffPoint* pt3 = (EffPoint*)value;
    pt3->x   = FACTOR(pt1->x, pt2->x, factor);
    pt3->y   = FACTOR(pt1->y, pt2->y, factor);
}

static void effanimation_interpolator_point3d(MGEFF_ANIMATION handle, void* start, void* end, 
        void* value, float factor)
{
    EffPoint3D* pt1 = (EffPoint3D*)start;
    EffPoint3D* pt2 = (EffPoint3D*)end;
    EffPoint3D* pt3 = (EffPoint3D*)value;
    pt3->x   = FACTOR(pt1->x, pt2->x, factor);
    pt3->y   = FACTOR(pt1->y, pt2->y, factor);
    pt3->z   = FACTOR(pt1->z, pt2->z, factor);
}

static void effanimation_interpolator_pointf2d(MGEFF_ANIMATION handle, void* start, void* end, 
        void* value, float factor)
{
    EffPointF* pt1 = (EffPointF*)start;
    EffPointF* pt2 = (EffPointF*)end;
    EffPointF* pt3 = (EffPointF*)value;
    pt3->x   = FACTOR(pt1->x, pt2->x, factor);
    pt3->y   = FACTOR(pt1->y, pt2->y, factor);
}

static void effanimation_interpolator_pointf3d(MGEFF_ANIMATION handle, void* start, void* end, 
        void* value, float factor)
{
    EffPointF3D* pt1 = (EffPointF3D*)start;
    EffPointF3D* pt2 = (EffPointF3D*)end;
    EffPointF3D* pt3 = (EffPointF3D*)value;
    pt3->x   = FACTOR(pt1->x, pt2->x, factor);
    pt3->y   = FACTOR(pt1->y, pt2->y, factor);
    pt3->z   = FACTOR(pt1->z, pt2->z, factor);
}

static void effanimation_interpolator_rect(MGEFF_ANIMATION handle, void* start, void* end, 
        void* value, float factor)
{
    EffRect* rc1 = (EffRect*)start;
    EffRect* rc2 = (EffRect*)end;
    EffRect* rc3 = (EffRect*)value;
    rc3->left   = FACTOR(rc1->left, rc2->left, factor);
    rc3->right  = FACTOR(rc1->right, rc2->right, factor);
    rc3->top    = FACTOR(rc1->top , rc2->top, factor);
    rc3->bottom = FACTOR(rc1->bottom , rc2->bottom, factor);
}

static void effanimation_interpolator_color(MGEFF_ANIMATION handle, void* start, void* end, 
        void* value, float factor)
{
    int as, ae, av;
    int rs, re, rv;
    int gs, ge, gv;
    int bs, be, bv;

    as = GetAValue(*(EffColor*)start);
    rs = GetRValue(*(EffColor*)start);
    gs = GetGValue(*(EffColor*)start);
    bs = GetBValue(*(EffColor*)start);
    ae = GetAValue(*(EffColor*)end);
    re = GetRValue(*(EffColor*)end);
    ge = GetGValue(*(EffColor*)end);
    be = GetBValue(*(EffColor*)end);

    av = FACTOR(as, ae, factor);
    rv = FACTOR(rs, re, factor);
    gv = FACTOR(gs, ge, factor);
    bv = FACTOR(bs, be, factor);

    *(EffColor*)value = MakeRGBA(rv, gv, bv, av);
}

static int effanimation_updatevalue(EffAnimation* anim, int time)
{
    float curve_value, progress;

    if (anim->duration)
        progress = 1.0f * time / anim->duration;
    else 
        progress = 1.0f;

    curve_value  = effmotioncurve_calcvalue(anim->curve, progress);
    _MGEFF_PRINTF("%s:time =%d, anim->duration = %d, progess = %f, curve_value = %f\n", 
            __FUNCTION__, time, anim->duration, progress, curve_value);

    if (anim->calcValue) {
        anim->calcValue((MGEFF_ANIMATION)anim, anim->startvalue, anim->endvalue, anim->currentvalue, curve_value);
    }
    if (anim->setProperty) {
        anim->setProperty((MGEFF_ANIMATION)anim, anim->targetobject, anim->property_id, anim->currentvalue);
    }
    return 0;
}


MGEFF_BOOL effanimation_init(EffAnimation* anim, int type)
{
    switch(type) {
        case MGEFF_INT:
            ANIM_VAR_INIT(anim, int);
            anim->calcValue = effanimation_interpolator_int;
            break;
        case MGEFF_FLOAT:
            ANIM_VAR_INIT(anim, float);
            anim->calcValue = effanimation_interpolator_float;
            break;
        case MGEFF_DOUBLE:
            ANIM_VAR_INIT(anim, double);
            anim->calcValue = effanimation_interpolator_double;
            break;
        case MGEFF_RECT:
            ANIM_VAR_INIT(anim, EffRect);
            anim->calcValue = effanimation_interpolator_rect;
            break;
        case MGEFF_POINT:
            ANIM_VAR_INIT(anim, EffPoint);
            anim->calcValue = effanimation_interpolator_point2d;
            break;
        case MGEFF_POINTF:
            ANIM_VAR_INIT(anim, EffPointF);
            anim->calcValue = effanimation_interpolator_pointf2d;
            break;
        case MGEFF_3DPOINT:
            ANIM_VAR_INIT(anim, EffPoint3D);
            anim->calcValue = effanimation_interpolator_point3d;
            break;
        case MGEFF_3DPOINTF:
            ANIM_VAR_INIT(anim, EffPointF3D);
            anim->calcValue = effanimation_interpolator_pointf3d;
            break;
        case MGEFF_COLOR:
            ANIM_VAR_INIT(anim, EffColor);
            anim->calcValue = effanimation_interpolator_color;
            break;
    } 

    anim->updateValue  = effanimation_updatevalue;
    anim->loopcount    = 1;
    anim->duration     = 1000;
    anim->currenttime  = 0;
    anim->currentframe = 0;
    anim->frequency    = 50;
    anim->lasttime = eff_time();
    anim->state = MGEFF_STATE_STOPPED;
    anim->direction = MGEFF_DIR_FORWARD;
    return MGEFF_TRUE;
}

int effanimation_getnexttime(EffAnimation* anim)
{
    return anim->lasttime + (anim->duration/anim->frequency);
}

MGEFF_BOOL effanimation_comparetime(EffAnimation* a, EffAnimation* b)
{
    if (effanimation_getnexttime(a) < effanimation_getnexttime(b)) {
        return MGEFF_TRUE;
    }
    return MGEFF_FALSE;
}

void effanimation_setstart(EffAnimation* anim, int time_ms)
{
    if (IS_ANIMATION(anim)
            || (IS_SEQGROUP(anim))) {
        if (anim->state == MGEFF_STATE_READY) {
            anim->lasttime = time_ms;
            effanimation_setstate(anim, MGEFF_STATE_RUNNING);
        }
    }
    else if (IS_PARGROUP(anim)){
        EffAnimationGroup* group = (EffAnimationGroup*)anim;
        EffAnimation *animation;
        struct list_head *i;
        if (group->animation.state == MGEFF_STATE_READY) {
            /* If it's a animation group, make all animation nodes start. */
            list_for_each(i, &group->animation_list) {
                animation = (EffAnimation *)i;
                effanimation_setstart(animation, time_ms);
            }
            anim->lasttime = time_ms;
            effanimation_setstate(anim, MGEFF_STATE_RUNNING);
        }
    }
}


MGEFF_BOOL effanimation_running(EffAnimation* anim)
{
    return anim->state == MGEFF_STATE_RUNNING;
}

MGEFF_BOOL effanimation_ready(EffAnimation* anim)
{
    return anim->state == MGEFF_STATE_READY;
}

static int effanimation_getdelta (EffAnimation* anim, int currenttime)
{
    int delta = currenttime - anim->lasttime;
    _MGEFF_PRINTF("%s:direction = %d\n", __FUNCTION__, anim->direction);
    if (anim->direction != MGEFF_DIR_FORWARD) {
        _MGEFF_PRINTF("%s:anim->lasttime = %d, currenttime is %d, delta is %d\n", __FUNCTION__, 
                anim->lasttime, currenttime, delta);
        delta = -delta;
    }
    anim->lasttime = currenttime;
    return delta;
}

static int effanimation_get_totaltime(EffAnimation* anim)
{
    return anim->duration * anim->loopcount;
}

/* If animation over or param invalid, it will return 0, else return 1. */
int effanimation_setcurrenttime(EffAnimation* anim, int currenttime)
{
    int oldloop, delta, time, total_time;

    CHECK_HANDLE_RET(anim);

    oldloop = anim->currentloop;
    delta = effanimation_getdelta (anim, currenttime);
    if (anim->state != MGEFF_STATE_RUNNING || anim->duration <= 0) {
        if (anim->duration == 0) assert(0);
        return 0;
    }

    anim->currenttime += delta ;
    _MGEFF_PRINTF("%s: anim->duration = %d, totaltime = %d, anim->loopcount = %d\n", __FUNCTION__, anim->duration, 
            anim->duration * anim->loopcount, anim->loopcount);

    total_time = effanimation_get_totaltime(anim);
    if (anim->loopcount != MGEFF_INFINITE) {
        if (anim->direction == MGEFF_DIR_FORWARD 
                && anim->currenttime > total_time){
            anim->currenttime = total_time;
        }
        else if (anim->direction == MGEFF_DIR_BACKWARD && anim->currenttime < 0) {
            anim->currenttime = 0;
        }
    }

    anim->currentloop = anim->currenttime / anim->duration;
#if 0
    if (anim->currentloop != oldloop ){
        if (anim->curloopchanged_cb){
            anim->curloopchanged_cb((MGEFF_ANIMATION)anim);
        }
    }
#endif
    if (anim->currenttime && !(anim->currenttime % anim->duration)) {
        time = anim->duration;
    }
    else {
        time = anim->currenttime % anim->duration;
    }
    if (IS_ANIMATION(anim)) {
        if (anim && anim->updateValue)
            anim->updateValue(anim, time);
        anim->currentframe++;
    }
    else if(IS_GROUP(anim)){
        anim->updateValue(anim, currenttime);
        anim->currentframe++;
    }
    else {
        return 0;
    }

    if (anim->currentloop != oldloop ){
        if (anim->curloopchanged_cb){
            anim->curloopchanged_cb((MGEFF_ANIMATION)anim);
        }
    }

    _MGEFF_PRINTF("%s:time = %d, anim->currentloop = %d, anim->currenttime = %d\n", __FUNCTION__, 
            time, anim->currentloop, anim->currenttime);

    if (!IS_GROUP(anim) && ((anim->direction == MGEFF_DIR_FORWARD
            && anim->currenttime == total_time)
            || (anim->direction == MGEFF_DIR_BACKWARD && anim->currenttime == 0))) {
        effanimation_setstate(anim, MGEFF_STATE_STOPPED);
        return 0;
    }
    return 1;
}

void effanimation_resume(EffAnimation* anim, int not_used)
{
    if (anim->state != MGEFF_STATE_PAUSED) {
        return;
    }
    effanimation_setstate(anim, MGEFF_STATE_READY);
}

void effanimation_paused(EffAnimation* anim, int no_used)
{
    if (anim->state != MGEFF_STATE_RUNNING) {
        return;
    }
    effanimation_setstate(anim, MGEFF_STATE_PAUSED);
}

void effanimation_setstate(EffAnimation* anim, int state)
{
    int oldstate;
    if (anim->state == state) {
        return;
    }
    oldstate = anim->state;
#if 0
    if ((state == MGEFF_STATE_RUNNING || state == MGEFF_STATE_PAUSED
                || state == MGEFF_STATE_READY)
            && anim->state == MGEFF_STATE_STOPPED) {
        anim->currentframe = 0;
        if (anim->direction == MGEFF_DIR_FORWARD)
            anim->currenttime = 0;
        else 
            anim->currenttime = effanimation_get_totaltime(anim);
    }
#else
    if ((state == MGEFF_STATE_RUNNING
                || state == MGEFF_STATE_READY)
            && anim->state == MGEFF_STATE_STOPPED) {
        anim->currentframe = 0;
        if (anim->direction == MGEFF_DIR_FORWARD)
            anim->currenttime = 0;
        else 
            anim->currenttime = effanimation_get_totaltime(anim);
    }
#endif

    if (anim->state != state && anim->statechanged_cb) {
        anim->statechanged_cb((MGEFF_ANIMATION)anim, (enum EffState)state, anim->state);
    }

    anim->state = (enum EffState)state;

    switch (state) {
        case MGEFF_STATE_RUNNING:
            /* not need call at this moment.*/
            //effanimation_setcurrenttime(anim, anim->lasttime);
            break;
        case MGEFF_STATE_STOPPED:
            _MGEFF_PRINTF("%s:animation stopped.\n", __FUNCTION__);
            if (!anim->keepalive) {
                anim->state = MGEFF_STATE_REMOVE;
            } else {
                anim->state = MGEFF_STATE_STOPPED;
            }

#if 0
            if (oldstate == MGEFF_STATE_RUNNING  && anim->finished_cb){
                anim->finished_cb((MGEFF_ANIMATION)anim);
            }
#else
            if (anim->finished_cb){
                anim->finished_cb((MGEFF_ANIMATION)anim);
            }
#endif
            break;
        case MGEFF_STATE_PAUSED:
            _MGEFF_PRINTF("%s:animation paused.\n", __FUNCTION__);
            break;
        case MGEFF_STATE_READY:
            _MGEFF_PRINTF("%s:animation ready.\n", __FUNCTION__);
            break;
    }
}

MGEFF_ANIMATION mGEffAnimationCreateEx(void* target, MGEFF_SETPROPERTY_CB setproperty, int id,
       int size, MGEFF_VARMALLOC_CB cb1, MGEFF_CALCVALUE_CB cb2)
{
    MGEFF_ANIMATION handle = mGEffAnimationCreate(target, setproperty, id, (enum EffVariantType)-1);
    EffAnimation* anim;

    CHECK_HANDLE_RET_NIL(handle);

    anim = (EffAnimation*)handle;
    anim->startvalue   = cb1(handle);   
    anim->endvalue     = cb1(handle);    
    anim->currentvalue = cb1(handle);  
    anim->calcValue    = cb2;
    anim->variantsize  = size;

    return handle;
}

MGEFF_ANIMATION mGEffAnimationCreate(void* target, MGEFF_SETPROPERTY_CB setproperty, 
        int id, enum EffVariantType varianttype)
{
    EffAnimation* anim = (EffAnimation*)calloc(1, sizeof(EffAnimation));

    CHECK_HANDLE_RET_NIL(anim);
    effanimation_init(anim, varianttype);
    anim->setProperty = setproperty;
    anim->property_id = id;
    anim->targetobject = target;
    anim->keepalive = 1;

    EffGCAddAnimation(anim);
    return (MGEFF_ANIMATION)anim;

}

static void effanimation_recursion(EffAnimation* anim, EFF_RECURSION_CB cb, int param)
{
    if (IS_ANIMATION(anim)) {
        cb(anim, param);
    }
    else if (IS_GROUP(anim)){
        EffAnimationGroup* group = (EffAnimationGroup*)anim;
        EffAnimation *animation;
        struct list_head *i;
        /* If it's a animation group, make all animation nodes start. */
        list_for_each(i, &group->animation_list) {
            animation = (EffAnimation *)i;
            effanimation_recursion(animation, cb, param);
        }
        cb(anim, param);
    }
}

void  mGEffAnimationDelete(MGEFF_ANIMATION handle)
{
    EffAnimation* anim = (EffAnimation*)handle;
    CHECK_HANDLE(handle);

    if (!(IS_ANIMATION(anim) || IS_GROUP(anim))) return;
    if (IS_GROUP(anim)) {
        EffAnimationGroup* group = (EffAnimationGroup*)anim;
        EffAnimation *child;
        struct list_head *i, *j;
        list_for_each(i, &group->animation_list) {
            j = i->prev;
            child = (EffAnimation *)i;
            list_del(i);
            mGEffAnimationDelete((MGEFF_ANIMATION) child);
            i = j;
        }
    }
    ANIM_VAR_DEINIT(anim);

    EffGCRemoveAnimation(anim);

    /* if is not keepalive there is auto delete in another place */
    if (anim->keepalive) 
        effschedular_remove_animation(g_schedular_default, anim);

    free(anim);

    _MGEFF_PRINTF("%s: delete animation.\n", __FUNCTION__);
}

static void effanimation_setdirection(EffAnimation* anim, int direction)
{
    anim->direction= (enum EffDirection)direction;
    if (anim->dirchanged_cb) {
        anim->dirchanged_cb((MGEFF_ANIMATION)anim);
    }
    _MGEFF_PRINTF("%s: direction =%d\n", __FUNCTION__, anim->direction);
}

void mGEffAnimationSetProperty(MGEFF_ANIMATION handle, enum EffAnimProperty id, int value)
{
    EffAnimation* anim = (EffAnimation*)handle;

    CHECK_HANDLE(handle);
    CHECK_PROPID(id);

    switch(id) {
        case MGEFF_PROP_DURATION:
            if (value <= 0) {
                fprintf(stderr, "%s:mGEff-Animation> Set Duration < 0 !!!\n", __FUNCTION__);
            }
            assert(value > 0);
            anim->duration = value;
            break;
        case MGEFF_PROP_LOOPCOUNT:
            anim->loopcount = value;
            break;
        case MGEFF_PROP_DIRECTION:
            effanimation_recursion(anim , effanimation_setdirection,  value);
            break;
        case MGEFF_PROP_CURLOOP:
        case MGEFF_PROP_CURFRAME:
        case MGEFF_PROP_STATE:
            _MGEFF_PRINTF("mGEff-Animation> Read Only property: %x\n", id);
            break;
        case MGEFF_PROP_KEEPALIVE:
            anim->keepalive = value;
            break;
        default:
            _MGEFF_PRINTF("mGEff-Animation> Unknown property: %x\n", id);
            break;
    }
}

void mGEffAnimationSetDuration(MGEFF_ANIMATION handle,
        int duration_ms)
{
    mGEffAnimationSetProperty(handle, MGEFF_PROP_DURATION, duration_ms);
}


int mGEffAnimationGetProperty(MGEFF_ANIMATION handle, enum EffAnimProperty id)
{
    int ret = 0;
    EffAnimation* anim = (EffAnimation*)handle;
    CHECK_HANDLE_RET(handle);
    CHECK_PROPID_RET(id, -1);

    switch(id) {
        case MGEFF_PROP_DURATION:
            ret  = anim->duration;
            break;
        case MGEFF_PROP_CURLOOP:
            ret  = anim->currentloop;
            break;
        case MGEFF_PROP_LOOPCOUNT:
            ret  = anim->loopcount;
            break;
        case MGEFF_PROP_DIRECTION:
            ret  = anim->direction;
            break;
        case MGEFF_PROP_CURFRAME:
            ret  = anim->currentframe;
            break;
        case MGEFF_PROP_STATE:
            ret  = anim->state;
            break;
        case MGEFF_PROP_KEEPALIVE:
            ret = anim->keepalive;
        default:
            _MGEFF_PRINTF("mGEff-Animation> Unknown property: %x\n", id);
            break;
    }
    return ret;
}

int mGEffAnimationGetDuration(MGEFF_ANIMATION handle)
{
    return mGEffAnimationGetProperty(handle, MGEFF_PROP_DURATION);
}


void mGEffAnimationSetStartValue(MGEFF_ANIMATION handle, const void* value)
{
    EffAnimation* anim = (EffAnimation*)handle;
    CHECK_HANDLE(handle);
    CHECK_HANDLE(value);
    CHECK_HANDLE(anim->startvalue);
    /* copy value.*/
    memcpy(anim->startvalue, value, anim->variantsize);
}

const void* mGEffAnimationGetStartValue(MGEFF_ANIMATION handle)
{
    EffAnimation* anim = (EffAnimation*)handle;

    CHECK_HANDLE_RET_NIL(handle);

    return anim->startvalue;
}

void mGEffAnimationSetEndValue(MGEFF_ANIMATION handle, const void* value)
{
    EffAnimation* anim = (EffAnimation*)handle;

    CHECK_HANDLE(handle);
    CHECK_HANDLE(value);
    CHECK_HANDLE(anim->endvalue);
    /* copy value.*/
    memcpy(anim->endvalue, value, anim->variantsize);
}

const void* mGEffAnimationGetEndValue(MGEFF_ANIMATION handle)
{
    EffAnimation* anim = (EffAnimation*)handle;
    CHECK_HANDLE_RET_NIL(handle);

    return anim->endvalue;
}

#if 0
void mGEffAnimationSetCustVar(MGEFF_ANIMATION handle, MGEFF_VARMALLOC_CB malloc_cb,
        MGEFF_CALCVALUE_CB calc_cb)
{
    EffAnimation* anim = (EffAnimation*)handle;
    anim->startvalue = malloc_cb();
    anim->endvalue = malloc_cb();
    anim->currentvalue = malloc_cb();

    anim->calcValue = calc_cb;
}

void mGEffAnimationSetChangedCb(MGEFF_ANIMATION handle,  MGEFF_FINISHED_CB cb1, 
        MGEFF_STATECHANGED_CB cb2, MGEFF_CURLOOPCHANGED_CB cb3, 
        MGEFF_DIRCHANGED_CB cb4)
{


}

enum EffChangedCb{
    MGEFF_FINISHED_CB = 0,
    MGEFF_STATECHANGED_CB = 1,
    MGEFF_DIRCHANGED_CB = 2,
    MGEFF_CURLOOPCHANGED_CB = 3,
};

static inline void effanimation_setchanged_cb(MGEFF_ANIMATION handle, cb)
{
    EffAnimation* anim = (EffAnimation*)handle;
    CHECK_HANDLE(handle);

    switch(type) {
        case MGEFF_FINISHED_CB:
            anim->finished_cb = cb;
            break;
        case MGEFF_STATECHANGED_CB:
            break;
        case MGEFF_DIRCHANGED_CB:
            break;
        case MGEFF_CURLOOPCHANGED_CB:
            break;
    }
}
#endif

void  mGEffAnimationSetFinishedCb(MGEFF_ANIMATION handle, MGEFF_FINISHED_CB cb)
{
    EffAnimation* anim = (EffAnimation*)handle;
    CHECK_HANDLE(handle);

    anim->finished_cb = cb;
}

void  mGEffAnimationSetStateChangedCb(MGEFF_ANIMATION handle, MGEFF_STATECHANGED_CB cb)
{
    EffAnimation* anim = (EffAnimation*)handle;
    CHECK_HANDLE(handle);

    anim->statechanged_cb = cb;
}

void  mGEffAnimationSetCurLoopChangedCb(MGEFF_ANIMATION handle, MGEFF_CURLOOPCHANGED_CB cb)
{
    EffAnimation* anim = (EffAnimation*)handle;
    CHECK_HANDLE(handle);

    anim->curloopchanged_cb = cb;

}

void  mGEffAnimationSetDirChangedCb(MGEFF_ANIMATION handle,  MGEFF_DIRCHANGED_CB cb)
{
    EffAnimation* anim = (EffAnimation*)handle;
    CHECK_HANDLE(handle);

    anim->dirchanged_cb = cb;
}

void  mGEffAnimationSetContext(MGEFF_ANIMATION handle, void* context)
{
    EffAnimation* anim = (EffAnimation*)handle;
    CHECK_HANDLE(handle);

    anim->context = context;

}

void* mGEffAnimationGetContext(MGEFF_ANIMATION handle)
{
    EffAnimation* anim = (EffAnimation*)handle;
    CHECK_HANDLE_RET_NIL(handle);

    return anim->context;
}

#define CHECK_CURVE_TYPE(type)                    \
    if (type < Linear || type > NCurveTypes) {    \
        return;                                   \
    }

void  mGEffAnimationSetCurve(MGEFF_ANIMATION handle, enum EffMotionType type)
{
    EffAnimation* anim = (EffAnimation*)handle;

    CHECK_HANDLE(handle);
    CHECK_CURVE_TYPE(type);

    if ((anim->curve && type == anim->curve->curve_type) || type == Custom) {
        return;
    }
    if (anim->curve) {
        free(anim->curve);
    }
    anim->curve = effmotioncurve_init(type);

    if (type >= InElastic && type <= OutInBounce) {  
        EffMotionCurveEx* curve_ex = (EffMotionCurveEx*)anim->curve;
        curve_ex->curveex_cb = effmotioncurve_getfuncex(type);
    }
    else {
        anim->curve->curve_cb = effmotioncurve_getfunc(type);
    }
    anim->curve->curve_type = type;
}

void  mGEffAnimationSetCurveEx(MGEFF_ANIMATION handle, MGEFF_MOTIONCURVE_CB cb)
{
    EffAnimation* anim = (EffAnimation*)handle;
    EffMotionCurve* curve = NULL;
    mGEffAnimationSetCurve(handle, Linear);
    curve = anim->curve;
    CHECK_MOTIONCURVE(curve);

    curve->curve_type = Custom;
    curve->curve_cb   = cb;
}

MGEFF_MOTIONCURVE mGEffAnimationGetCurve(MGEFF_ANIMATION handle)
{
    EffAnimation* anim = (EffAnimation*)handle;
    CHECK_HANDLE_RET_NIL(handle);

    return (MGEFF_MOTIONCURVE)anim->curve;
}


void*  mGEffAnimationGetTarget(MGEFF_ANIMATION handle)
{
    EffAnimation* anim = (EffAnimation*)handle;

    CHECK_HANDLE_RET_NIL(handle);
    return anim->targetobject;
}

void effanimation_start(EffAnimation* anim, int not_used)
{
    if (anim->state != MGEFF_STATE_READY) {
        effanimation_setstate(anim, MGEFF_STATE_READY);

        /* if not set curve, default set as Linear. */
        if (!anim->curve) {
            anim->curve = effmotioncurve_init(Linear);
        }
    }
}

int  mGEffAnimationSyncRun(MGEFF_ANIMATION handle)
{
    EffAnimation* anim = (EffAnimation*)handle;
    CHECK_HANDLE_RET(handle);

    if (!g_schedular_default) return -1;
    effanimation_recursion(anim, effanimation_start, -1);
    effschedular_add_animation(g_schedular_default, anim);

    if (anim->state != MGEFF_STATE_READY) return -1;
    effschedular_sync_run(g_schedular_default, anim);

    return 0;
}


int mGEffAnimationAsyncRun(MGEFF_ANIMATION handle)
{
    EffAnimation* anim = (EffAnimation*)handle;
    EffSchedular* schedular;

    CHECK_HANDLE_RET(handle);
    schedular = effschedular_check_sch();
    if (!schedular) {
        return -1;
    }

    effanimation_recursion(anim, effanimation_start, -1);
    effschedular_add_animation(schedular, anim);

    return 0;
}

void mGEffAnimationPause(MGEFF_ANIMATION handle)
{
    EffAnimation* anim = (EffAnimation*)handle;
    CHECK_HANDLE(handle);

    if (anim->state == MGEFF_STATE_PAUSED) return;
    effanimation_recursion(anim, effanimation_paused, MGEFF_STATE_PAUSED);
}

void mGEffAnimationResume(MGEFF_ANIMATION handle)
{
    EffAnimation* anim = (EffAnimation*)handle;
    CHECK_HANDLE(handle);

    if (anim->state != MGEFF_STATE_PAUSED) return;
    effanimation_recursion(anim, effanimation_resume, MGEFF_STATE_READY);
}

MGEFF_BOOL mGEffAnimationWait(void* phWnd, MGEFF_ANIMATION handle)
{
    EffAnimation* anim = (EffAnimation*)handle;
    EffSchedular* schedular = NULL;
    HWND mainHwnd;
    CHECK_HANDLE_RET(handle);

    mainHwnd = GetMainWindowHandle(*(HWND *)phWnd);
    schedular = effschedular_check_sch();
    if (!schedular) {
        assert(0);
        dbg();
        return MGEFF_FALSE;
    }

#ifndef _MG_MINIMALGDI 
    {
        MSG Msg;
        for (;;) {
            /* check if the animation is in the schedular's animation_list.*/
            {
                MGEFF_BOOL find_flag = MGEFF_FALSE;
                struct list_head *i;
                list_for_each(i, &schedular->animation_list) {
                    if (anim == (EffAnimation *)i) {
                        find_flag = MGEFF_TRUE;
                        break;
                    }
                }
                if (! find_flag) {
                    dbg();
                    break;
                }
            }

            if (! GetMessage(&Msg, mainHwnd)) {
                assert(0);
                break;
            }

            TranslateMessage(&Msg);
            DispatchMessage(&Msg);
        }
    }
    return MGEFF_TRUE;
#else
#error
    return MGEFF_FALSE;
#endif
}

void mGEffAnimationStop(MGEFF_ANIMATION handle)
{
    /* TODO: finish callback is not called
     */
    EffAnimation* anim = (EffAnimation*)handle;
    CHECK_HANDLE(handle);

    if (anim->state == MGEFF_STATE_STOPPED) return;
    effanimation_setstate((EffAnimation *)handle, MGEFF_STATE_STOPPED);

    /* delete the animation or remove from animation list at moment*/
#if 0
    effschedular_remove_animation(g_schedular_default, anim);
    if (!anim->keepalive) {
        mGEffAnimationDelete((MGEFF_ANIMATION)anim);
    }
#endif
}

static void eff_copy_value(MGEFF_ANIMATION animation, void *target, int id, void *value) 
{
    memcpy(target, value, ((EffAnimation *)animation)->variantsize);
}

MGEFF_ANIMATION mGEffCreatePropertyAnimation(const EffPropertyAnimationSetting *settings) 
{
    MGEFF_ANIMATION group = NULL;
    MGEFF_ANIMATION animation = NULL;
    const EffPropertyAnimationSetting *p;

    if (!settings || !settings[0].property) {
        assert(0);
        return NULL;
    }

    group = mGEffAnimationCreateGroup(MGEFF_PARALLEL);

    for (p=settings; p->property; ++p) {
        animation = mGEffAnimationCreate(p->property, eff_copy_value, 0, p->type);
        mGEffAnimationSetStartValue(animation, p->start_value);
        mGEffAnimationSetEndValue(animation, p->end_value);
        mGEffAnimationSetCurve(animation, p->curve);
        mGEffAnimationSetDuration(animation, p->duration);
        mGEffAnimationSetFinishedCb (animation, p->cb);

        mGEffAnimationAddToGroup(group, animation);
    }

    return group;
}

void effanimation_restart(MGEFF_ANIMATION handle)
{
    EffAnimation* anim = (EffAnimation*)handle;
    CHECK_HANDLE(handle);
    if (IS_ANIMATION(anim)
            || (IS_PARGROUP(anim))
            || (IS_SEQGROUP(anim))) {
        if (anim->state != MGEFF_STATE_RUNNING) {
#if 0
            effanimation_recursion(anim, effanimation_start, -1);
            effanimation_recursion(anim, effanimation_setstate, MGEFF_STATE_READY);
#else
            effanimation_recursion(anim, effanimation_start, MGEFF_STATE_READY);
#endif
        }
    }
}
