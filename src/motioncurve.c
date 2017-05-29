/*! ============================================================================
 * \file montioncurve.c 
 * \brief  
 * \author DongKai
 * \version 1.0
 * \date 02/07/2010
 *
 *  Company: Beijing Feynman Software Technology Co., Ltd.
 */
#include <stdio.h>
#include <stdlib.h>
#include "mgeff.h"
#include "motion-curve.h"
#include "easing.h"

void  mGEffCurveSetProperty(MGEFF_MOTIONCURVE handle, enum EffCurveProperty id, float value)
{
    EffMotionCurve* curve = (EffMotionCurve*)handle;
    CHECK_MOTIONCURVE_EX(curve);
    switch(id) {
        case MGEFFCURVE_PROP_OVERSHOOT:
            ((EffMotionCurveEx *)curve)->overshoot = value;
            break;
        case MGEFFCURVE_PROP_PERIOD:
            ((EffMotionCurveEx *)curve)->period= value;
            break;
        case MGEFFCURVE_PROP_AMPLITUDE:
            ((EffMotionCurveEx *)curve)->amplitude= value;
            break;
        default:
            break;
    }
}

float mGEffCurveGetProperty(MGEFF_MOTIONCURVE handle, enum EffCurveProperty id)
{
    float ret = 0;
    EffMotionCurve* curve = (EffMotionCurve*)handle;
    CHECK_MOTIONCURVE_EX_RET(curve, 0.0f);
    switch(id) {
        case MGEFFCURVE_PROP_OVERSHOOT:
            ret = ((EffMotionCurveEx *)curve)->overshoot;
            break;
        case MGEFFCURVE_PROP_PERIOD:
            ret = ((EffMotionCurveEx *)curve)->period;
            break;
        case MGEFFCURVE_PROP_AMPLITUDE:
            ret = ((EffMotionCurveEx *)curve)->amplitude;
            break;
        default:
            break;
    }
    return ret;
}

EffMotionCurve* effmotioncurve_init(int type)
{
    EffMotionCurve* curve = NULL; 

    if (type >= InElastic && type <= OutInBounce) {  
        curve = (EffMotionCurve*)calloc(1, sizeof(EffMotionCurveEx));
        ((EffMotionCurveEx*)curve)->period = 0.3;
        ((EffMotionCurveEx*)curve)->amplitude = 1.0;
        ((EffMotionCurveEx*)curve)->overshoot = 1.70158;
        ((EffMotionCurveEx*)curve)->curveex_cb = 
            effmotioncurve_getfuncex(type);
    }
    else {
        curve = (EffMotionCurve*)calloc(1, sizeof(EffMotionCurve));
        curve->curve_cb = effmotioncurve_getfunc(type);
    }
    return curve;
}

float effmotioncurve_elastic_cb(EffMotionCurve* c, float t)
{
    float p = (((EffMotionCurveEx *)c)->period < 0) ? 0.3f : ((EffMotionCurveEx *)c)->period ;
    float a = (((EffMotionCurveEx *)c)->amplitude < 0) ? 1.0f : ((EffMotionCurveEx *)c)->amplitude;

    switch(c->curve_type) {
        case InElastic:
            return easeInElastic(t, a, p);
        case OutElastic:
            return easeOutElastic(t, a, p);
        case InOutElastic:
            return easeInOutElastic(t, a, p);
        case OutInElastic:
            return easeOutInElastic(t, a, p);
        default:
            return t;
    }
}

float effmotioncurve_back_cb(EffMotionCurve* c, float t)
{
    float  o = (((EffMotionCurveEx *)c)->overshoot < 0) ? 1.70158f : ((EffMotionCurveEx *)c)->overshoot;

    switch(c->curve_type) {
        case InBack:
            return easeInBack(t, o);
        case OutBack:
            return easeOutBack(t, o);
        case InOutBack:
            return easeInOutBack(t, o);
        case OutInBack:
            return easeOutInBack(t, o);
        default:
            return t;
    }
}

float effmotioncurve_bounce_cb(EffMotionCurve* c, float t)
{
    float a = (((EffMotionCurveEx *)c)->amplitude < 0) ? 1.0f : ((EffMotionCurveEx *)c)->amplitude;

    switch(c->curve_type) {
        case InBounce:
            return easeInBounce(t, a);
        case OutBounce:
            return easeOutBounce(t, a);
        case InOutBounce:
            return easeInOutBounce(t, a);
        case OutInBounce:
            return easeOutInBounce(t, a);
        default:
            return t;
    }
}

float effmotioncurve_calcvalue(EffMotionCurve* c, float t)
{
    if (c->curve_cb) {
        return c->curve_cb(t);
    }
    if (((EffMotionCurveEx*)c)->curveex_cb) {
        return ((EffMotionCurveEx*)c)->curveex_cb(c, t);
    }
    return t;
}

MGEFF_MOTIONCURVE_CB effmotioncurve_getfunc(int type)
{
    switch(type) {
    case Linear:
        return &easeNone;
    case InQuad:
        return &easeInQuad;
    case OutQuad:
        return &easeOutQuad;
    case InOutQuad:
        return &easeInOutQuad;
    case OutInQuad:
        return &easeOutInQuad;
    case InCubic:
        return &easeInCubic;
    case OutCubic:
        return &easeOutCubic;
    case InOutCubic:
        return &easeInOutCubic;
    case OutInCubic:
        return &easeOutInCubic;
    case InQuart:
        return &easeInQuart;
    case OutQuart:
        return &easeOutQuart;
    case InOutQuart:
        return &easeInOutQuart;
    case OutInQuart:
        return &easeOutInQuart;
    case InQuint:
        return &easeInQuint;
    case OutQuint:
        return &easeOutQuint;
    case InOutQuint:
        return &easeInOutQuint;
    case OutInQuint:
        return &easeOutInQuint;
    case InSine:
        return &easeInSine;
    case OutSine:
        return &easeOutSine;
    case InOutSine:
        return &easeInOutSine;
    case OutInSine:
        return &easeOutInSine;
    case InExpo:
        return &easeInExpo;
    case OutExpo:
        return &easeOutExpo;
    case InOutExpo:
        return &easeInOutExpo;
    case OutInExpo:
        return &easeOutInExpo;
    case InCirc:
        return &easeInCirc;
    case OutCirc:
        return &easeOutCirc;
    case InOutCirc:
        return &easeInOutCirc;
    case OutInCirc:
        return &easeOutInCirc;
    // Internal for, compatibility with QTimeLine only ??
    case InCurve:
        return &easeInCurve;
    case OutCurve:
        return &easeOutCurve;
    case SineCurve:
        return &easeSineCurve;
    case CosineCurve:
        return &easeCosineCurve;
    default:
        return NULL;
    }
    return NULL;
}

MGEFF_MOTIONCURVEEX_CB effmotioncurve_getfuncex(int type)
{
    switch(type) {
        case InElastic:
        case OutElastic:
        case InOutElastic:
        case OutInElastic:
            return effmotioncurve_elastic_cb;
        case InBack:
        case OutBack:
        case InOutBack:
        case OutInBack:
            return effmotioncurve_back_cb;
        case InBounce:
        case OutBounce:
        case InOutBounce:
        case OutInBounce:
            return effmotioncurve_bounce_cb;
            /* ... */
        default:
                break;
    }
    return NULL;
}

