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
