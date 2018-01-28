/*
Disclaimer for Robert Penner's Easing Equations license:

TERMS OF USE - EASING EQUATIONS

Open source under the BSD License.

Copyright © 2001 Robert Penner
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
    * Neither the name of the author nor the names of contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

qreal easeNone(qreal progress)
{
    return progress;
}


qreal easeInQuad(qreal t)
{
    return t*t;
}


qreal easeOutQuad(qreal t)
{
    return -t*(t-2);
}


qreal easeInOutQuad(qreal t)
{
    t*=2.0;
    if (t < 1) {
        return t*t/(qreal)(2);
    } else {
        --t;
        return -0.5 * (t*(t-2) - 1);
    }
}


qreal easeOutInQuad(qreal t)
{
    if (t < 0.5) return easeOutQuad (t*2)/2;
    return easeInQuad((2*t)-1)/2 + 0.5;
}


qreal easeInCubic(qreal t)
{
    return t*t*t;
}


qreal easeOutCubic(qreal t)
{
    t-=1.0;
    return t*t*t + 1;
}


qreal easeInOutCubic(qreal t)
{
    t*=2.0;
    if(t < 1) {
        return 0.5*t*t*t;
    } else {
        t -= (qreal)(2.0);
        return 0.5*(t*t*t + 2);
    }
}


qreal easeOutInCubic(qreal t)
{
    if (t < 0.5) return easeOutCubic (2*t)/2;
    return easeInCubic(2*t - 1)/2 + 0.5;
}


qreal easeInQuart(qreal t)
{
    return t*t*t*t;
}


qreal easeOutQuart(qreal t)
{
    t-= (qreal)1.0;
    return - (t*t*t*t- 1);
}


qreal easeInOutQuart(qreal t)
{
    t*=2;
    if (t < 1) return 0.5*t*t*t*t;
    else {
        t -= 2.0f;
        return -0.5 * (t*t*t*t- 2);
    }
}


qreal easeOutInQuart(qreal t)
{
    if (t < 0.5) return easeOutQuart (2*t)/2;
    return easeInQuart(2*t-1)/2 + 0.5;
}


qreal easeInQuint(qreal t)
{
    return t*t*t*t*t;
}


qreal easeOutQuint(qreal t)
{
    t-=1.0;
    return t*t*t*t*t + 1;
}


qreal easeInOutQuint(qreal t)
{
    t*=2.0;
    if (t < 1) return 0.5*t*t*t*t*t;
    else {
        t -= 2.0;
        return 0.5*(t*t*t*t*t + 2);
    }
}


qreal easeOutInQuint(qreal t)
{
    if (t < 0.5) return easeOutQuint (2*t)/2;
    return easeInQuint(2*t - 1)/2 + 0.5;
}


qreal easeInSine(qreal t)
{
#if 1
    return (t == 1.0) ? 1.0 : -cos(t * M_PI_2) + 1.0;
#else
    return 0.0;
#endif
}


qreal easeOutSine(qreal t)
{
#if 1
    return sin(t* M_PI_2);
#else
    return 0.0;
#endif
}


qreal easeInOutSine(qreal t)
{
#if 1
    return -0.5 * (cos(M_PI*t) - 1);
#else
    return 0.0;
#endif
}


qreal easeOutInSine(qreal t)
{
    if (t < 0.5) return easeOutSine (2*t)/2;
    return easeInSine(2*t - 1)/2 + 0.5;
}


qreal easeInExpo(qreal t)
{
#if 1
    return (t==0 || t == 1.0) ? t : pow(2.0, 10 * (t - 1)) - (qreal)(0.001);
#else
    return 0.0;
#endif
}


qreal easeOutExpo(qreal t)
{
#if 1
    return (t==1.0) ? 1.0 : 1.001 * (-pow(2.0f, -10 * t) + 1);
#else
    return 0.0;
#endif
}


qreal easeInOutExpo(qreal t)
{
#if 1
    if (t==0.0) return (qreal)(0.0);
    if (t==1.0) return (qreal)(1.0);
    t*=2.0;
    if (t < 1) return 0.5 * pow((qreal)(2.0), 10 * (t - 1)) - 0.0005;
    return 0.5 * 1.0005 * (-pow((qreal)(2.0), -10 * (t - 1)) + 2);
#else
    return 0.0;
#endif
}


qreal easeOutInExpo(qreal t)
{
    if (t < 0.5) return easeOutExpo (2*t)/2;
    return easeInExpo(2*t - 1)/2 + 0.5;
}


qreal easeInCirc(qreal t)
{
    return -(sqrt(1 - t*t) - 1);
}


qreal easeOutCirc(qreal t)
{
    t-= (qreal)(1.0);
    return sqrt(1 - t* t);
}


qreal easeInOutCirc(qreal t)
{
    t*=(qreal)(2.0);
    if (t < 1) {
        return -0.5 * (sqrt(1 - t*t) - 1);
    } else {
        t -= (qreal)(2.0);
        return 0.5 * (sqrt(1 - t*t) + 1);
    }
}


qreal easeOutInCirc(qreal t)
{
    if (t < 0.5) return easeOutCirc (2*t)/2;
    return easeInCirc(2*t - 1)/2 + 0.5;
}

qreal easeInElastic_helper(qreal t, qreal b, qreal c, qreal d, qreal a, qreal p)
{
#if 1
    qreal t_adj;
    qreal s;
    if (t==0) return b;
    t_adj = (qreal)t / (qreal)d;
    if (t_adj==1) return b+c;

    if(a < fabs(c)) {
        a = c;
        s = p / 4.0f;
    } else {
        s = p / (2 * M_PI) * asin(c / a);
    }

    t_adj -= 1.0f;
    return -(a*pow(2.0f,10*t_adj) * sin( (t_adj*d-s)*(2*M_PI)/p )) + b;
#else
    return 0.0;
#endif
}


qreal easeInElastic(qreal t, qreal a, qreal p)
{
    return easeInElastic_helper(t, 0, 1, 1, a, p);
}

qreal easeOutElastic_helper(qreal t, qreal b, qreal c, qreal d, qreal a, qreal p)
{
#if 1
    qreal s;
    if (t==0) return 0;
    if (t==1) return c;

    if(a < c) {
        a = c;
        s = p / 4.0f;
    } else {
        s = p / (2 * M_PI) * asin(c / a);
    }

    return (a*pow(2.0f,-10*t) * sin( (t-s)*(2*M_PI)/p ) + c);
#else
    return 0.0;
#endif
}


qreal easeOutElastic(qreal t, qreal a, qreal p)
{
    return easeOutElastic_helper(t, 0, 1, 1, a, p);
}


qreal easeInOutElastic(qreal t, qreal a, qreal p)
{
#if 1
    qreal s;
    if (t==0) return 0.0;
    t*=2.0;
    if (t==2) return 1.0;

    if(a < 1.0) {
        a = 1.0;
        s = p / 4.0f;
    } else {
        s = p / (2 * M_PI) * asin(1.0 / a);
    }

    if (t < 1) return -.5*(a*pow(2.0f,10*(t-1)) * sin( (t-1-s)*(2*M_PI)/p ));
    return a*pow(2.0f,-10*(t-1)) * sin( (t-1-s)*(2*M_PI)/p )*.5 + 1.0;
#else
    return 0.0;
#endif
}


qreal easeOutInElastic(qreal t, qreal a, qreal p)
{
    if (t < 0.5) return easeOutElastic_helper(t*2, 0, 0.5, 1.0, a, p);
    return easeInElastic_helper(2*t - 1.0, 0.5, 0.5, 1.0, a, p);
}


qreal easeInBack(qreal t, qreal s)
{
    return t*t*((s+1)*t - s);
}


qreal easeOutBack(qreal t, qreal s)
{
    t-= (qreal)(1.0);
    return t*t*((s+1)*t+ s) + 1;
}


qreal easeInOutBack(qreal t, qreal s)
{
    t *= 2.0;
    if (t < 1) {
        s *= 1.525f;
        return 0.5*(t*t*((s+1)*t - s));
    } else {
        t -= 2;
        s *= 1.525f;
        return 0.5*(t*t*((s+1)*t+ s) + 2);
    }
}


qreal easeOutInBack(qreal t, qreal s)
{
    if (t < 0.5) return easeOutBack (2*t, s)/2;
    return easeInBack(2*t - 1, s)/2 + 0.5;
}

qreal easeOutBounce_helper(qreal t, qreal c, qreal a)
{
    if (t == 1.0) return c;
    if (t < (4/11.0)) {
        return c*(7.5625*t*t);
    } else if (t < (8/11.0)) {
        t -= (6/11.0);
        return -a * (1. - (7.5625*t*t + .75)) + c;
    } else if (t < (10/11.0)) {
        t -= (9/11.0);
        return -a * (1. - (7.5625*t*t + .9375)) + c;
    } else {
        t -= (21/22.0);
        return -a * (1. - (7.5625*t*t + .984375)) + c;
    }
}


qreal easeOutBounce(qreal t, qreal a)
{
    return easeOutBounce_helper(t, 1, a);
}


qreal easeInBounce(qreal t, qreal a)
{
    return 1.0 - easeOutBounce_helper(1.0-t, 1.0, a);
}



qreal easeInOutBounce(qreal t, qreal a)
{
    if (t < 0.5) return easeInBounce (2*t, a)/2;
    else return (t == 1.0) ? 1.0 : easeOutBounce (2*t - 1, a)/2 + 0.5;
}


qreal easeOutInBounce(qreal t, qreal a)
{
    if (t < 0.5) return easeOutBounce_helper(t*2, 0.5, a);
    return 1.0 - easeOutBounce_helper (2.0-2*t, 0.5, a);
}

inline qreal qt_sinProgress(qreal value)
{
#if 1
    return sin((value * M_PI) - M_PI_2) / 2 + (qreal)(0.5);
#else
    return 0.0;
#endif
}

inline qreal qt_smoothBeginEndMixFactor(qreal value)
{
#if 1
    return MIN(MAX(1 - value * 2 + (qreal)(0.3), (qreal)(0.0)), (qreal)(1.0));
#else
    return 0.0;
#endif
}

/** 
 * SmoothBegin blends Smooth and Linear Interpolation.
 * Progress 0 - 0.3      : Smooth only
 * Progress 0.3 - ~ 0.5  : Mix of Smooth and Linear
 * Progress ~ 0.5  - 1   : Linear only
 */


qreal easeInCurve(qreal t)
{
    const qreal sinProgress = qt_sinProgress(t);
    const qreal mix = qt_smoothBeginEndMixFactor(t);
    return sinProgress * mix + t * (1 - mix);
}


qreal easeOutCurve(qreal t)
{
    const qreal sinProgress = qt_sinProgress(t);
    const qreal mix = qt_smoothBeginEndMixFactor(1 - t);
    return sinProgress * mix + t * (1 - mix);
}


qreal easeSineCurve(qreal t)
{
#if 1
    return (sin(((t * M_PI * 2)) - M_PI_2) + 1) / 2;
#else
    return 0.0;
#endif
}


qreal easeCosineCurve(qreal t)
{
#if 1
    return (cos(((t * M_PI * 2)) - M_PI_2) + 1) / 2;
#else
    return 0.0;
#endif
}

