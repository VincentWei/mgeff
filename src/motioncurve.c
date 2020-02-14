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
 *   <http://www.minigui.com/blog/minigui-licensing-policy/>.
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

EffMotionCurve* __mgeffmotioncurve_init(int type)
{
    EffMotionCurve* curve = NULL; 

    if (type >= InElastic && type <= OutInBounce) {  
        curve = (EffMotionCurve*)calloc(1, sizeof(EffMotionCurveEx));
        ((EffMotionCurveEx*)curve)->period = 0.3;
        ((EffMotionCurveEx*)curve)->amplitude = 1.0;
        ((EffMotionCurveEx*)curve)->overshoot = 1.70158;
        ((EffMotionCurveEx*)curve)->curveex_cb = 
            __mgeffmotioncurve_getfuncex(type);
    }
    else {
        curve = (EffMotionCurve*)calloc(1, sizeof(EffMotionCurve));
        curve->curve_cb = __mgeffmotioncurve_getfunc(type);
    }
    return curve;
}

static float effmotioncurve_elastic_cb(EffMotionCurve* c, float t)
{
    float p = (((EffMotionCurveEx *)c)->period < 0) ? 0.3f : ((EffMotionCurveEx *)c)->period ;
    float a = (((EffMotionCurveEx *)c)->amplitude < 0) ? 1.0f : ((EffMotionCurveEx *)c)->amplitude;

    switch(c->curve_type) {
        case InElastic:
            return __mgeff_easeInElastic(t, a, p);
        case OutElastic:
            return __mgeff_easeOutElastic(t, a, p);
        case InOutElastic:
            return __mgeff_easeInOutElastic(t, a, p);
        case OutInElastic:
            return __mgeff_easeOutInElastic(t, a, p);
        default:
            return t;
    }
}

static float effmotioncurve_back_cb(EffMotionCurve* c, float t)
{
    float  o = (((EffMotionCurveEx *)c)->overshoot < 0) ? 1.70158f : ((EffMotionCurveEx *)c)->overshoot;

    switch(c->curve_type) {
        case InBack:
            return __mgeff_easeInBack(t, o);
        case OutBack:
            return __mgeff_easeOutBack(t, o);
        case InOutBack:
            return __mgeff_easeInOutBack(t, o);
        case OutInBack:
            return __mgeff_easeOutInBack(t, o);
        default:
            return t;
    }
}

static float effmotioncurve_bounce_cb(EffMotionCurve* c, float t)
{
    float a = (((EffMotionCurveEx *)c)->amplitude < 0) ? 1.0f : ((EffMotionCurveEx *)c)->amplitude;

    switch(c->curve_type) {
        case InBounce:
            return __mgeff_easeInBounce(t, a);
        case OutBounce:
            return __mgeff_easeOutBounce(t, a);
        case InOutBounce:
            return __mgeff_easeInOutBounce(t, a);
        case OutInBounce:
            return __mgeff_easeOutInBounce(t, a);
        default:
            return t;
    }
}

float __mgeffmotioncurve_calcvalue(EffMotionCurve* c, float t)
{
    if (c->curve_cb) {
        return c->curve_cb(t);
    }
    if (((EffMotionCurveEx*)c)->curveex_cb) {
        return ((EffMotionCurveEx*)c)->curveex_cb(c, t);
    }
    return t;
}

MGEFF_MOTIONCURVE_CB __mgeffmotioncurve_getfunc(int type)
{
    switch(type) {
    case Linear:
        return &__mgeff_easeNone;
    case InQuad:
        return &__mgeff_easeInQuad;
    case OutQuad:
        return &__mgeff_easeOutQuad;
    case InOutQuad:
        return &__mgeff_easeInOutQuad;
    case OutInQuad:
        return &__mgeff_easeOutInQuad;
    case InCubic:
        return &__mgeff_easeInCubic;
    case OutCubic:
        return &__mgeff_easeOutCubic;
    case InOutCubic:
        return &__mgeff_easeInOutCubic;
    case OutInCubic:
        return &__mgeff_easeOutInCubic;
    case InQuart:
        return &__mgeff_easeInQuart;
    case OutQuart:
        return &__mgeff_easeOutQuart;
    case InOutQuart:
        return &__mgeff_easeInOutQuart;
    case OutInQuart:
        return &__mgeff_easeOutInQuart;
    case InQuint:
        return &__mgeff_easeInQuint;
    case OutQuint:
        return &__mgeff_easeOutQuint;
    case InOutQuint:
        return &__mgeff_easeInOutQuint;
    case OutInQuint:
        return &__mgeff_easeOutInQuint;
    case InSine:
        return &__mgeff_easeInSine;
    case OutSine:
        return &__mgeff_easeOutSine;
    case InOutSine:
        return &__mgeff_easeInOutSine;
    case OutInSine:
        return &__mgeff_easeOutInSine;
    case InExpo:
        return &__mgeff_easeInExpo;
    case OutExpo:
        return &__mgeff_easeOutExpo;
    case InOutExpo:
        return &__mgeff_easeInOutExpo;
    case OutInExpo:
        return &__mgeff_easeOutInExpo;
    case InCirc:
        return &__mgeff_easeInCirc;
    case OutCirc:
        return &__mgeff_easeOutCirc;
    case InOutCirc:
        return &__mgeff_easeInOutCirc;
    case OutInCirc:
        return &__mgeff_easeOutInCirc;
    case InCurve:
        return &__mgeff_easeInCurve;
    case OutCurve:
        return &__mgeff_easeOutCurve;
    case SineCurve:
        return &__mgeff_easeSineCurve;
    case CosineCurve:
        return &__mgeff_easeCosineCurve;
    default:
        return NULL;
    }
    return NULL;
}

MGEFF_MOTIONCURVEEX_CB __mgeffmotioncurve_getfuncex(int type)
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

