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
 *   <http://www.minigui.com/en/about/licensing-policy/>.
 */
#include <sys/times.h>
#include <math.h>
#include <gtest/gtest.h>
#include <mgeff/mgeff.h>
//#include "easing.h"

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

#define dbg() printf("%s %d\n", __FUNCTION__, __LINE__)

float step = 0;
float start = 0;
float end = 1000;
int duration = 1000;
double t0 = 0.0;

float calcInQuad(float t)
{
    return t*t;
} 
float calcOutQuad(float t)
{
    return -t*(t-2);
} 
float calcInOutQuad(float t)
{
    t*=2.0;
    if (t < 1) {
        return t*t/(float)(2);
    } else {
        --t;
        return -0.5 * (t*(t-2) - 1);
    }
} 
float calcOutInQuad(float t)
{
    if (t < 0.5) return calcOutQuad (t*2)/2;
    return calcInQuad((2*t)-1)/2 + 0.5;
}


float calcInCubic(float t)
{
    return t*t*t;
} 
float calcOutCubic(float t)
{
    t-=1.0;
    return t*t*t + 1;
} 
float calcInOutCubic(float t)
{
    t*=2.0;
    if(t < 1) {
        return 0.5*t*t*t;
    } else {
        t -= (float)(2.0);
        return 0.5*(t*t*t + 2);
    }
} 
float calcOutInCubic(float t)
{
    if (t < 0.5) return calcOutCubic (2*t)/2;
    return calcInCubic(2*t - 1)/2 + 0.5;
}


float calcInQuart(float t)
{
    return t*t*t*t;
} 
float calcOutQuart(float t)
{
    t-= (float)1.0;
    return - (t*t*t*t- 1);
} 
float calcInOutQuart(float t)
{
    t*=2;
    if (t < 1) return 0.5*t*t*t*t;
    else {
        t -= 2.0f;
        return -0.5 * (t*t*t*t- 2);
    }
} 
float calcOutInQuart(float t)
{
    if (t < 0.5) return calcOutQuart (2*t)/2;
    return calcInQuart(2*t-1)/2 + 0.5;
}


float calcInQuint(float t)
{
    return t*t*t*t*t;
} 
float calcOutQuint(float t)
{
    t-=1.0;
    return t*t*t*t*t + 1;
} 
float calcInOutQuint(float t)
{
    t*=2.0;
    if (t < 1) return 0.5*t*t*t*t*t;
    else {
        t -= 2.0;
        return 0.5*(t*t*t*t*t + 2);
    }
} 
float calcOutInQuint(float t)
{
    if (t < 0.5) return calcOutQuint (2*t)/2;
    return calcInQuint(2*t - 1)/2 + 0.5;
}


float calcInSine(float t)
{
    return (t == 1.0) ? 1.0 : -cos(t * M_PI_2) + 1.0;
} 
float calcOutSine(float t)
{
    return sin(t* M_PI_2);
} 
float calcInOutSine(float t)
{
    return -0.5 * (cos(M_PI*t) - 1);
} 
float calcOutInSine(float t)
{
    if (t < 0.5) return calcOutSine (2*t)/2;
    return calcInSine(2*t - 1)/2 + 0.5;
}


float calcInExpo(float t)
{
    return (t==0 || t == 1.0) ? t : pow(2.0, 10 * (t - 1)) - (float)(0.001);
} 
float calcOutExpo(float t)
{
    return (t==1.0) ? 1.0 : 1.001 * (-pow(2.0f, -10 * t) + 1);
} 
float calcInOutExpo(float t)
{
    if (t==0.0) return (float)(0.0);
    if (t==1.0) return (float)(1.0);
    t*=2.0;
    if (t < 1) return 0.5 * pow((float)(2.0), 10 * (t - 1)) - 0.0005;
    return 0.5 * 1.0005 * (-pow((float)(2.0), -10 * (t - 1)) + 2);
} 
float calcOutInExpo(float t)
{
    if (t < 0.5) return calcOutExpo (2*t)/2;
    return calcInExpo(2*t - 1)/2 + 0.5;
}


float calcInCirc(float t)
{
    return -(sqrt(1 - t*t) - 1);
} 
float calcOutCirc(float t)
{
    t-= (float)(1.0);
    return sqrt(1 - t* t);
} 
float calcInOutCirc(float t)
{
    t*=(float)(2.0);
    if (t < 1) {
        return -0.5 * (sqrt(1 - t*t) - 1);
    } else {
        t -= (float)(2.0);
        return 0.5 * (sqrt(1 - t*t) + 1);
    }
} 
float calcOutInCirc(float t)
{
    if (t < 0.5) return calcOutCirc (2*t)/2;
    return calcInCirc(2*t - 1)/2 + 0.5;
}

float calcInElastic_helper(float t, float b, float c, float d, float a, float p)
{
    if (t==0) return b;
    float t_adj = (float)t / (float)d;
    if (t_adj==1) return b+c;

    float s;
    if(a < fabs(c)) {
        a = c;
        s = p / 4.0f;
    } else {
        s = p / (2 * M_PI) * asin(c / a);
    }

    t_adj -= 1.0f;
    return -(a*pow(2.0f,10*t_adj) * sin( (t_adj*d-s)*(2*M_PI)/p )) + b;
} 
float calcInElastic(float t, float a, float p)
{
    return calcInElastic_helper(t, 0, 1, 1, a, p);
}

float calcOutElastic_helper(float t, float b, float c, float d, float a, float p)
{
    if (t==0) return 0;
    if (t==1) return c;

    float s;
    if(a < c) {
        a = c;
        s = p / 4.0f;
    } else {
        s = p / (2 * M_PI) * asin(c / a);
    }

    return (a*pow(2.0f,-10*t) * sin( (t-s)*(2*M_PI)/p ) + c);
} 
float calcOutElastic(float t, float a, float p)
{
    return calcOutElastic_helper(t, 0, 1, 1, a, p);
} 
float calcInOutElastic(float t, float a, float p)
{
    if (t==0) return 0.0;
    t*=2.0;
    if (t==2) return 1.0;

    float s;
    if(a < 1.0) {
        a = 1.0;
        s = p / 4.0f;
    } else {
        s = p / (2 * M_PI) * asin(1.0 / a);
    }

    if (t < 1) return -.5*(a*pow(2.0f,10*(t-1)) * sin( (t-1-s)*(2*M_PI)/p ));
    return a*pow(2.0f,-10*(t-1)) * sin( (t-1-s)*(2*M_PI)/p )*.5 + 1.0;
} 
float calcOutInElastic(float t, float a, float p)
{
    if (t < 0.5) return calcOutElastic_helper(t*2, 0, 0.5, 1.0, a, p);
    return calcInElastic_helper(2*t - 1.0, 0.5, 0.5, 1.0, a, p);
}


float calcInBack(float t, float s)
{
    return t*t*((s+1)*t - s);
} 
float calcOutBack(float t, float s)
{
    t-= (float)(1.0);
    return t*t*((s+1)*t+ s) + 1;
} 
float calcInOutBack(float t, float s)
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
float calcOutInBack(float t, float s)
{
    if (t < 0.5) return calcOutBack (2*t, s)/2;
    return calcInBack(2*t - 1, s)/2 + 0.5;
}

float calcOutBounce_helper(float t, float c, float a)
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
float calcOutBounce(float t, float a)
{
    return calcOutBounce_helper(t, 1, a);
} 
float calcInBounce(float t, float a)
{
    return 1.0 - calcOutBounce_helper(1.0-t, 1.0, a);
} 
float calcInOutBounce(float t, float a)
{
    if (t < 0.5) return calcInBounce (2*t, a)/2;
    else return (t == 1.0) ? 1.0 : calcOutBounce (2*t - 1, a)/2 + 0.5;
} 
float calcOutInBounce(float t, float a)
{
    if (t < 0.5) return calcOutBounce_helper(t*2, 0.5, a);
    return 1.0 - calcOutBounce_helper (2.0-2*t, 0.5, a);
}

inline float qt_sinProgress(float value)
{
    return sin((value * M_PI) - M_PI_2) / 2 + (float)(0.5);
} 
inline float qt_smoothBeginEndMixFactor(float value)
{
    return MIN(MAX(1 - value * 2 + (float)(0.3), (float)(0.0)), (float)(1.0));
}

// SmoothBegin blends Smooth and Linear Interpolation.
// Progress 0 - 0.3      : Smooth only
// Progress 0.3 - ~ 0.5  : Mix of Smooth and Linear
// Progress ~ 0.5  - 1   : Linear only
float calcInCurve(float t)
{
    const float sinProgress = qt_sinProgress(t);
    const float mix = qt_smoothBeginEndMixFactor(t);
    return sinProgress * mix + t * (1 - mix);
} 
float calcOutCurve(float t)
{
    const float sinProgress = qt_sinProgress(t);
    const float mix = qt_smoothBeginEndMixFactor(1 - t);
    return sinProgress * mix + t * (1 - mix);
} 
float calcSineCurve(float t)
{
    return (sin(((t * M_PI * 2)) - M_PI_2) + 1) / 2;
} 
float calcCosineCurve(float t)
{
    return (cos(((t * M_PI * 2)) - M_PI_2) + 1) / 2;
}

float calcCustom(float t)
{
    return t*t*t/3.0 + t*t/3.0 + t/3.0;
}

float curve_calc(int type, float x)
{
    switch(type) {
    case Linear:
        return x;

    case InQuad:
        return x*x;
    case OutQuad:
        return calcOutQuad(x);
    case InOutQuad:
        return calcInOutQuad(x);
    case OutInQuad:
        return calcOutInQuad(x);

    case InCubic:
        return calcInCubic(x);
    case OutCubic:
        return calcOutCubic(x);
    case InOutCubic:
        return calcInOutCubic(x);
    case OutInCubic:
        return calcOutInCubic(x);

    case InQuart:
        return calcInQuart(x);
    case OutQuart:
        return calcOutQuart(x);
    case InOutQuart:
        return calcInOutQuart(x);
    case OutInQuart:
        return calcOutInQuart(x);

    case InQuint:
        return calcInQuint(x);
    case OutQuint:
        return calcOutQuint(x);
    case InOutQuint:
        return calcInOutQuint(x);
    case OutInQuint:
        return calcOutInQuint(x);

    case InSine:
        return calcInSine(x);
    case OutSine:
        return calcOutSine(x);
    case InOutSine:
        return calcInOutSine(x);
    case OutInSine:
        return calcOutInSine(x);

    case InExpo:
        return calcInExpo(x);
    case OutExpo:
        return calcOutExpo(x);
    case InOutExpo:
        return calcInOutExpo(x);
    case OutInExpo:
        return calcOutInExpo(x);

    case InCirc:
        return calcInCirc(x);
    case OutCirc:
        return calcOutCirc(x);
    case InOutCirc:
        return calcInOutCirc(x);
    case OutInCirc:
        return calcOutInCirc(x);

    case InCurve:
        return calcInCurve(x);
    case OutCurve:
        return calcOutCurve(x);
    case SineCurve:
        return calcSineCurve(x);
    case CosineCurve:
        return calcCosineCurve(x);

    case InElastic:
        return calcInElastic(x, 1.0, 0.3);
    case OutElastic:
        return calcOutElastic(x, 1.0, 0.3);
    case InOutElastic:
        return calcInOutElastic(x, 1.0, 0.3);
    case OutInElastic:
        return calcOutInElastic(x, 1.0, 0.3); 

    case InBack:
        return calcInBack(x, 1.70158f);
    case OutBack:
        return calcOutBack(x, 1.70158f);
    case InOutBack:
        return calcInOutBack(x, 1.70158f);
    case OutInBack:
        return calcOutInBack(x, 1.70158f);

    case InBounce:
        return calcInBounce(x, 1.0);
    case OutBounce:
        return calcOutBounce(x, 1.0);
    case InOutBounce:
        return calcInOutBounce(x, 1.0);
    case OutInBounce:
        return calcOutInBounce(x, 1.0);

    case Custom:
        return calcCustom(x);

    default:
        return 0;
    }
    return 0;
}

void test_cb(MGEFF_ANIMATION handle, void* target, int id, void *value) { 
    double t = 10 * times(NULL);
    if(t - t0 > duration)
        t = t0 + duration;
    float y0 = *(float*)value;
    float x = (t - t0) / duration;
    float y = duration * curve_calc(id, x);
    if(fabs(y - y0) >= 0.001)
        printf("x = %f,y = %f, value = %f\n", x, y, y0);
    ASSERT_TRUE(fabs(y - y0) < 0.001);
    return;
}
void test_curve(enum EffMotionType type) {
    ASSERT_TRUE(mGEffInit() == 0);
    MGEFF_ANIMATION animation = mGEffAnimationCreate(NULL, test_cb, type, MGEFF_FLOAT);
    ASSERT_TRUE(animation); 
    mGEffAnimationSetStartValue(animation, &start);
    mGEffAnimationSetEndValue(animation, &end);
    mGEffAnimationSetDuration(animation, duration);
    mGEffAnimationSetCurve(animation, type); 
    t0 = 10 * times(NULL);
    mGEffAnimationSyncRun(animation);
    mGEffAnimationDelete(animation);
    mGEffDeinit();
}

//linear
TEST(MotioncurveTest, Linear) {
    test_curve(Linear);
} 

//Quad
TEST(MotioncurveTest, InQuad) {
    test_curve(InQuad); 
} 
TEST(MotioncurveTest, OutQuad) {
    test_curve(OutQuad);
}
TEST(MotioncurveTest, InOutQuad) {
    test_curve(InOutQuad); 
} 
TEST(MotioncurveTest, OutInQuad) {
    test_curve(OutInQuad);
}

//cubic
TEST(MotioncurveTest, InCubic) {
    test_curve(InCubic); 
} 
TEST(MotioncurveTest, OutCubic) {
    test_curve(OutCubic);
}
TEST(MotioncurveTest, InOutCubic) {
    test_curve(InOutCubic); 
} 
TEST(MotioncurveTest, OutInCubic) {
    test_curve(OutInCubic);
}

//Quart 
TEST(MotioncurveTest, InQuart) {
    test_curve(InQuart); 
} 
TEST(MotioncurveTest, OutQuart) {
    test_curve(OutQuart);
}
TEST(MotioncurveTest, InOutQuart) {
    test_curve(InOutQuart); 
} 
TEST(MotioncurveTest, OutInQuart) {
    test_curve(OutInQuart);
}

//Quint 
TEST(MotioncurveTest, InQuint) {
    test_curve(InQuint); 
} 
TEST(MotioncurveTest, OutQuint) {
    test_curve(OutQuint);
}
TEST(MotioncurveTest, InOutQuint) {
    test_curve(InOutQuint); 
} 
TEST(MotioncurveTest, OutInQuint) {
    test_curve(OutInQuint);
}

//Sine
TEST(MotioncurveTest, InSine) {
    test_curve(InSine); 
} 
TEST(MotioncurveTest, OutSine) {
    test_curve(OutSine);
}
TEST(MotioncurveTest, InOutSine) {
    test_curve(InOutSine); 
} 
TEST(MotioncurveTest, OutInSine) {
    test_curve(OutInSine);
}

//Expo
TEST(MotioncurveTest, InExpo) {
    test_curve(InExpo); 
} 
TEST(MotioncurveTest, OutExpo) {
    test_curve(OutExpo);
}
TEST(MotioncurveTest, InOutExpo) {
    test_curve(InOutExpo); 
} 
TEST(MotioncurveTest, OutInExpo) {
    test_curve(OutInExpo);
}

//Circ
TEST(MotioncurveTest, InCirc) {
    test_curve(InCirc); 
} 
TEST(MotioncurveTest, OutCirc) {
    test_curve(OutCirc);
}
TEST(MotioncurveTest, InOutCirc) {
    test_curve(InOutCirc); 
} 
TEST(MotioncurveTest, OutInCirc) {
    test_curve(OutInCirc);
}
//Elastic
void test_elastic(enum EffMotionType type) {
    ASSERT_TRUE(mGEffInit() == 0);
    MGEFF_ANIMATION animation = mGEffAnimationCreate(NULL, test_cb, type, MGEFF_FLOAT);
    ASSERT_TRUE(animation); 
    mGEffAnimationSetStartValue(animation, &start);
    mGEffAnimationSetEndValue(animation, &end);
    mGEffAnimationSetDuration(animation, duration);
    mGEffAnimationSetCurve(animation, type); 
    mGEffCurveSetProperty(mGEffAnimationGetCurve(animation), MGEFFCURVE_PROP_PERIOD, 0.3f);
    mGEffCurveSetProperty(mGEffAnimationGetCurve(animation), MGEFFCURVE_PROP_AMPLITUDE, 1.0f);
    t0 = 10 * times(NULL);
    mGEffAnimationSyncRun(animation);
    mGEffAnimationDelete(animation);
    mGEffDeinit();
}
TEST(MotioncurveTest, InElastic) {
    test_elastic(InElastic); 
} 
TEST(MotioncurveTest, OutElastic) {
    test_elastic(OutElastic);
}
TEST(MotioncurveTest, InOutElastic) {
    test_elastic(InOutElastic); 
} 
TEST(MotioncurveTest, OutInElastic) {
    test_elastic(OutInElastic);
} 

void test_back(enum EffMotionType type) {
    ASSERT_TRUE(mGEffInit() == 0);
    MGEFF_ANIMATION animation = mGEffAnimationCreate(NULL, test_cb, type, MGEFF_FLOAT);
    ASSERT_TRUE(animation); 
    mGEffAnimationSetStartValue(animation, &start);
    mGEffAnimationSetEndValue(animation, &end);
    mGEffAnimationSetDuration(animation, duration);
    mGEffAnimationSetCurve(animation, type); 
    mGEffCurveSetProperty(mGEffAnimationGetCurve(animation), MGEFFCURVE_PROP_PERIOD, 1.70158f);
    t0 = 10 * times(NULL);
    mGEffAnimationSyncRun(animation);
    mGEffAnimationDelete(animation);
    mGEffDeinit();
}
//Back
TEST(MotioncurveTest, InBack) {
    test_back(InBack); 
} 
TEST(MotioncurveTest, OutBack) {
    test_back(OutBack);
}
TEST(MotioncurveTest, InOutBack) {
    test_back(InOutBack); 
} 
TEST(MotioncurveTest, OutInBack) {
    test_back(OutInBack);
}

void test_bounce(enum EffMotionType type) {
    ASSERT_TRUE(mGEffInit() == 0);
    MGEFF_ANIMATION animation = mGEffAnimationCreate(NULL, test_cb, type, MGEFF_FLOAT);
    ASSERT_TRUE(animation); 
    mGEffAnimationSetStartValue(animation, &start);
    mGEffAnimationSetEndValue(animation, &end);
    mGEffAnimationSetDuration(animation, duration);
    mGEffAnimationSetCurve(animation, type); 
    mGEffCurveSetProperty(mGEffAnimationGetCurve(animation), MGEFFCURVE_PROP_AMPLITUDE, 1.0f);
    t0 = 10 * times(NULL);
    mGEffAnimationSyncRun(animation);
    mGEffAnimationDelete(animation);
    mGEffDeinit();
}
//Bounce
TEST(MotioncurveTest, InBounce) {
    test_bounce(InBounce); 
} 
TEST(MotioncurveTest, OutBounce) {
    test_bounce(OutBounce);
}
TEST(MotioncurveTest, InOutBounce) {
    test_bounce(InOutBounce); 
} 
TEST(MotioncurveTest, OutInBounce) {
    test_bounce(OutInBounce);
}

//Curve
TEST(MotioncurveTest, InCurve) {
    test_curve(InCurve); 
} 
TEST(MotioncurveTest, OutCurve) {
    test_curve(OutCurve);
}
TEST(MotioncurveTest, SineCurve) {
    test_curve(SineCurve); 
} 
TEST(MotioncurveTest, CosineCurve) {
    test_curve(CosineCurve);
}

//Custom
TEST(MotioncurveTest, Custom) {
    ASSERT_TRUE(mGEffInit() == 0);
    MGEFF_ANIMATION animation = mGEffAnimationCreate(NULL, test_cb, Custom, MGEFF_FLOAT);
    ASSERT_TRUE(animation); 
    mGEffAnimationSetStartValue(animation, &start);
    mGEffAnimationSetEndValue(animation, &end);
    mGEffAnimationSetDuration(animation, duration);
    mGEffAnimationSetCurveEx(animation, &calcCustom);
    t0 = 10 * times(NULL);
    mGEffAnimationSyncRun(animation);
    mGEffAnimationDelete(animation);
    mGEffDeinit();
}

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
