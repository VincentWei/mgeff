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

#ifndef __EASING_H__
#define __EASING_H__

typedef float effreal;

effreal __mgeff_easeNone(effreal progress);

/**
 * Easing equation function for a quadratic (t^2) easing in:
 * accelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
effreal __mgeff_easeInQuad(effreal t);


/**
 * Easing equation function for a quadratic (t^2) easing out:
 * decelerating to zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
effreal __mgeff_easeOutQuad(effreal t);


/**
 * Easing equation function for a quadratic (t^2) easing in/out:
 * acceleration until halfway, then deceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
effreal __mgeff_easeInOutQuad(effreal t);


/**
 * Easing equation function for a quadratic (t^2) easing out/in:
 * deceleration until halfway, then acceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
effreal __mgeff_easeOutInQuad(effreal t);


/**
 * Easing equation function for a cubic (t^3) easing in:
 * accelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
effreal __mgeff_easeInCubic(effreal t);


/**
 * Easing equation function for a cubic (t^3) easing out:
 * decelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
effreal __mgeff_easeOutCubic(effreal t);


/**
 * Easing equation function for a cubic (t^3) easing in/out:
 * acceleration until halfway, then deceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
effreal __mgeff_easeInOutCubic(effreal t);


/**
 * Easing equation function for a cubic (t^3) easing out/in:
 * deceleration until halfway, then acceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
effreal __mgeff_easeOutInCubic(effreal t);


/**
 * Easing equation function for a quartic (t^4) easing in:
 * accelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
effreal __mgeff_easeInQuart(effreal t);


/**
 * Easing equation function for a quartic (t^4) easing out:
 * decelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
effreal __mgeff_easeOutQuart(effreal t);


/**
 * Easing equation function for a quartic (t^4) easing in/out:
 * acceleration until halfway, then deceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
effreal __mgeff_easeInOutQuart(effreal t);


/**
 * Easing equation function for a quartic (t^4) easing out/in:
 * deceleration until halfway, then acceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
effreal __mgeff_easeOutInQuart(effreal t);


/**
 * Easing equation function for a quintic (t^5) easing in:
 * accelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
effreal __mgeff_easeInQuint(effreal t);


/**
 * Easing equation function for a quintic (t^5) easing out:
 * decelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
effreal __mgeff_easeOutQuint(effreal t);


/**
 * Easing equation function for a quintic (t^5) easing in/out:
 * acceleration until halfway, then deceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
effreal __mgeff_easeInOutQuint(effreal t);


/**
 * Easing equation function for a quintic (t^5) easing out/in:
 * deceleration until halfway, then acceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
effreal __mgeff_easeOutInQuint(effreal t);


/**
 * Easing equation function for a sinusoidal (sin(t)) easing in:
 * accelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
effreal __mgeff_easeInSine(effreal t);


/**
 * Easing equation function for a sinusoidal (sin(t)) easing out:
 * decelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
effreal __mgeff_easeOutSine(effreal t);


/**
 * Easing equation function for a sinusoidal (sin(t)) easing in/out:
 * acceleration until halfway, then deceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
effreal __mgeff_easeInOutSine(effreal t);


/**
 * Easing equation function for a sinusoidal (sin(t)) easing out/in:
 * deceleration until halfway, then acceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
effreal __mgeff_easeOutInSine(effreal t);


/**
 * Easing equation function for an exponential (2^t) easing in:
 * accelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
effreal __mgeff_easeInExpo(effreal t);


/**
 * Easing equation function for an exponential (2^t) easing out:
 * decelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
effreal __mgeff_easeOutExpo(effreal t);


/**
 * Easing equation function for an exponential (2^t) easing in/out:
 * acceleration until halfway, then deceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
effreal __mgeff_easeInOutExpo(effreal t);


/**
 * Easing equation function for an exponential (2^t) easing out/in:
 * deceleration until halfway, then acceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
effreal __mgeff_easeOutInExpo(effreal t);


/**
 * Easing equation function for a circular (sqrt(1-t^2)) easing in:
 * accelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
effreal __mgeff_easeInCirc(effreal t);


/**
 * Easing equation function for a circular (sqrt(1-t^2)) easing out:
 * decelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
effreal __mgeff_easeOutCirc(effreal t);


/**
 * Easing equation function for a circular (sqrt(1-t^2)) easing in/out:
 * acceleration until halfway, then deceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
effreal __mgeff_easeInOutCirc(effreal t);


/**
 * Easing equation function for a circular (sqrt(1-t^2)) easing out/in:
 * deceleration until halfway, then acceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
effreal __mgeff_easeOutInCirc(effreal t);


effreal __mgeff_easeInElastic_helper(effreal t, effreal b, effreal c, effreal d, effreal a, effreal p);

/**
 * Easing equation function for an elastic (exponentially decaying
 * sine wave) easing in: accelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @param a		Amplitude.
 * @param p		Period.
 * @return		The correct value.
 */
effreal __mgeff_easeInElastic(effreal t, effreal a, effreal p);


effreal __mgeff_easeOutElastic_helper(effreal t, effreal b, effreal c, effreal d, effreal a, effreal p);


/**
 * Easing equation function for an elastic (exponentially decaying
 * sine wave) easing out: decelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @param a		Amplitude.
 * @param p		Period.
 * @return		The correct value.
 */
effreal __mgeff_easeOutElastic(effreal t, effreal a, effreal p);


/**
 * Easing equation function for an elastic (exponentially decaying
 * sine wave) easing in/out: acceleration until halfway, then deceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @param a		Amplitude.
 * @param p		Period.
 * @return		The correct value.
 */
effreal __mgeff_easeInOutElastic(effreal t, effreal a, effreal p);


/**
 * Easing equation function for an elastic (exponentially decaying
 * sine wave) easing out/in: deceleration until halfway, then acceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @param a		Amplitude.
 * @param p		Period.
 * @return		The correct value.
 */
effreal __mgeff_easeOutInElastic(effreal t, effreal a, effreal p);


/**
 * Easing equation function for a back (overshooting cubic easing:
 * (s+1)*t^3 - s*t^2) easing in: accelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @param s		Overshoot ammount: higher s means greater overshoot
 *              (0 produces cubic easing with no overshoot, and the
 *              default value of 1.70158 produces an overshoot of 10 percent).
 * @return		The correct value.
 */
effreal __mgeff_easeInBack(effreal t, effreal s);


/**
 * Easing equation function for a back (overshooting cubic easing:
 * (s+1)*t^3 - s*t^2) easing out: decelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @param s		Overshoot ammount: higher s means greater overshoot
 *              (0 produces cubic easing with no overshoot, and the
 *              default value of 1.70158 produces an overshoot of 10 percent).
 * @return		The correct value.
 */
effreal __mgeff_easeOutBack(effreal t, effreal s);


/**
 * Easing equation function for a back (overshooting cubic easing:
 * (s+1)*t^3 - s*t^2) easing in/out: acceleration until halfway, then deceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @param s		Overshoot ammount: higher s means greater overshoot
 *              (0 produces cubic easing with no overshoot, and the
 *              default value of 1.70158 produces an overshoot of 10 percent).
 * @return		The correct value.
 */
effreal __mgeff_easeInOutBack(effreal t, effreal s);


/**
 * Easing equation function for a back (overshooting cubic easing:
 * (s+1)*t^3 - s*t^2) easing out/in: deceleration until halfway, then acceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @param s		Overshoot ammount: higher s means greater overshoot
 *              (0 produces cubic easing with no overshoot, and the
 *              default value of 1.70158 produces an overshoot of 10 percent).
 * @return		The correct value.
 */
effreal __mgeff_easeOutInBack(effreal t, effreal s);


effreal __mgeff_easeOutBounce_helper(effreal t, effreal c, effreal a);


/**
 * Easing equation function for a bounce (exponentially decaying
 * parabolic bounce) easing out: decelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @param a		Amplitude.
 * @return		The correct value.
 */
effreal __mgeff_easeOutBounce(effreal t, effreal a);


/**
 * Easing equation function for a bounce (exponentially decaying
 * parabolic bounce) easing in: accelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @param a		Amplitude.
 * @return		The correct value.
 */
effreal __mgeff_easeInBounce(effreal t, effreal a);



/**
 * Easing equation function for a bounce (exponentially decaying
 * parabolic bounce) easing in/out: acceleration until halfway, then deceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @param a		Amplitude.
 * @return		The correct value.
 */
effreal __mgeff_easeInOutBounce(effreal t, effreal a);


/**
 * Easing equation function for a bounce (exponentially decaying
 * parabolic bounce) easing out/in: deceleration until halfway, then acceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @param a		Amplitude.
 * @return		The correct value.
 */
effreal __mgeff_easeOutInBounce(effreal t, effreal a);


effreal __mgeff_sinProgress(effreal value);


effreal __mgeff_smoothBeginEndMixFactor(effreal value);


/** 
 * SmoothBegin blends Smooth and Linear Interpolation.
 * Progress 0 - 0.3      : Smooth only
 * Progress 0.3 - ~ 0.5  : Mix of Smooth and Linear
 * Progress ~ 0.5  - 1   : Linear only
 */

/**
 * Easing function that starts growing slowly, then increases in speed.
 * At the end of the curve the speed will be constant.
 */
effreal __mgeff_easeInCurve(effreal t);


/**
 * Easing function that starts growing steadily, then ends slowly.
 * The speed will be constant at the beginning of the curve.
 */
effreal __mgeff_easeOutCurve(effreal t);


/**
 * Easing function where the value grows sinusoidally.
 * Note that the calculated  end value will be 0 rather than 1.
 */
effreal __mgeff_easeSineCurve(effreal t);


/**
 * Easing function where the value grows cosinusoidally.
 * Note that the calculated start value will be 0.5 and the end value will be 0.5
 * contrary to the usual 0 to 1 easing curve.
 */
effreal __mgeff_easeCosineCurve(effreal t);

#endif
