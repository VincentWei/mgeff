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
#include "animation_base.h"
#include "animation_int.h"

ANIMAPARAM animaparam;
TESTPRJ testprj;

static void *check_pause_command_pthread (void *arg)
{
    int stopframe = 0;
    MGEFF_ANIMATION handle = (MGEFF_ANIMATION)arg;
    
    assert (handle);
    mGEffAnimationPause(handle);
    usleep (0);
    stopframe = mGEffAnimationGetProperty(handle, MGEFF_PROP_CURFRAME);
    usleep (1000 * 1000);/*sleep 1s*/

    if (stopframe != mGEffAnimationGetProperty(handle, MGEFF_PROP_CURFRAME)) /*if animation don't pause, will error*/
    {
        printf ("sorry, check pause error, curframe: %d, expect frame: %d\n", 
                mGEffAnimationGetProperty(handle, MGEFF_PROP_CURFRAME), stopframe);
        assert (0);
    }

    /*restart animation*/
    mGEffAnimationResume (handle);

    return NULL;
}

static void check_pause_finished (MGEFF_ANIMATION handle)
{
    int usetime;
    ANIMAPARAM *animaparam = (ANIMAPARAM*)mGEffAnimationGetContext (handle);
    TESTPRJ *testprj = (TESTPRJ*)mGEffAnimationGetTarget (handle);
    testprj->val.bfinished = true;

    EXPECT_EQ (*((int*)testprj->val.endval), *((int*)animaparam->endval));

    usetime = 1000 * (testprj->times.endtime.tv_sec - testprj->times.starttime.tv_sec) + \
                  (testprj->times.endtime.tv_usec - testprj->times.starttime.tv_usec)/1000;
    EXPECT_TRUE ((animaparam->duration <= usetime) && \
            ((animaparam->duration+ 1000)+70 > usetime)); /*enable 50ms mistake*/
}

static pthread_t thread_id;
static void check_pause_cb (MGEFF_ANIMATION handle, void* target, int id, void* value)
{
    TESTPRJ *testprj = (TESTPRJ*)mGEffAnimationGetTarget (handle);
    /*public part*/
    if (mGEffAnimationGetProperty (handle, MGEFF_PROP_CURFRAME) == 0)
    {
        gettimeofday (&testprj->times.starttime, NULL);
    }
    gettimeofday (&testprj->times.endtime, NULL);
    testprj->val.endval = value;
    ++testprj->val.loopcount;

    /*start the thread to send command */
    if (mGEffAnimationGetProperty(handle, MGEFF_PROP_CURFRAME) == 2) {
        pthread_create(&thread_id, NULL, check_pause_command_pthread, (void *)handle);
    }
}

void test_animation_pause()
{
    TESTPRJ testprj;
    MGEFF_ANIMATION handle;
    ANIMAPARAM animaparam;
    int startval_int = 3, endval_int = 232;

    memset (&testprj, 0, sizeof(testprj));
    memset (&animaparam, 0, sizeof(animaparam));
    animaparam.duration = 3250;
    animaparam.startval = (void*)&startval_int;
    animaparam.endval = (void*)&endval_int;
    handle = create_animation ((void*)&testprj, &animaparam, MGEFF_INT, check_pause_cb);

    mGEffAnimationSetFinishedCb (handle, check_pause_finished);

    mGEffAnimationSyncRun (handle);

    mGEffAnimationDelete (handle);
}
#if 0
/*test animationex*/
static void* varmalloc_callback (MGEFF_ANIMATION handle)
{
    return (void *)malloc (sizeof(int));
}

void  calcvalue_callback (MGEFF_ANIMATION handle, void* s, void* e, void* v, float factor)
{
    *(int*)v = *(int*)s + (*(int*)e - *(int*)s) * factor;
}
#endif
static void animationex_cb (MGEFF_ANIMATION handle, void* target, int id, void* value)
{
    TESTPRJ *testprj= (TESTPRJ*)target;

    /*public part*/
    if (mGEffAnimationGetProperty (handle, MGEFF_PROP_CURFRAME) == 0)
    {
        gettimeofday (&testprj->times.starttime, NULL);
    }
    gettimeofday (&testprj->times.endtime, NULL);
    testprj->val.endval = value;
    ++testprj->val.loopcount;
}
void test_animation_create ()
{
    MGEFF_ANIMATION handle = NULL;

    TESTPRJ testprj;
    ANIMAPARAM animaparam;

    int usetime;
    int startval_int=0, endval_int=541;

    memset (&testprj, 0, sizeof (testprj));
    memset (&animaparam, 0, sizeof (animaparam));

    animaparam.duration = 345;
    animaparam.loopcount = 3;
    animaparam.fps = 301;
    animaparam.direction = MGEFF_DIR_FORWARD;
    animaparam.startval = (void*)&startval_int;
    animaparam.endval = (void*)&endval_int;

    handle = mGEffAnimationCreate((void*)&testprj, animationex_cb, 0, MGEFF_INT );

    ASSERT_TRUE( handle != NULL );

    mGEffAnimationSetStartValue (handle, animaparam.startval);
    mGEffAnimationSetEndValue (handle, animaparam.endval);
    mGEffAnimationSetProperty (handle, MGEFF_PROP_DURATION, animaparam.duration);
//    mGEffAnimationSetProperty (handle, MGEFF_PROP_FREQUENCY, animaparam.fps);
    mGEffAnimationSetProperty (handle, MGEFF_PROP_DIRECTION, animaparam.direction);

    mGEffAnimationSyncRun (handle);

    EXPECT_EQ (*(int*)animaparam.endval, *(int*)testprj.val.endval);
    usetime = 1000 * (testprj.times.endtime.tv_sec - testprj.times.starttime.tv_sec) + \
                  (testprj.times.endtime.tv_usec-testprj.times.starttime.tv_usec)/1000;
    EXPECT_TRUE (usetime > animaparam.duration && usetime < animaparam.duration+80);

    ASSERT_DEATH(mGEffAnimationDelete (handle), "");
}

void test_animation_createex ()
{
    int usetime;
    TESTPRJ testprj;
    int startval_int=0, endval_int=541;
    ANIMAPARAM animaparam;
    MGEFF_ANIMATION handle = NULL;

    memset (&animaparam, 0, sizeof (animaparam));
    animaparam.duration = 345;
    animaparam.loopcount = 3;
    animaparam.fps = 301;
    animaparam.direction = MGEFF_DIR_FORWARD;
    animaparam.startval = (void*)&startval_int;
    animaparam.endval = (void*)&endval_int;
    handle = mGEffAnimationCreateEx((void*)&testprj, animationex_cb, 0, 
            sizeof(int), varmalloc_callback, calcvalue_callback);

    mGEffAnimationSetStartValue (handle, animaparam.startval);
    mGEffAnimationSetEndValue (handle, animaparam.endval);

    mGEffAnimationSetProperty (handle, MGEFF_PROP_DURATION, animaparam.duration);
//    mGEffAnimationSetProperty (handle, MGEFF_PROP_FREQUENCY, animaparam.fps);
    mGEffAnimationSetProperty (handle, MGEFF_PROP_DIRECTION, animaparam.direction);

    mGEffAnimationSyncRun (handle);

    EXPECT_EQ (*(int*)animaparam.endval, *(int*)testprj.val.endval);
    usetime = 1000 * (testprj.times.endtime.tv_sec - testprj.times.starttime.tv_sec) + \
                  (testprj.times.endtime.tv_usec-testprj.times.starttime.tv_usec)/1000;
    EXPECT_TRUE (usetime > animaparam.duration && usetime < animaparam.duration+80);

    mGEffAnimationDelete (handle);
}

static enum EffState cur_state = MGEFF_STATE_READY, last_state = MGEFF_STATE_READY;
static void state_change_cb (MGEFF_ANIMATION handle, enum EffState newEffState, enum EffState oldEffState)
{
    cur_state = newEffState;
    last_state = oldEffState;
}
void test_animation_state_change ()
{
    int i;
    TESTPRJ testprj;
    MGEFF_ANIMATION handle;
    ANIMAPARAM animaparam;
    int startval_int = 3, endval_int = 232;

    memset (&testprj, 0, sizeof(testprj));
    memset (&animaparam, 0, sizeof(animaparam));
    animaparam.duration = 325;
    animaparam.loopcount = 7;
    animaparam.startval = (void*)&startval_int;
    animaparam.endval = (void*)&endval_int;
    handle = create_animation ((void*)&testprj, &animaparam, MGEFF_INT);
    mGEffAnimationSetStateChangedCb (handle, state_change_cb);
    for (i=0; i<5; i++) {
        mGEffAnimationSyncRun(handle);
        EXPECT_EQ (MGEFF_STATE_STOPPED, cur_state);
        EXPECT_EQ (MGEFF_STATE_RUNNING, last_state);
    }
    mGEffAnimationDelete (handle);
}

void test_animation_finished ()
{
    TESTPRJ testprj;
    MGEFF_ANIMATION handle;
    ANIMAPARAM animaparam;
    int startval_int = 3, endval_int = 232;

    assert (!mGEffInit());
    memset (&testprj, 0, sizeof(testprj));
    memset (&animaparam, 0, sizeof(animaparam));
    animaparam.duration = 325;
    animaparam.loopcount = 13;
    animaparam.fps = 13;
    animaparam.startval = (void*)&startval_int;
    animaparam.endval = (void*)&endval_int;
    handle = create_animation ((void*)&testprj, &animaparam, MGEFF_INT);
    mGEffAnimationSetFinishedCb (handle, finished_callback);

    mGEffAnimationSyncRun(handle);
    
    EXPECT_TRUE (testprj.val.bfinished);

    mGEffAnimationDelete (handle);
}

void test_animation_duration()
{
    int usetime;
    int startval_int = 3, endval_int = 232;
    MGEFF_ANIMATION handle;

    memset (&testprj, 0, sizeof(testprj));
    memset (&animaparam, 0, sizeof(animaparam));
    animaparam.duration = 325;
    animaparam.loopcount = 1;
    animaparam.fps = 13;
    animaparam.startval = (void*)&startval_int;
    animaparam.endval = (void*)&endval_int;
    handle = create_animation ((void*)&testprj, &animaparam, MGEFF_INT);

    mGEffAnimationSyncRun (handle);

    usetime = 1000 * (testprj.times.endtime.tv_sec - testprj.times.starttime.tv_sec) + \
                  (testprj.times.endtime.tv_usec-testprj.times.starttime.tv_usec)/1000;
    EXPECT_TRUE (usetime >= animaparam.duration && usetime < animaparam.duration+260);

    mGEffAnimationDelete (handle);
}

static void loopcount_cb(MGEFF_ANIMATION handle)
{
    TESTPRJ *testprj = (TESTPRJ*)mGEffAnimationGetTarget (handle);
    ++testprj->val.loopcount;
}
void test_animation_loopcount ()
{
    int startval_int = 3, endval_int = 232;
    TESTPRJ testprj;
    MGEFF_ANIMATION handle;
    ANIMAPARAM animaparam;

    memset (&testprj, 0, sizeof(testprj));
    memset (&animaparam, 0, sizeof(animaparam));
    animaparam.duration = 230;
    animaparam.loopcount = 13;
    animaparam.fps = 13;
    animaparam.startval = (void*)&startval_int;
    animaparam.endval = (void*)&endval_int;
    handle = create_animation ((void*)&testprj, &animaparam, MGEFF_INT);
    mGEffAnimationSetCurLoopChangedCb(handle,loopcount_cb);

    mGEffAnimationSyncRun (handle);

    EXPECT_EQ (animaparam.loopcount, testprj.val.loopcount);

    mGEffAnimationDelete (handle);
}

/*test direction change */
static void direction_change_cb (MGEFF_ANIMATION handle)
{
    TESTPRJ *testprj;
    testprj = (TESTPRJ*)mGEffAnimationGetTarget (handle);

    ++testprj->val.reserve;
}
void test_animation_direction()
{
    int changes = 13, i;
    int startval_int = 3, endval_int = 232;
    TESTPRJ testprj;
    MGEFF_ANIMATION handle;
    ANIMAPARAM animaparam;

    memset (&testprj, 0, sizeof(testprj));
    memset (&animaparam, 0, sizeof(animaparam));

    animaparam.duration = 230;
    animaparam.loopcount = 1;
    animaparam.fps = 113;
    animaparam.startval = (void*)&startval_int;
    animaparam.endval = (void*)&endval_int;
    handle = create_animation ((void*)&testprj, &animaparam, MGEFF_INT);

    assert (handle != NULL);

    mGEffAnimationSetDirChangedCb (handle, direction_change_cb);

    mGEffAnimationSyncRun (handle);

    for (i=0; i<changes; ++i)
    {
        mGEffAnimationSetProperty (handle, 
                MGEFF_PROP_DIRECTION, (i%2) ? MGEFF_DIR_FORWARD : MGEFF_DIR_BACKWARD);

        mGEffAnimationSyncRun (handle);
    }

    EXPECT_EQ (changes, testprj.val.reserve);
    mGEffAnimationDelete (handle);
}

void test_animation_paramsetting ()
{
    int usetime;
    int startval_int = 3, endval_int = 232;
    TESTPRJ testprj;
    MGEFF_ANIMATION handle;
    ANIMAPARAM animaparam;

    memset (&testprj, 0, sizeof(testprj));
    memset (&animaparam, 0, sizeof(animaparam));
    animaparam.duration = 325;
//    animaparam.loopcount = 13;
    animaparam.fps = 13;
//    animaparam.startval = (void*)&startval_int;
    animaparam.endval = (void*)&endval_int;
    handle = create_animation ((void*)&testprj, &animaparam, MGEFF_INT);

    mGEffAnimationSyncRun (handle);

    usetime = 1000 * (testprj.times.endtime.tv_sec - testprj.times.starttime.tv_sec) + \
                  (testprj.times.endtime.tv_usec-testprj.times.starttime.tv_usec)/1000;
    EXPECT_TRUE (usetime > animaparam.duration && usetime < animaparam.duration+260);
    mGEffAnimationDelete (handle);

    memset (&testprj, 0, sizeof(testprj));
    memset (&animaparam, 0, sizeof(animaparam));
    animaparam.duration = 325;
//    animaparam.loopcount = 13;
    animaparam.fps = 13;
    animaparam.startval = (void*)&startval_int;
//    animaparam.endval = (void*)&endval_int;
    handle = create_animation ((void*)&testprj, &animaparam, MGEFF_INT);

    mGEffAnimationSyncRun (handle);

    usetime = 1000 * (testprj.times.endtime.tv_sec - testprj.times.starttime.tv_sec) + \
                  (testprj.times.endtime.tv_usec-testprj.times.starttime.tv_usec)/1000;
    EXPECT_TRUE (usetime > animaparam.duration && usetime < animaparam.duration+260);
    mGEffAnimationDelete (handle);
}

void test_animation_fps ()
{
}
