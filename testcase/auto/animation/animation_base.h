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

