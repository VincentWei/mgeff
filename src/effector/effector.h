#ifndef _EFF_EFFECTOR_H
#define _EFF_EFFECTOR_H
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "linux-list.h"
#include "mgeff-effector.h"
#include "animation.h"
#include "effector-sink.h"
#include "effector-source.h"

#define FAST_MODE   0
#define MGPLUS_MODE 1

typedef struct _EffCBObjInfo{
    struct list_head list;
    void* obj;
}EffCBObjInfo;

struct _EffEffector;
typedef struct _EffEffector EffEffector;

struct _EffEffector{
    unsigned long key;
    MGEFF_BOOL initstate;
    int        direction;
    EffSink*   sink;
    void*      context;
    struct list_head source_list;
    MGEFF_EFFECTOROPS* ops;
};

int  effbaseeffector_setproperty(MGEFF_EFFECTOR effector, int property_id, int value);
int  effbaseeffector_getproperty(MGEFF_EFFECTOR effector, int property_id, int* pValue);
void effbaseeffector_rect(HDC hdc, RECT* rect);
int  effeffector_init(void);
void effeffector_deinit(void);
#endif
