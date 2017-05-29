/*! ============================================================================
 * \file animation-group.h 
 * \brief Head file for animation group. 
 * \author DongKai
 * \version 1.0
 * \date 02/08/2010 10:08:16 AM 
 *
 *  Company: Beijing Feynman Software Technology Co., Ltd.
 */
#ifndef __ANIMATION_GROUP_H
#define __ANIMATION_GROUP_H

#include "animation.h"

typedef struct _EffAnimationGroup {
    EffAnimation animation;
    struct list_head animation_list;
} EffAnimationGroup;

#endif
