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
#include <mgeff/mgeff.h>
#include <mgeff/animation.h>

#include "animation_base.h"

#define MAXVAL(src1, src2) ((src1>src2)?src1:src2)
#define MINVAL(src1, src2) ((src1>src2)?src2:src1)

void finished_callback (MGEFF_ANIMATION handle)
{
//    int usetime;
//    ANIMAPARAM *animaparam = (ANIMAPARAM*)mGEffAnimationGetContext (handle);
    TESTPRJ *testprj = (TESTPRJ*)mGEffAnimationGetTarget (handle);

    gettimeofday (&testprj->times.finishtime, NULL);

    testprj->val.bfinished = TRUE;
#if 0
    EXPECT_EQ (*((int*)testprj->val.endval), *((int*)animaparam->endval));

    usetime = 1000 * (testprj->times.endtime.tv_sec - testprj->times.starttime.tv_sec) + \
                  (testprj->times.endtime.tv_usec - testprj->times.starttime.tv_usec)/1000;

    EXPECT_TRUE ((animaparam->duration <= usetime) && \
            ((animaparam->duration+ CHECK_PAUSE_STOPTIME)+70 > usetime)); /*enable 50ms mistake*/
#endif
}

void *varmalloc_callback (MGEFF_ANIMATION handle)
{
    TESTPRJ *testprj = (TESTPRJ*)mGEffAnimationGetTarget (handle);
    EffAnimation* anim;

    anim = (EffAnimation*)handle;

    testprj->val.bvarmalloc = TRUE;

    return (void *)malloc (anim->variantsize);
}

void calcvalue_callback (MGEFF_ANIMATION handle, void *s, void *e, void *v, float factor)
{
    TESTPRJ *testprj = (TESTPRJ*)mGEffAnimationGetTarget (handle);

    testprj->val.bcalcvalue = TRUE;

    *(int*)v = *(int*)s + (*(int*)e - *(int*)s) * factor;
}

void calcloopcount_callback(MGEFF_ANIMATION handle)
{
    TESTPRJ *testprj = (TESTPRJ*)mGEffAnimationGetTarget (handle);

    ++testprj->val.loopcount;
}

void directionchanged_callback(MGEFF_ANIMATION handle)
{
    TESTPRJ *testprj = (TESTPRJ*)mGEffAnimationGetTarget (handle);

    testprj->val.bdirchanged = TRUE;
}

void animation_callback (MGEFF_ANIMATION handle, void* target, int id, void* value)
{
//    TESTPRJ *testprj = (TESTPRJ *)mGEffAnimationGetTarget (handle);
    TESTPRJ *testprj = (TESTPRJ *)target;
    EffAnimation* anim;

    EXPECT_TRUE (testprj->val.bstopflag != TRUE);

    anim = (EffAnimation*)handle;

    testprj->val.bsetproperty = TRUE;

    /*public part*/
    if (mGEffAnimationGetProperty (handle, MGEFF_PROP_CURFRAME) == 0)
    {
        gettimeofday (&testprj->times.starttime, NULL);

        if (testprj->val.startval != NULL)
            memcpy (testprj->val.startval, value, anim->variantsize);
    }

    gettimeofday (&testprj->times.endtime, NULL);

    if (testprj->val.endval != NULL)
        memcpy (testprj->val.endval, value, anim->variantsize);
    ++testprj->val.fcount;
#if 0
    /*stand alone*/
    switch (id)
    {
        case MGEFF_RECT:
            {
                RECT *curval = (RECT*)value;
                RECT *endval = (RECT*)((ANIMAPARAM*)mGEffAnimationGetContext(handle))->endval;
                RECT *startval = (RECT*)((ANIMAPARAM*)mGEffAnimationGetContext(handle))->startval;
                if (mGEffAnimationGetProperty(handle, MGEFF_PROP_CURFRAME) == 1)
                {
                    EXPECT_TRUE (curval->left > MINVAL(endval->left,startval->left) && \
                            curval->top > MINVAL(startval->top, endval->top) && \
                            curval->right > MINVAL(startval->right, endval->right) && \
                            curval->bottom > MINVAL(startval->bottom, endval->bottom));
                    EXPECT_TRUE (curval->left < MINVAL(endval->left, endval->right) && \
                            curval->top < MAXVAL(endval->top, startval->top) && \
                            curval->right < MAXVAL(endval->right, startval->right) && \
                            curval->bottom < MAXVAL(endval->bottom, startval->bottom));
                }
            }
            break;
        case  MGEFF_FLOAT:
            {
                float *curval = (float*)value;
                float *endval = (float*)((ANIMAPARAM*)mGEffAnimationGetContext(handle))->endval;
                float *startval = (float*)((ANIMAPARAM*)mGEffAnimationGetContext(handle))->startval;
                if (mGEffAnimationGetProperty(handle, MGEFF_PROP_CURFRAME) == 2)
                {
                    EXPECT_TRUE (*curval > MINVAL(*startval, *endval) && *curval < MAXVAL(*endval, *startval));
                }
            }
            break;
        case MGEFF_DOUBLE:
            {
                double *curval = (double*)value;
                double *endval = (double*)((ANIMAPARAM*)mGEffAnimationGetContext(handle))->endval;
                double *startval = (double*)((ANIMAPARAM*)mGEffAnimationGetContext(handle))->startval;

                if ((mGEffAnimationGetProperty(handle, MGEFF_PROP_CURFRAME) == 2))
                {
                    EXPECT_TRUE (*curval > MINVAL(*startval, *endval) && *curval < MAXVAL(*endval, *startval));
                }
            }
            break;

        case MGEFF_POINT:
            {
                POINT *curval = (POINT*)value;
                POINT *endval = (POINT*)((ANIMAPARAM*)mGEffAnimationGetContext(handle))->endval;
                POINT *startval = (POINT*)((ANIMAPARAM*)mGEffAnimationGetContext(handle))->startval;

                if (mGEffAnimationGetProperty(handle, MGEFF_PROP_CURFRAME) == 3)
                {
                    EXPECT_TRUE (curval->x > MINVAL(endval->x, startval->x) && curval->y > MINVAL(endval->y,startval->y));
                    EXPECT_TRUE (curval->x < MAXVAL(endval->x, startval->x) && curval->y < MAXVAL(endval->y, startval->y));
                }
            }
            break;
        case MGEFF_POINTF:
            {
                EffPointF *curval = (EffPointF*)value;
                EffPointF *endval = (EffPointF*)((ANIMAPARAM*)mGEffAnimationGetContext(handle))->endval;
                EffPointF *startval = (EffPointF*)((ANIMAPARAM*)mGEffAnimationGetContext(handle))->startval;

                if (mGEffAnimationGetProperty(handle, MGEFF_PROP_CURFRAME) == 2)
                {
                    EXPECT_TRUE (curval->x > MINVAL(startval->x, endval->x) && curval->y > MINVAL(startval->y, endval->y));
                    EXPECT_TRUE (curval->x < MAXVAL(startval->x, endval->x) && curval->y < MAXVAL(startval->y, endval->y));
                }
            }
        case MGEFF_3DPOINT:
            {
                EffPoint3D *curval = (EffPoint3D*)value;
                EffPoint3D *endval = (EffPoint3D*)((ANIMAPARAM*)mGEffAnimationGetContext(handle))->endval;
                EffPoint3D *startval = (EffPoint3D*)((ANIMAPARAM*)mGEffAnimationGetContext(handle))->startval;

                if (mGEffAnimationGetProperty(handle, MGEFF_PROP_CURFRAME) == 3)
                {
                    EXPECT_TRUE (curval->x > MINVAL(endval->x,startval->x) && \
                            curval->y > MINVAL(endval->y, startval->y) && \
                            curval->z > MINVAL(endval->z,startval->z));
                    EXPECT_TRUE (curval->x < MAXVAL(startval->x, endval->x) && \
                            curval->y < MAXVAL(startval->y,endval->y) && \
                            curval->z < MAXVAL(startval->z, endval->z));
                }
            }
            break;
        case MGEFF_3DPOINTF:
            {
                EffPointF3D *curval = (EffPointF3D*)value;
                EffPointF3D *endval = (EffPointF3D*)((ANIMAPARAM*)mGEffAnimationGetContext(handle))->endval;
                EffPointF3D *startval = (EffPointF3D*)((ANIMAPARAM*)mGEffAnimationGetContext(handle))->startval;

                /*
                EffPointF3D *tmp = curval;
                printf ("endval: %8f, %8f, %8f, %d\n", tmp->x, tmp->y, tmp->z,mGEffAnimationGetProperty(handle, MGEFF_PROP_CURFRAME));
                */
                if (mGEffAnimationGetProperty(handle, MGEFF_PROP_CURFRAME) == 3)
                {

                    EXPECT_TRUE (curval->x > MINVAL(endval->x, startval->x) && \
                                 curval->y > MINVAL(endval->y, startval->y) && \
                                 curval->z > MINVAL(endval->z, startval->z));
                    EXPECT_TRUE (curval->x < MAXVAL(startval->x, endval->x) && \
                                 curval->y < MAXVAL(startval->y, endval->y) && \
                                 curval->z < MAXVAL(startval->z, endval->z));
                }
            }
            break;
        case MGEFF_INT:
            {
                int *curval = (int*)value;
                int *endval = (int*)((ANIMAPARAM*)mGEffAnimationGetContext(handle))->endval;
                int *startval = (int*)((ANIMAPARAM*)mGEffAnimationGetContext(handle))->startval;

                if (mGEffAnimationGetProperty(handle, MGEFF_PROP_CURFRAME) == 3)
                {
                    EXPECT_TRUE (*curval > MINVAL(*startval, *endval) && *curval < MAXVAL(*startval,*endval));
                }
            }
            break;
        default: 
            {
                printf ("unknow id, exit!\n");
                assert (0);
                break;
            }
    }
#endif
}

MGEFF_ANIMATION create_animation (void *target, ANIMAPARAM *anima_param, 
        enum EffVariantType varianttype, MGEFF_SETPROPERTY_CB setproperty_callback)
{
    MGEFF_ANIMATION animation_handle = NULL;
    animation_handle = mGEffAnimationCreate(target, setproperty_callback, varianttype, varianttype);

    mGEffAnimationSetStartValue (animation_handle, anima_param->startval);
    mGEffAnimationSetEndValue (animation_handle, anima_param->endval);

    if (anima_param->duration)
        mGEffAnimationSetProperty (animation_handle, MGEFF_PROP_DURATION, anima_param->duration);
    if (anima_param->loopcount)
        mGEffAnimationSetProperty (animation_handle, MGEFF_PROP_LOOPCOUNT, anima_param->loopcount);
    if (anima_param->direction)
        mGEffAnimationSetProperty (animation_handle, MGEFF_PROP_DIRECTION, anima_param->direction);
//    if (anima_param->fps)
//        mGEffAnimationSetProperty (animation_handle, MGEFF_PROP_FREQUENCY, anima_param->fps);

    mGEffAnimationSetFinishedCb (animation_handle, finished_callback);

    mGEffAnimationSetCurLoopChangedCb (animation_handle, calcloopcount_callback);
    mGEffAnimationSetDirChangedCb (animation_handle, directionchanged_callback);

    mGEffAnimationSetContext(animation_handle, anima_param);

    return animation_handle;
}

MGEFF_ANIMATION create_animation (void *target, ANIMAPARAM *anima_param,
        enum EffVariantType varianttype)
{
    return create_animation (target, anima_param, varianttype, animation_callback);
}

#if 0
void animationex_callback (MGEFF_ANIMATION handle, void *target, int id, void* value)
{
    TESTPRJ *testprj= (TESTPRJ*)target;
    EffAnimation* anim;

    anim = (EffAnimation*)handle;

    /*public part*/
    if (mGEffAnimationGetProperty (handle, MGEFF_PROP_CURFRAME) == 0)
    {
        gettimeofday (&testprj->times.starttime, NULL);
        if (testprj->val.startval != NULL)
            memcpy (testprj->val.startval, value, anim->variantsize);
    }

    gettimeofday (&testprj->times.endtime, NULL);

    if (testprj->val.endval != NULL)
        memcpy (testprj->val.endval, value, anim->variantsize);
    ++testprj->val.loopcount;
}
#endif
MGEFF_ANIMATION createex_animation (void *target, ANIMAPARAM *anima_param, 
        enum EffVariantType varianttype, int size,
        MGEFF_VARMALLOC_CB varmalloc, MGEFF_CALCVALUE_CB calcvalue,
        MGEFF_SETPROPERTY_CB setproperty_callback)
{
    MGEFF_ANIMATION animation_handle = NULL;

    animation_handle = mGEffAnimationCreateEx(target, setproperty_callback, varianttype,
           size, varmalloc, calcvalue);

    mGEffAnimationSetStartValue (animation_handle, anima_param->startval);
    mGEffAnimationSetEndValue (animation_handle, anima_param->endval);

    if (anima_param->duration)
        mGEffAnimationSetProperty (animation_handle, MGEFF_PROP_DURATION, anima_param->duration);
    if (anima_param->loopcount)
        mGEffAnimationSetProperty (animation_handle, MGEFF_PROP_LOOPCOUNT, anima_param->loopcount);
    if (anima_param->direction)
        mGEffAnimationSetProperty (animation_handle, MGEFF_PROP_DIRECTION, anima_param->direction);
//    if (anima_param->fps)
//        mGEffAnimationSetProperty (animation_handle, MGEFF_PROP_FREQUENCY, anima_param->fps);

    mGEffAnimationSetFinishedCb (animation_handle, finished_callback);
    mGEffAnimationSetDirChangedCb (animation_handle, directionchanged_callback);

    mGEffAnimationSetContext(animation_handle, anima_param);

    return animation_handle;
}

MGEFF_ANIMATION createex_animation (void *target, ANIMAPARAM *anima_param,
        enum EffVariantType varianttype, int size,
        MGEFF_VARMALLOC_CB varmalloc_callback, MGEFF_CALCVALUE_CB calcvalue_callback)
{
    return createex_animation (target, anima_param,
            varianttype, size,
            varmalloc_callback, calcvalue_callback,
            animation_callback);
}

