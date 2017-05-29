#ifndef _EFF_MOTIONCURVE_H
#define _EFF_MOTIONCURVE_H
#include "mgeff-motioncurve.h"


typedef struct _EffMotionCurve{
    int curve_type;
    MGEFF_MOTIONCURVE_CB curve_cb;
}EffMotionCurve;

typedef float (*MGEFF_MOTIONCURVEEX_CB)(EffMotionCurve* curve, float progress);
typedef struct _EffMotionCurveEx{
    EffMotionCurve base;
    float period;
    float amplitude;
    float overshoot;
    MGEFF_MOTIONCURVEEX_CB curveex_cb;
} EffMotionCurveEx;

#define CHECK_MOTIONCURVE(curve)    \
    if (NULL == curve || curve->curve_type < Linear || curve->curve_type > NCurveTypes) { \
        return; \
    }

#define CHECK_MOTIONCURVE_EX(curve) \
    if (NULL == curve || curve->curve_type < InElastic || curve->curve_type > OutInBounce) { \
        return; \
    }

#define CHECK_MOTIONCURVE_EX_RET(curve, ret) \
    if (NULL == curve || curve->curve_type < InElastic || curve->curve_type > OutInBounce) { \
        return ret; \
    }


EffMotionCurve* effmotioncurve_init(int type);
float effmotioncurve_elastic_cb(EffMotionCurve* c, float t);
float effmotioncurve_back_cb(EffMotionCurve* c, float t);
float effmotioncurve_bounce_cb(EffMotionCurve* c, float t);
float effmotioncurve_calcvalue(EffMotionCurve* c, float t);
MGEFF_MOTIONCURVE_CB effmotioncurve_getfunc(int type);
MGEFF_MOTIONCURVEEX_CB effmotioncurve_getfuncex(int type);

#endif
