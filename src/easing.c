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

#include <math.h>
#include "mgeff.h"
#include "easing.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#ifndef M_PI_2
#define M_PI_2 (M_PI / 2)
#endif

#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif
#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

effreal __mgeff_easeNone(effreal progress)
{
    return progress;
}


effreal __mgeff_easeInQuad(effreal t)
{
    return t*t;
}


effreal __mgeff_easeOutQuad(effreal t)
{
    return -t*(t-2);
}


effreal __mgeff_easeInOutQuad(effreal t)
{
    t*=2.0;
    if (t < 1) {
        return t*t/(effreal)(2);
    }

    --t;
    return -0.5 * (t*(t-2) - 1);
}


effreal __mgeff_easeOutInQuad(effreal t)
{
    if (t < 0.5)
        return __mgeff_easeOutQuad (t*2)/2;

    return __mgeff_easeInQuad((2*t)-1)/2 + 0.5;
}


effreal __mgeff_easeInCubic(effreal t)
{
    return t*t*t;
}


effreal __mgeff_easeOutCubic(effreal t)
{
    t-=1.0;
    return t*t*t + 1;
}


effreal __mgeff_easeInOutCubic(effreal t)
{
    t*=2.0;
    if(t < 1) {
        return 0.5*t*t*t;
    }

    t -= (effreal)(2.0);
    return 0.5*(t*t*t + 2);
}


effreal __mgeff_easeOutInCubic(effreal t)
{
    if (t < 0.5)
        return __mgeff_easeOutCubic (2*t)/2;

    return __mgeff_easeInCubic(2*t - 1)/2 + 0.5;
}


effreal __mgeff_easeInQuart(effreal t)
{
    return t*t*t*t;
}


effreal __mgeff_easeOutQuart(effreal t)
{
    t-= (effreal)1.0;
    return - (t*t*t*t- 1);
}


effreal __mgeff_easeInOutQuart(effreal t)
{
    t*=2;
    if (t < 1)
        return 0.5*t*t*t*t;

    t -= 2.0f;
    return -0.5 * (t*t*t*t- 2);
}


effreal __mgeff_easeOutInQuart(effreal t)
{
    if (t < 0.5)
        return __mgeff_easeOutQuart (2*t)/2;
    return __mgeff_easeInQuart(2*t-1)/2 + 0.5;
}


effreal __mgeff_easeInQuint(effreal t)
{
    return t*t*t*t*t;
}


effreal __mgeff_easeOutQuint(effreal t)
{
    t-=1.0;
    return t*t*t*t*t + 1;
}


effreal __mgeff_easeInOutQuint(effreal t)
{
    t*=2.0;
    if (t < 1)
        return 0.5*t*t*t*t*t;

    t -= 2.0;
    return 0.5*(t*t*t*t*t + 2);
}


effreal __mgeff_easeOutInQuint(effreal t)
{
    if (t < 0.5)
        return __mgeff_easeOutQuint (2*t)/2;
    return __mgeff_easeInQuint(2*t - 1)/2 + 0.5;
}


effreal __mgeff_easeInSine(effreal t)
{
    return (t == 1.0) ? 1.0 : -cos(t * M_PI_2) + 1.0;
}


effreal __mgeff_easeOutSine(effreal t)
{
    return sin(t* M_PI_2);
}


effreal __mgeff_easeInOutSine(effreal t)
{
    return -0.5 * (cos(M_PI*t) - 1);
}


effreal __mgeff_easeOutInSine(effreal t)
{
    if (t < 0.5)
        return __mgeff_easeOutSine (2*t)/2;

    return __mgeff_easeInSine(2*t - 1)/2 + 0.5;
}


effreal __mgeff_easeInExpo(effreal t)
{
    return (t==0 || t == 1.0) ? t : pow(2.0, 10 * (t - 1)) - (effreal)(0.001);
}


effreal __mgeff_easeOutExpo(effreal t)
{
    return (t==1.0) ? 1.0 : 1.001 * (-pow(2.0f, -10 * t) + 1);
}


effreal __mgeff_easeInOutExpo(effreal t)
{
    if (t==0.0)
        return (effreal)(0.0);

    if (t==1.0)
        return (effreal)(1.0);

    t*=2.0;
    if (t < 1)
        return 0.5 * pow((effreal)(2.0), 10 * (t - 1)) - 0.0005;

    return 0.5 * 1.0005 * (-pow((effreal)(2.0), -10 * (t - 1)) + 2);
}


effreal __mgeff_easeOutInExpo(effreal t)
{
    if (t < 0.5)
        return __mgeff_easeOutExpo (2*t)/2;
    return __mgeff_easeInExpo(2*t - 1)/2 + 0.5;
}


effreal __mgeff_easeInCirc(effreal t)
{
    return -(sqrt(1 - t*t) - 1);
}


effreal __mgeff_easeOutCirc(effreal t)
{
    t-= (effreal)(1.0);
    return sqrt(1 - t* t);
}


effreal __mgeff_easeInOutCirc(effreal t)
{
    t*=(effreal)(2.0);
    if (t < 1) {
        return -0.5 * (sqrt(1 - t*t) - 1);
    }
    else {
        t -= (effreal)(2.0);
        return 0.5 * (sqrt(1 - t*t) + 1);
    }
}


effreal __mgeff_easeOutInCirc(effreal t)
{
    if (t < 0.5)
        return __mgeff_easeOutCirc (2*t)/2;

    return __mgeff_easeInCirc(2*t - 1)/2 + 0.5;
}

effreal __mgeff_easeInElastic_helper(effreal t, effreal b, effreal c, effreal d, effreal a, effreal p)
{
    effreal t_adj;
    effreal s;

    if (t==0)
        return b;

    t_adj = (effreal)t / (effreal)d;
    if (t_adj==1)
        return b+c;

    if(a < fabs(c)) {
        a = c;
        s = p / 4.0f;
    }
    else {
        s = p / (2 * M_PI) * asin(c / a);
    }

    t_adj -= 1.0f;
    return -(a*pow(2.0f,10*t_adj) * sin( (t_adj*d-s)*(2*M_PI)/p )) + b;
}


effreal __mgeff_easeInElastic(effreal t, effreal a, effreal p)
{
    return __mgeff_easeInElastic_helper(t, 0, 1, 1, a, p);
}

effreal __mgeff_easeOutElastic_helper(effreal t, effreal b, effreal c, effreal d, effreal a, effreal p)
{
    effreal s;

    if (t==0)
        return 0;
    if (t==1)
        return c;

    if(a < c) {
        a = c;
        s = p / 4.0f;
    }
    else {
        s = p / (2 * M_PI) * asin(c / a);
    }

    return (a*pow(2.0f,-10*t) * sin( (t-s)*(2*M_PI)/p ) + c);
}


effreal __mgeff_easeOutElastic(effreal t, effreal a, effreal p)
{
    return __mgeff_easeOutElastic_helper(t, 0, 1, 1, a, p);
}


effreal __mgeff_easeInOutElastic(effreal t, effreal a, effreal p)
{
    effreal s;

    if (t==0)
        return 0.0;

    t*=2.0;
    if (t==2)
        return 1.0;

    if (a < 1.0) {
        a = 1.0;
        s = p / 4.0f;
    } else {
        s = p / (2 * M_PI) * asin(1.0 / a);
    }

    if (t < 1)
        return -.5*(a*pow(2.0f,10*(t-1)) * sin( (t-1-s)*(2*M_PI)/p ));

    return a*pow(2.0f,-10*(t-1)) * sin( (t-1-s)*(2*M_PI)/p )*.5 + 1.0;
}


effreal __mgeff_easeOutInElastic(effreal t, effreal a, effreal p)
{
    if (t < 0.5)
        return __mgeff_easeOutElastic_helper(t*2, 0, 0.5, 1.0, a, p);

    return __mgeff_easeInElastic_helper(2*t - 1.0, 0.5, 0.5, 1.0, a, p);
}


effreal __mgeff_easeInBack(effreal t, effreal s)
{
    return t*t*((s+1)*t - s);
}


effreal __mgeff_easeOutBack(effreal t, effreal s)
{
    t -= (effreal)(1.0);
    return t*t*((s+1)*t+ s) + 1;
}


effreal __mgeff_easeInOutBack(effreal t, effreal s)
{
    t *= 2.0;
    if (t < 1) {
        s *= 1.525f;
        return 0.5*(t*t*((s+1)*t - s));
    }

    t -= 2;
    s *= 1.525f;
    return 0.5*(t*t*((s+1)*t+ s) + 2);
}


effreal __mgeff_easeOutInBack(effreal t, effreal s)
{
    if (t < 0.5)
        return __mgeff_easeOutBack (2*t, s)/2;

    return __mgeff_easeInBack(2*t - 1, s)/2 + 0.5;
}

effreal __mgeff_easeOutBounce_helper(effreal t, effreal c, effreal a)
{
    if (t == 1.0)
        return c;

    if (t < (4/11.0)) {
        return c*(7.5625*t*t);
    }
    else if (t < (8/11.0)) {
        t -= (6/11.0);
        return -a * (1. - (7.5625*t*t + .75)) + c;
    }
    else if (t < (10/11.0)) {
        t -= (9/11.0);
        return -a * (1. - (7.5625*t*t + .9375)) + c;
    }

    t -= (21/22.0);
    return -a * (1. - (7.5625*t*t + .984375)) + c;
}


effreal __mgeff_easeOutBounce(effreal t, effreal a)
{
    return __mgeff_easeOutBounce_helper(t, 1, a);
}


effreal __mgeff_easeInBounce(effreal t, effreal a)
{
    return 1.0 - __mgeff_easeOutBounce_helper(1.0-t, 1.0, a);
}



effreal __mgeff_easeInOutBounce(effreal t, effreal a)
{
    if (t < 0.5)
        return __mgeff_easeInBounce (2*t, a)/2;

    return (t == 1.0) ? 1.0 : __mgeff_easeOutBounce (2*t - 1, a)/2 + 0.5;
}


effreal __mgeff_easeOutInBounce(effreal t, effreal a)
{
    if (t < 0.5)
        return __mgeff_easeOutBounce_helper(t*2, 0.5, a);

    return 1.0 - __mgeff_easeOutBounce_helper (2.0-2*t, 0.5, a);
}

inline effreal __mgeff_sinProgress(effreal value)
{
    return sin((value * M_PI) - M_PI_2) / 2 + (effreal)(0.5);
}

inline effreal __mgeff_smoothBeginEndMixFactor(effreal value)
{
    return MIN(MAX(1 - value * 2 + (effreal)(0.3), (effreal)(0.0)), (effreal)(1.0));
}

/** 
 * SmoothBegin blends Smooth and Linear Interpolation.
 * Progress 0 - 0.3      : Smooth only
 * Progress 0.3 - ~ 0.5  : Mix of Smooth and Linear
 * Progress ~ 0.5  - 1   : Linear only
 */


effreal __mgeff_easeInCurve(effreal t)
{
    const effreal sinProgress = __mgeff_sinProgress(t);
    const effreal mix = __mgeff_smoothBeginEndMixFactor(t);
    return sinProgress * mix + t * (1 - mix);
}


effreal __mgeff_easeOutCurve(effreal t)
{
    const effreal sinProgress = __mgeff_sinProgress(t);
    const effreal mix = __mgeff_smoothBeginEndMixFactor(1 - t);
    return sinProgress * mix + t * (1 - mix);
}


effreal __mgeff_easeSineCurve(effreal t)
{
    return (sin(((t * M_PI * 2)) - M_PI_2) + 1) / 2;
}


effreal __mgeff_easeCosineCurve(effreal t)
{
    return (cos(((t * M_PI * 2)) - M_PI_2) + 1) / 2;
}

