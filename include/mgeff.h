/*!============================================================================
 * \file mgeff.h
 * \brief The interface relative headfile.
 * \author WanZheng
 * \version 1.0
 * \date 02/03/2010 04:48:51 PM
 *
 *  Company: Beijing Feynman Software Technology Co., Ltd.
 */
#ifndef _MGEFF_H_
#define _MGEFF_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "mgeff-common.h"
#include "mgeff-animation.h"
#include "mgeff-animationgroup.h"
#include "mgeff-motioncurve.h"
#ifdef _MGEFF_EFFECTOR
    #include "mgeff-effector-sink.h"
    #include "mgeff-effector-source.h"
    #include "mgeff-effector.h"
    #include "mgeff-window-animation.h"
#endif

#ifdef __cplusplus
}
#endif

#endif

