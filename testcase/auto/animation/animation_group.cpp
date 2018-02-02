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
#include "animation_base.h"

static bool groupFinished = false;
timeval group_start, group_end;
void group_finished (MGEFF_ANIMATION handle)
{
    gettimeofday (&group_end, NULL);
    groupFinished = true;
}

void test_animation_group_duration ()
{
    int i;
    TESTPRJ testprj[MGEFF_MAX];
    MGEFF_ANIMATION handles[MGEFF_MAX];
    int usetime;
    ANIMAPARAM animaparam[MGEFF_MAX];
    MGEFF_ANIMATION animation_group = NULL;

    EffPoint3D startval_3dpoint = {0,0,0}, endval_3dpoint  = {22, 134, 153};
    EffPointF3D startval_3dpointf = {0.0, 0.0, 0.0}, endval_3dpointf = {123.0, 213.0, 341.0};
    EffPointF startval_pointf = {0.0, 0.0}, endval_pointf = {129.0, 452.0};
    POINT startval_point = {0,0}, endval_point = {128, 512};
    RECT startval_rect = {0, 0, 0,0}, endval_rect = {100, 125, 139, 148};
    double endval_double = 1000000, startval_double = 0;
    float startval_float = 0.0f, endval_float = 1.0f;
    int startval_int = 3, endval_int = 232;

    memset (animaparam, 0, sizeof(animaparam));
    {
        animaparam[0].duration = 1230;
        animaparam[0].loopcount = 1;
        animaparam[0].fps = 2113;
        animaparam[0].startval = (void*)&startval_float;
        animaparam[0].endval = (void*)&endval_float;
        handles[0] = create_animation ((void*)&testprj[0], &animaparam[0], MGEFF_POINTF );
    }
#if 1
    {
        animaparam[1].duration = 892;
        animaparam[1].loopcount = 1;
        animaparam[1].fps = 213;
        animaparam[1].startval = (void*)&startval_double;
        animaparam[1].endval = (void*)&endval_double;
        handles[1] = create_animation ((void*)&testprj[1], &animaparam[1], MGEFF_DOUBLE);
    }
    {
        animaparam[2].duration = 1892;
        animaparam[2].loopcount = 1;
        animaparam[2].fps = 2213;
        animaparam[2].startval = (void*)&startval_point;
        animaparam[2].endval = (void*)&endval_point;
        handles[2] = create_animation ((void*)&testprj[2], &animaparam[2], MGEFF_POINT);
    }
    {
        animaparam[3].duration = 779;
        animaparam[3].loopcount = 1;
        animaparam[3].fps = 765;
        animaparam[3].startval = (void*)&startval_pointf;
        animaparam[3].endval = (void*)&endval_pointf;
        handles[3] = create_animation ((void*)&testprj[3], &animaparam[3], MGEFF_POINTF);
    }
    {
        animaparam[4].duration = 658;
        animaparam[4].loopcount = 1;
        animaparam[4].fps = 987;
        animaparam[4].startval = (void*)&startval_3dpoint;
        animaparam[4].endval = (void*)&endval_3dpoint;
        handles[4] = create_animation ((void*)&testprj[4], &animaparam[4], MGEFF_3DPOINT);
    }
    {
        animaparam[5].duration = 1444;
        animaparam[5].loopcount = 1;
        animaparam[5].fps = 675;
        animaparam[5].startval = (void*)&startval_3dpointf;
        animaparam[5].endval = (void*)&endval_3dpointf;
        handles[5] = create_animation ((void*)&testprj[5], &animaparam[5], MGEFF_3DPOINTF);
    }
    {
        animaparam[6].duration = 2333;
        animaparam[6].loopcount = 1;
        animaparam[6].fps = 1987;
        animaparam[6].startval = (void*)&startval_rect;
        animaparam[6].endval = (void*)&endval_rect;
        handles[6] = create_animation ((void*)&testprj[6], &animaparam[6], MGEFF_RECT);
    }
    {
        animaparam[7].duration = 2333;
        animaparam[7].loopcount = 1;
        animaparam[7].fps = 1987;
        animaparam[7].startval = (void*)&startval_int;
        animaparam[7].endval = (void*)&endval_int;
        handles[7] = create_animation ((void*)&testprj[7], &animaparam[7], MGEFF_INT);
    }
#endif

    animation_group = mGEffAnimationCreateGroup(MGEFF_PARALLEL);

    for (i=0; i<MGEFF_MAX-1; i++)
        mGEffAnimationAddToGroup (animation_group, handles[i]);

#define GROUPDURATION 5000
    mGEffAnimationSetProperty (animation_group, MGEFF_PROP_DURATION, GROUPDURATION);
    mGEffAnimationSetFinishedCb (animation_group, group_finished);

    gettimeofday (&group_start, NULL);
    mGEffAnimationSyncRun(animation_group);

    EXPECT_TRUE (usetime > GROUPDURATION-200);

    /*check duration time*/
    //i =0;
    for (i=0; i<MGEFF_MAX-1; i++)
    {
#define MISTAKE 460
        usetime = 1000 * (testprj[i].times.endtime.tv_sec - testprj[i].times.starttime.tv_sec) + \
                  (testprj[i].times.endtime.tv_usec-testprj[i].times.starttime.tv_usec)/1000;
        /*
        printf ("usetime: %d, animaparam[%d].duration:%d %d\n", usetime, i,
                animaparam[i].duration, animaparam[i].duration+MISTAKE);
                */
        EXPECT_TRUE (usetime > animaparam[i].duration && usetime < animaparam[i].duration+MISTAKE);
    }
    
    mGEffAnimationDelete (animation_group);
}

void test_animation_group_finished ()
{
    int i;
    TESTPRJ testprj[MGEFF_MAX];
    MGEFF_ANIMATION handles[MGEFF_MAX];
    int usetime;
    ANIMAPARAM animaparam[MGEFF_MAX];
    MGEFF_ANIMATION animation_group = NULL;

    EffPoint3D startval_3dpoint = {0,0,0}, endval_3dpoint  = {22, 134, 153};
    EffPointF3D startval_3dpointf = {0.0, 0.0, 0.0}, endval_3dpointf = {123.0, 213.0, 341.0};
    EffPointF startval_pointf = {0.0, 0.0}, endval_pointf = {129.0, 452.0};
    POINT startval_point = {0,0}, endval_point = {128, 512};
    RECT startval_rect = {0, 0, 0,0}, endval_rect = {100, 125, 139, 148};
    double endval_double = 1000000, startval_double = 0;
    float startval_float = 0.0f, endval_float = 1.0f;
    int startval_int = 3, endval_int = 232;

    memset (animaparam, 0, sizeof(animaparam));
    {
        animaparam[0].duration = 1230;
        animaparam[0].loopcount = 2;
        animaparam[0].fps = 2113;
        animaparam[0].startval = (void*)&startval_float;
        animaparam[0].endval = (void*)&endval_float;
        handles[0] = create_animation ((void*)&testprj[0], &animaparam[0], MGEFF_FLOAT);
    }
#if 1
    {
        animaparam[1].duration = 892;
        animaparam[1].loopcount = 3;
        animaparam[1].fps = 213;
        animaparam[1].startval = (void*)&startval_double;
        animaparam[1].endval = (void*)&endval_double;
        handles[1] = create_animation ((void*)&testprj[1], &animaparam[1], MGEFF_DOUBLE);
    }
    {
        animaparam[2].duration = 1892;
        animaparam[2].loopcount = 0;
        animaparam[2].fps = 2213;
        animaparam[2].startval = (void*)&startval_point;
        animaparam[2].endval = (void*)&endval_point;
        handles[2] = create_animation ((void*)&testprj[2], &animaparam[2], MGEFF_POINT);
    }
    {
        animaparam[3].duration = 779;
        animaparam[3].loopcount = 0;
        animaparam[3].fps = 765;
        animaparam[3].startval = (void*)&startval_pointf;
        animaparam[3].endval = (void*)&endval_pointf;
        handles[3] = create_animation ((void*)&testprj[3], &animaparam[3], MGEFF_POINTF);
    }
    {
        animaparam[4].duration = 658;
        animaparam[4].loopcount = 2;
        animaparam[4].fps = 987;
        animaparam[4].startval = (void*)&startval_3dpoint;
        animaparam[4].endval = (void*)&endval_3dpoint;
        handles[4] = create_animation ((void*)&testprj[4], &animaparam[4], MGEFF_3DPOINT);
    }
    {
        animaparam[5].duration = 1444;
        animaparam[5].loopcount = 1;
        animaparam[5].fps = 675;
        animaparam[5].startval = (void*)&startval_3dpointf;
        animaparam[5].endval = (void*)&endval_3dpointf;
        handles[5] = create_animation ((void*)&testprj[5], &animaparam[5], MGEFF_3DPOINTF);
    }
    {
        animaparam[6].duration = 465;
        animaparam[6].loopcount = 5;
        animaparam[6].fps = 232;
        animaparam[6].startval = (void*)&startval_rect;
        animaparam[6].endval = (void*)&endval_rect;
        handles[6] = create_animation ((void*)&testprj[6], &animaparam[6], MGEFF_RECT);
    }
    {
        animaparam[7].duration = 2333;
        animaparam[7].loopcount = 1;
        animaparam[7].fps = 1987;
        animaparam[7].startval = (void*)&startval_int;
        animaparam[7].endval = (void*)&endval_int;
        handles[7] = create_animation ((void*)&testprj[7], &animaparam[7], MGEFF_INT);
    }
#endif

    animation_group = mGEffAnimationCreateGroup(MGEFF_PARALLEL);

    for (i=0; i<MGEFF_MAX-1; i++)
        mGEffAnimationAddToGroup (animation_group, handles[i]);

    mGEffAnimationSetFinishedCb (animation_group, group_finished);

    gettimeofday (&group_start, NULL);

    mGEffAnimationSyncRun(animation_group);

    EXPECT_TRUE (groupFinished);

    /*check duration time*/
    for (i=0; i<MGEFF_MAX-1; i++)
    {
#define MISTAKE 460
        usetime = 1000 * (testprj[i].times.endtime.tv_sec - testprj[i].times.starttime.tv_sec) + \
                  (testprj[i].times.endtime.tv_usec-testprj[i].times.starttime.tv_usec)/1000;
        //printf ("usetime: %d, animaparam[%d].duration:%d %d\n", usetime, i,
        //        animaparam[i].duration, animaparam[i].duration+MISTAKE);
        EXPECT_TRUE (usetime > animaparam[i].duration && usetime < animaparam[i].duration+MISTAKE);
    }
    
   // mGEffAnimationDelete (animation_group);
}


/*test direction */
static void direction_change (MGEFF_ANIMATION handle)
{
    TESTPRJ *testprj = (TESTPRJ*)mGEffAnimationGetTarget (handle);
    ++testprj->val.reserve;
}

void test_animation_group_direction ()
{
    int i;
    TESTPRJ testprj[MGEFF_MAX];
    MGEFF_ANIMATION handles[MGEFF_MAX];
    ANIMAPARAM animaparam[MGEFF_MAX];
    MGEFF_ANIMATION animation_group = NULL;
    mGEffInit();

    EffPoint3D startval_3dpoint = {0,0,0}, endval_3dpoint  = {22, 134, 153};
    EffPointF3D startval_3dpointf = {0.0, 0.0, 0.0}, endval_3dpointf = {123.0, 213.0, 341.0};
    EffPointF startval_pointf = {0.0, 0.0}, endval_pointf = {129.0, 452.0};
    POINT startval_point = {0,0}, endval_point = {128, 512};
    RECT startval_rect = {0, 0, 0,0}, endval_rect = {100, 125, 139, 148};
    double endval_double = 1000000, startval_double = 0;
    float startval_float = 0.0f, endval_float = 1.0f;
    int startval_int = 3, endval_int = 232;

    memset (animaparam, 0, sizeof(animaparam));
    {
        animaparam[0].duration = 230;
        animaparam[0].loopcount = 8;
        animaparam[0].fps = 113;
        animaparam[0].startval = (void*)&startval_float;
        animaparam[0].endval = (void*)&endval_float;
        animaparam[0].direction = MGEFF_DIR_FORWARD;
        handles[0] = create_animation ((void*)&testprj[0], &animaparam[0], MGEFF_FLOAT);
        mGEffAnimationSetDirChangedCb (handles[0], direction_change);
    }
#if 1
    {
        animaparam[1].duration = 592;
        animaparam[1].loopcount = 4;
        animaparam[1].fps = 213;
        animaparam[1].startval = (void*)&startval_double;
        animaparam[1].endval = (void*)&endval_double;
        animaparam[1].direction = MGEFF_DIR_BACKWARD;
        handles[1] = create_animation ((void*)&testprj[1], &animaparam[1], MGEFF_DOUBLE);
        mGEffAnimationSetDirChangedCb (handles[1], direction_change);
    }
    {
        animaparam[2].duration = 1892;
        animaparam[2].loopcount = 2;
        animaparam[2].fps = 2213;
        animaparam[2].startval = (void*)&startval_point;
        animaparam[2].endval = (void*)&endval_point;
        handles[2] = create_animation ((void*)&testprj[2], &animaparam[2], MGEFF_POINT);
        mGEffAnimationSetDirChangedCb (handles[2], direction_change);
    }
    {
        animaparam[3].duration = 779;
        animaparam[3].loopcount = 0;
        animaparam[3].fps = 765;
        animaparam[3].startval = (void*)&startval_pointf;
        animaparam[3].endval = (void*)&endval_pointf;
        animaparam[3].direction = MGEFF_DIR_BACKWARD;
        handles[3] = create_animation ((void*)&testprj[3], &animaparam[3], MGEFF_POINTF);
        mGEffAnimationSetDirChangedCb (handles[3], direction_change);
    }
    {
        animaparam[4].duration = 658;
        animaparam[4].loopcount = 2;
        animaparam[4].fps = 987;
        animaparam[4].startval = (void*)&startval_3dpoint;
        animaparam[4].endval = (void*)&endval_3dpoint;
        animaparam[4].direction = MGEFF_DIR_FORWARD;
        handles[4] = create_animation ((void*)&testprj[4], &animaparam[4], MGEFF_3DPOINT);
        mGEffAnimationSetDirChangedCb (handles[4], direction_change);
    }
    {
        animaparam[5].duration = 1444;
        animaparam[5].loopcount = 1;
        animaparam[5].fps = 675;
        animaparam[5].startval = (void*)&startval_3dpointf;
        animaparam[5].endval = (void*)&endval_3dpointf;
        handles[5] = create_animation ((void*)&testprj[5], &animaparam[5], MGEFF_3DPOINTF);
        mGEffAnimationSetDirChangedCb (handles[5], direction_change);
    }
    {
        animaparam[6].duration = 2333;
        animaparam[6].loopcount = 0;
        animaparam[6].fps = 1987;
        animaparam[6].startval = (void*)&startval_rect;
        animaparam[6].endval = (void*)&endval_rect;
        animaparam[6].direction = MGEFF_DIR_BACKWARD;
        handles[6] = create_animation ((void*)&testprj[6], &animaparam[6], MGEFF_RECT);
        mGEffAnimationSetDirChangedCb (handles[6], direction_change);
    }
    {
        animaparam[7].duration = 1333;
        animaparam[7].loopcount = 1;
        animaparam[7].fps = 874;
        animaparam[7].startval = (void*)&startval_int;
        animaparam[7].endval = (void*)&endval_int;
        handles[7] = create_animation ((void*)&testprj[7], &animaparam[7], MGEFF_INT);
        mGEffAnimationSetDirChangedCb (handles[7], direction_change);
    }
#endif

    animation_group = mGEffAnimationCreateGroup(MGEFF_PARALLEL);

    for (i=0; i<MGEFF_MAX-1; i++)
        mGEffAnimationAddToGroup (animation_group, handles[i]);

#define GROUPDURATION 5000
    mGEffAnimationSetFinishedCb (animation_group, group_finished);

    gettimeofday (&group_start, NULL);
    mGEffAnimationSyncRun(animation_group);

    EXPECT_TRUE (groupFinished);

    mGEffAnimationDelete (animation_group);
    mGEffDeinit();
}

void test_animation_group_loopcount ()
{}

