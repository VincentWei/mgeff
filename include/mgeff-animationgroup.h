/*!============================================================================
 * \file mgeff-animationgroup.h
 * \brief animation group headfile.
 * \author DongKai
 * \version 1.0
 * \date 02/03/2010 05:22:39 PM
 *
 *  Company: Beijing Feynman Software Technology Co., Ltd.
 */
#ifndef _MGEFF_ANIMATION_GROUP_H_
#define _MGEFF_ANIMATION_GROUP_H_

/**
 * \fn MGEFF_EXPORT mGEffAnimationCreateGroup(enum EffAnimationType type)
 * \brief Create mgeff animation group.
 *
 * This function Creates an animation group according to the \a type..
 *
 * \param type Animation group type. It can be the following values:
 *
 *      -MGEFF_PARALLEL
 *      Parallel animation group: all animation in the group will run
 *      simultaneously.
 *
 *      -MGEFF_SEQUENTIAL
 *      Sequential animation group: all animation in the group will run one
 *      after another as when they are added to this animation group.
 *
 * \return the handle to the newly created animation group.
 *
 * \sa mGEffAnimationCreateEx   mGEffAnimationCreate.
 */
MGEFF_ANIMATION MGEFF_EXPORT mGEffAnimationCreateGroup(enum EffAnimationType type);

/**
 * \fn void MGEFF_EXPORT mGEffAnimationAddToGroup(MGEFF_ANIMATION group,
 *             MGEFF_ANIMATION animation)
 * \brief Adds an animation to an animation group.
 *
 * This function Adds an animation specified by \a animation to an existing
 * animation group specified by \a group.
 *
 * \param group animation group to which new animation will be added.
 * \param animation the animation to be added.
 *
 * \sa mGEffAnimationCreateGroup.
 */
void MGEFF_EXPORT mGEffAnimationAddToGroup(MGEFF_ANIMATION group,
        MGEFF_ANIMATION animation);

#endif

