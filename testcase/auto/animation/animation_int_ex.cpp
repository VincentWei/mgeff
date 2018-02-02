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
#include "animation_int_ex.h"

extern HWND g_hwnd;
static MGEFF_ANIMATION g_handle = NULL;

int mysleep( long sleeptime )
{
    long tst = sleeptime;

    struct timeval start, end;
    struct timeval tv;

    gettimeofday( &start, NULL);

    for (;;){
        tv.tv_sec = tst / 1000000L;
        tv.tv_usec = tst % 1000000L;

        select( 0, NULL, NULL, NULL, &tv );
        gettimeofday( &end, NULL);
        if (((end.tv_sec * 1000000L + end.tv_usec) - (start.tv_sec * 1000000L + start.tv_usec))>=tst)
        {
            break;
        }
        tst = (end.tv_sec * 1000000L + end.tv_usec) - (start.tv_sec * 1000000L + start.tv_usec);
    }
    return 0;
}

static BOOL execute_pauseresume_command( HWND hWnd, int id, DWORD ms )
{
    MGEFF_ANIMATION handle = NULL;
    TESTPRJ *testprj = NULL;

    static int g_timertick = 0;
    static int stopframe1 = 0;
    static int stopframe2 = 0;

    assert (g_handle != NULL);

    handle = (MGEFF_ANIMATION)g_handle;

    testprj = (TESTPRJ *)mGEffAnimationGetTarget (handle);

    g_timertick++;

    switch (g_timertick)
    {
        case 20:
            testprj->val.bpauseflag = TRUE;

            mGEffAnimationPause(handle);

            gettimeofday (&testprj->times.pausetime, NULL);

            testprj->val.bstopflag = TRUE;

            stopframe1 = mGEffAnimationGetProperty(handle, MGEFF_PROP_CURFRAME);
            /* pause */
            break;
        case 40:
            stopframe2 = mGEffAnimationGetProperty(handle, MGEFF_PROP_CURFRAME);

            EXPECT_EQ(stopframe1, stopframe2);

            EXPECT_TRUE(testprj->val.bfinished == FALSE);

            testprj->val.bstopflag = FALSE;

            /* resume animation */
            mGEffAnimationResume (handle);

            gettimeofday (&testprj->times.resumetime, NULL);
            break;
        case 60:
            stopframe2 = mGEffAnimationGetProperty(handle, MGEFF_PROP_CURFRAME);

            EXPECT_LT(stopframe1, stopframe2);

            EXPECT_TRUE(testprj->val.bfinished == FALSE);

            KillTimer( hWnd, id );
            break;
        default:
            /* do nothing */
            break;
    }

    return TRUE;
}
#if 0
static void check_pauseresume_callback (MGEFF_ANIMATION handle, void* target, int id, void* value)
{
    TESTPRJ *testprj = (TESTPRJ*)mGEffAnimationGetTarget (handle);
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
void test_animation_pauseresume()
{
    MGEFF_ANIMATION handle;

    TESTPRJ testprj;
    ANIMAPARAM animaparam;

    int usetime, pausetime;
    int startval_int = 1, endval_int = 300;
    int ret_startval_int, ret_endval_int;

//    assert (!mGEffInit());

    memset (&testprj, 0, sizeof (testprj));
    memset (&animaparam, 0, sizeof (animaparam));

    testprj.val.startval = (void *)&ret_startval_int;
    testprj.val.endval = (void *)&ret_endval_int;

    animaparam.duration = 15000; /* > 3s */
    animaparam.loopcount = 1;
//    animaparam.fps = 30;
    animaparam.direction = MGEFF_DIR_FORWARD;
    animaparam.startval = (void *)&startval_int;
    animaparam.endval = (void *)&endval_int;

    handle = createex_animation ((void*)&testprj, &animaparam, MGEFF_INT,
            sizeof(int), varmalloc_callback, calcvalue_callback);

    ASSERT_TRUE (handle != NULL);

    g_handle = handle;

//    mGEffAnimationSetFinishedCb (handle, finished_callback);

    assert (g_hwnd != (HWND)NULL);

    SetTimerEx (g_hwnd, PAUSERESUME_TIMER_ID, 10, execute_pauseresume_command);

    mGEffAnimationAsyncRun (handle);

    assert (g_hwnd != (HWND)NULL);

    mGEffAnimationWait ((void *)&g_hwnd, handle);

    EXPECT_TRUE (testprj.val.bpauseflag == TRUE);

    EXPECT_EQ (*(int*)animaparam.startval, *(int*)testprj.val.startval);
    EXPECT_EQ (*(int*)animaparam.endval, *(int*)testprj.val.endval);

    usetime = (1000000 * (testprj.times.endtime.tv_sec - testprj.times.starttime.tv_sec) + \
                  (testprj.times.endtime.tv_usec - testprj.times.starttime.tv_usec)) / 1000;

    // sleep 1s
    //EXPECT_TRUE ((usetime >= ((animaparam.duration + 1000) * animaparam.loopcount)) &&
    //        (usetime < ((animaparam.duration + 1000 + 80) * animaparam.loopcount)));
    EXPECT_GE (usetime, (animaparam.duration + 2000) * animaparam.loopcount);
    EXPECT_LT (usetime, (animaparam.duration + 2000 + 250) * animaparam.loopcount);

    pausetime = (1000000 * (testprj.times.resumetime.tv_sec - testprj.times.pausetime.tv_sec) + \
                  (testprj.times.resumetime.tv_usec - testprj.times.pausetime.tv_usec)) / 1000;

    EXPECT_GE (pausetime, 2000);
    EXPECT_LT (pausetime, 2000 + 250);

    mGEffAnimationDelete (handle);

//    mGEffDeinit();
}

static BOOL execute_stop_command (HWND hWnd, int id, DWORD ms)
{
    MGEFF_ANIMATION handle = NULL;

    static int g_timertick = 0;

    TESTPRJ *testprj = NULL;

    assert (g_handle != NULL);

    handle = (MGEFF_ANIMATION)g_handle;

    testprj = (TESTPRJ*)mGEffAnimationGetTarget (handle);

    g_timertick++;
    switch (g_timertick)
    {
        case 20:
            EXPECT_TRUE (testprj->val.bfinished == FALSE);

            mGEffAnimationStop(handle);

            gettimeofday (&testprj->times.stoptime, NULL);

            testprj->val.bstopflag = TRUE;

            KillTimer( hWnd, id );
            break;
        default:
            /* do nothing */
            break;
    }

    return TRUE;
}

void test_animation_stop(int keepalive)
{
    MGEFF_ANIMATION handle = NULL;

    TESTPRJ testprj;
    ANIMAPARAM animaparam;

    int usetime, stoptime, finishtime;
    int startval_int = 1, endval_int = 400;
    int ret_startval_int, ret_endval_int;
    int stopframe1 = 0, stopframe2 = 0;

    memset (&testprj, 0, sizeof (testprj));
    memset (&animaparam, 0, sizeof (animaparam));

    testprj.val.startval = (void *)&ret_startval_int;
    testprj.val.endval = (void *)&ret_endval_int;

    animaparam.duration = 8000;
    animaparam.loopcount = 1;
//    animaparam.fps = 20;
    animaparam.direction = MGEFF_DIR_FORWARD;
    animaparam.startval = (void*)&startval_int;
    animaparam.endval = (void*)&endval_int;

//    assert (!mGEffInit());

    handle = createex_animation ((void*)&testprj, &animaparam, MGEFF_INT,
            sizeof(int), varmalloc_callback, calcvalue_callback);

    ASSERT_TRUE( handle != NULL );

    g_handle = handle;

//    mGEffAnimationSetFinishedCb (handle, finished_callback);

    SetTimerEx (g_hwnd, STOP_TIMER_ID + keepalive, 5, execute_stop_command);

    mGEffAnimationAsyncRun (handle);

    assert (g_hwnd != (HWND)NULL);

    mGEffAnimationWait ((void *)&g_hwnd, handle);

    EXPECT_TRUE (testprj.val.bstopflag == TRUE);

    EXPECT_TRUE (testprj.val.bfinished == TRUE);

    EXPECT_EQ (*(int*)animaparam.startval, *(int*)testprj.val.startval);

    EXPECT_GT (*(int*)animaparam.endval, *(int*)testprj.val.endval);

    usetime = 1000 * (testprj.times.endtime.tv_sec - testprj.times.starttime.tv_sec) + \
                  (testprj.times.endtime.tv_usec - testprj.times.starttime.tv_usec) / 1000;

    // EXPECT_TRUE ((usetime >= 1000) && (usetime < (1000 + 80)));
    EXPECT_GE (usetime, 1000);
    EXPECT_LT (usetime, 1000 + 80);

    stoptime = 1000 * (testprj.times.stoptime.tv_sec - testprj.times.starttime.tv_sec) + \
                  (testprj.times.stoptime.tv_usec - testprj.times.starttime.tv_usec) / 1000;

    // EXPECT_TRUE ((usetime >= 1000) && (usetime < (1000 + 80)));
    EXPECT_GE (stoptime, 1000);
    EXPECT_LT (stoptime, 1000 + 80);

    finishtime = 1000 * (testprj.times.finishtime.tv_sec - testprj.times.starttime.tv_sec) + \
                  (testprj.times.finishtime.tv_usec - testprj.times.starttime.tv_usec) / 1000;

    // EXPECT_TRUE ((usetime >= 1000) && (usetime < (1000 + 80)));
    EXPECT_GE (finishtime, 1000);
    EXPECT_LT (finishtime, 1000 + 80);

    if (keepalive == 1)
    {
        stopframe1 = mGEffAnimationGetProperty(handle, MGEFF_PROP_CURFRAME);

        mysleep (1000 * 1000);/*sleep 1s*/

        stopframe2 = mGEffAnimationGetProperty(handle, MGEFF_PROP_CURFRAME);

        EXPECT_EQ (stopframe1, stopframe2);

        EXPECT_NO_THROW ({
            mGEffAnimationDelete (handle);
        });
#if 0
    } else {
        EXPECT_ANY_THROW ({
            mGEffAnimationSetProperty (handle, MGEFF_PROP_DURATION, animaparam.duration);
        });
#endif
    }

    handle = NULL;
//    mGEffDeinit();
} /* test_animation_stop */

#if 0
void test_animation_create ()
{
    MGEFF_ANIMATION handle = NULL;

    TESTPRJ testprj;
    ANIMAPARAM animaparam;

    int usetime;
    int startval_int = 0, endval_int = 541;

    memset (&testprj, 0, sizeof (testprj));
    memset (&animaparam, 0, sizeof (animaparam));

    animaparam.duration = 345;
    animaparam.loopcount = 3;
//    animaparam.fps = 301;
    animaparam.direction = MGEFF_DIR_FORWARD;
    animaparam.startval = (void*)&startval_int;
    animaparam.endval = (void*)&endval_int;

//    assert (!mGEffInit());

    handle = mGEffAnimationCreate((void*)&testprj, animationex_callback, 0, MGEFF_INT );

    ASSERT_TRUE( handle != NULL );

    mGEffAnimationSetStartValue (handle, animaparam.startval);
    mGEffAnimationSetEndValue (handle, animaparam.endval);

    mGEffAnimationSetProperty (handle, MGEFF_PROP_DURATION, animaparam.duration);
    mGEffAnimationSetProperty (handle, MGEFF_PROP_FREQUENCY, animaparam.fps);
    mGEffAnimationSetProperty (handle, MGEFF_PROP_DIRECTION, animaparam.direction);

    mGEffAnimationSyncRun(handle);

    EXPECT_EQ (*(int*)animaparam.endval, *(int*)testprj.val.endval);
    usetime = 1000 * (testprj.times.endtime.tv_sec - testprj.times.starttime.tv_sec) + \
                  (testprj.times.endtime.tv_usec - testprj.times.starttime.tv_usec) / 1000;

    EXPECT_TRUE ((usetime >= (animaparam.duration * animaparam.loopcount)) &&
            (usetime < ((animaparam.duration + 80) * animaparam.loopcount)));

    mGEffAnimationDelete (handle);

//    mGEffDeinit();
} /* end test_animation_create */
#endif

void *t_varmalloc_callback (MGEFF_ANIMATION handle)
{
    TESTPRJ *testprj = (TESTPRJ*)mGEffAnimationGetTarget (handle);

    testprj->val.bvarmalloc = TRUE;

    return (void *)malloc (32);
}

void t_calcvalue_callback (MGEFF_ANIMATION handle, void *s, void *e, void *v, float factor)
{
    TESTPRJ *testprj = (TESTPRJ*)mGEffAnimationGetTarget (handle);

    testprj->val.bcalcvalue = TRUE;

    *(int*)v = *(int*)s + (*(int*)e - *(int*)s) * factor;
}

void t_animationex_callback (MGEFF_ANIMATION handle, void *target, int id, void* value)
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

void test_animation_runtime ()
{
    MGEFF_ANIMATION handle = NULL;

    TESTPRJ testprj;
    ANIMAPARAM animaparam;

    int usetime;
    int startval_int = 100, endval_int = 200;
    int ret_startval_int, ret_endval_int;

    memset (&testprj, 0, sizeof (testprj));
    memset (&animaparam, 0, sizeof (animaparam));

    testprj.val.startval = (void *)&ret_startval_int;
    testprj.val.endval = (void *)&ret_endval_int;

    animaparam.duration = 2000;
    animaparam.loopcount = 1;
//    animaparam.fps = 20;
    animaparam.direction = MGEFF_DIR_FORWARD;
    animaparam.startval = (void *)&startval_int;
    animaparam.endval = (void *)&endval_int;

//    assert (!mGEffInit());

//    handle = mGEffAnimationCreateEx((void*)&testprj, t_animationex_callback, MGEFF_INT, 
//            sizeof(int), t_varmalloc_callback, t_calcvalue_callback);

    handle = mGEffAnimationCreate((void*)&testprj, t_animationex_callback,0, MGEFF_INT );
    ASSERT_TRUE( handle != NULL );

    mGEffAnimationSetStartValue (handle, animaparam.startval);
    mGEffAnimationSetEndValue (handle, animaparam.endval);

    mGEffAnimationSetProperty (handle, MGEFF_PROP_DURATION, animaparam.duration);
//    mGEffAnimationSetProperty (handle, MGEFF_PROP_LOOPCOUNT, animaparam.loopcount);
//    mGEffAnimationSetProperty (handle, MGEFF_PROP_FREQUENCY, animaparam.fps);
//    mGEffAnimationSetProperty (handle, MGEFF_PROP_DIRECTION, animaparam.direction);

    mGEffAnimationSetFinishedCb (handle, finished_callback);
    mGEffAnimationSetContext (handle, (void *)&animaparam);

#if 1
    mGEffAnimationAsyncRun(handle);

    assert (g_hwnd != (HWND)NULL);

    mGEffAnimationWait((void *)&g_hwnd,handle);
#else
    mGEffAnimationSyncRun(handle);
#endif

    EXPECT_EQ (*(int*)animaparam.startval, *(int*)testprj.val.startval);
    EXPECT_EQ (*(int*)animaparam.endval, *(int*)testprj.val.endval);

    usetime = 1000 * (testprj.times.endtime.tv_sec - testprj.times.starttime.tv_sec) + \
                  (testprj.times.endtime.tv_usec - testprj.times.starttime.tv_usec) / 1000;

    //EXPECT_TRUE ((usetime >= (animaparam.duration * animaparam.loopcount)) &&
    //        (usetime < ((animaparam.duration + 80) * animaparam.loopcount)));
    EXPECT_GE (usetime, animaparam.duration * animaparam.loopcount);
    EXPECT_LT (usetime, (animaparam.duration + 80) * animaparam.loopcount);

    mGEffAnimationDelete (handle);

//    mGEffDeinit();
} /* end test_animation_time */

void test_animation_createex ()
{
    MGEFF_ANIMATION handle = NULL;

    TESTPRJ testprj;
    ANIMAPARAM animaparam;

    int usetime;
    int startval_int = 1, endval_int = 30;
    int ret_startval_int, ret_endval_int;

    memset (&testprj, 0, sizeof (testprj));
    memset (&animaparam, 0, sizeof (animaparam));

    testprj.val.startval = (void *)&ret_startval_int;
    testprj.val.endval = (void *)&ret_endval_int;

    animaparam.duration = 3000;
    animaparam.loopcount = 2;
//    animaparam.fps = 301;
    animaparam.direction = MGEFF_DIR_FORWARD;
    animaparam.startval = (void*)&startval_int;
    animaparam.endval = (void*)&endval_int;

//    assert (!mGEffInit());

    handle = mGEffAnimationCreateEx((void*)&testprj, animation_callback, MGEFF_INT, 
            sizeof(int), varmalloc_callback, calcvalue_callback);

    ASSERT_TRUE( handle != NULL );

    mGEffAnimationSetStartValue (handle, animaparam.startval);
    mGEffAnimationSetEndValue (handle, animaparam.endval);

    mGEffAnimationSetProperty (handle, MGEFF_PROP_DURATION, animaparam.duration);
    mGEffAnimationSetProperty (handle, MGEFF_PROP_LOOPCOUNT, animaparam.loopcount);
//    mGEffAnimationSetProperty (handle, MGEFF_PROP_FREQUENCY, animaparam.fps);
    mGEffAnimationSetProperty (handle, MGEFF_PROP_DIRECTION, animaparam.direction);

    mGEffAnimationSetFinishedCb (handle, finished_callback);
    mGEffAnimationSetContext (handle, (void *)&animaparam);

    mGEffAnimationSyncRun(handle);

    EXPECT_TRUE (testprj.val.bsetproperty == TRUE);
    EXPECT_TRUE (testprj.val.bvarmalloc == TRUE);
    EXPECT_TRUE (testprj.val.bcalcvalue == TRUE);

    EXPECT_TRUE (testprj.val.bfinished == TRUE);

    EXPECT_EQ (*(int*)animaparam.startval, *(int*)testprj.val.startval);
    EXPECT_EQ (*(int*)animaparam.endval, *(int*)testprj.val.endval);

    usetime = 1000 * (testprj.times.endtime.tv_sec - testprj.times.starttime.tv_sec) + \
                  (testprj.times.endtime.tv_usec - testprj.times.starttime.tv_usec) / 1000;

    //EXPECT_TRUE ((usetime >= (animaparam.duration * animaparam.loopcount)) &&
    //        (usetime < ((animaparam.duration + 80) * animaparam.loopcount)));
    EXPECT_GE (usetime, animaparam.duration * animaparam.loopcount);
    EXPECT_LT (usetime, (animaparam.duration + 80) * animaparam.loopcount);

    mGEffAnimationDelete (handle);

//    mGEffDeinit();
} /* end test_animation_createex */

void test_animation_delete ()
{
    MGEFF_ANIMATION handle = NULL;

    TESTPRJ testprj;
    ANIMAPARAM animaparam;

//    int usetime;
    int startval_int = 1, endval_int = 30;

    memset (&testprj, 0, sizeof (testprj));
    memset (&animaparam, 0, sizeof (animaparam));

    animaparam.duration = 3000;
    animaparam.loopcount = 1;
//    animaparam.fps = 301;
    animaparam.direction = MGEFF_DIR_FORWARD;
    animaparam.startval = (void*)&startval_int;
    animaparam.endval = (void*)&endval_int;

//    assert (!mGEffInit());

    handle = createex_animation ((void*)&testprj, &animaparam, MGEFF_INT,
            sizeof(int), varmalloc_callback, calcvalue_callback);

    ASSERT_TRUE (handle != NULL);

    mGEffAnimationSyncRun (handle);

    EXPECT_NO_THROW ({
       mGEffAnimationDelete (handle);
    });

    EXPECT_NO_THROW ({
        handle = NULL;
        mGEffAnimationDelete (handle);
    });

//    mGEffDeinit();
} /* end test_animation_delete */

static bool bmotioncurve;

static float motioncurve_callback(float progress)
{
    bmotioncurve = TRUE;

    return progress;
}

void test_animation_setcurveex()
{
    MGEFF_ANIMATION handle = NULL;

    TESTPRJ testprj;
    ANIMAPARAM animaparam;

    int startval_int = 1, endval_int = 50;

    memset (&testprj, 0, sizeof (testprj));
    memset (&animaparam, 0, sizeof (animaparam));

    animaparam.duration = 5000;
    animaparam.loopcount = 1;
//    animaparam.fps = 13;
    animaparam.direction = MGEFF_DIR_FORWARD;
    animaparam.startval = (void*)&startval_int;
    animaparam.endval = (void*)&endval_int;

    bmotioncurve = FALSE;

//    assert (!mGEffInit());

    handle = createex_animation ((void*)&testprj, &animaparam, MGEFF_INT,
            sizeof(int), varmalloc_callback, calcvalue_callback);

    mGEffAnimationSetCurveEx (handle, motioncurve_callback);

    mGEffAnimationSyncRun(handle);

    EXPECT_TRUE (bmotioncurve == TRUE);

    mGEffAnimationDelete (handle);

//    mGEffDeinit();
}

static BOOL execute_setdirection_command( HWND hWnd, int id, DWORD ms )
{
    MGEFF_ANIMATION handle = NULL;

    TESTPRJ *testprj = NULL;

    static int g_timertick = 0;

    assert (g_handle != NULL);

    handle = (MGEFF_ANIMATION)g_handle;

    testprj = (TESTPRJ *)mGEffAnimationGetTarget (handle);

    switch (g_timertick)
    {
        case 20:
            mGEffAnimationSetProperty (handle, MGEFF_PROP_DIRECTION, MGEFF_DIR_BACKWARD);

            KillTimer( hWnd, id );
            break;
        default:
            /* do nothing */
            break;
    }

    return TRUE;
}

void test_animation_setdirection(int keepalive)
{
    MGEFF_ANIMATION handle = NULL;

    TESTPRJ testprj;
    ANIMAPARAM animaparam;

    int usetime;
    int startval_int = 3, endval_int = 632;
    int ret_startval_int, ret_endval_int;

    memset (&testprj, 0, sizeof (testprj));
    memset (&animaparam, 0, sizeof (animaparam));

    animaparam.duration = 8000;
    animaparam.loopcount = 1;
//    animaparam.fps = 1;
    animaparam.direction = MGEFF_DIR_FORWARD;
    animaparam.startval = (void*)&startval_int;
    animaparam.endval = (void*)&endval_int;

    testprj.val.bfinished = FALSE;
    testprj.val.startval = (void *)&ret_startval_int;
    testprj.val.endval = (void *)&ret_endval_int;

//    assert (!mGEffInit());

    handle = createex_animation ((void*)&testprj, &animaparam, MGEFF_INT,
            sizeof(int), varmalloc_callback, calcvalue_callback);

    g_handle = handle;

//    mGEffAnimationSetFinishedCb (handle, finished_callback);

    SetTimerEx( g_hwnd, SETDIRECTION_TIMER_ID + keepalive, 5, execute_setdirection_command );

#if 1
    mGEffAnimationAsyncRun(handle);

    assert (g_hwnd != (HWND)NULL);

    mGEffAnimationWait((void *)&g_hwnd, handle);
#else
    mGEffAnimationSyncRun(handle);
#endif
    EXPECT_TRUE (testprj.val.bfinished == TRUE);
    EXPECT_TRUE (testprj.val.bdirchanged == TRUE);

    EXPECT_EQ (*(int*)animaparam.startval, *(int*)testprj.val.startval);
    //EXPECT_EQ (*(int*)animaparam.endval, *(int*)testprj.val.endval);
    EXPECT_EQ (*(int*)animaparam.startval, *(int*)testprj.val.endval);

    usetime = 1000 * (testprj.times.endtime.tv_sec - testprj.times.starttime.tv_sec) + \
                  (testprj.times.endtime.tv_usec - testprj.times.starttime.tv_usec) / 1000;

    //EXPECT_TRUE ((usetime >= (animaparam.duration * animaparam.loopcount)) &&
    //        (usetime < ((animaparam.duration + 80) * animaparam.loopcount)));
    EXPECT_GE (usetime, animaparam.duration * animaparam.loopcount);
    EXPECT_LT (usetime, (animaparam.duration + 80) * animaparam.loopcount);

    EXPECT_EQ (animaparam.loopcount, testprj.val.loopcount);

    if (keepalive == 1)
    {
        mGEffAnimationDelete (handle);
    }

//    mGEffDeinit();
}

void test_animation_setgetvalue()
{
    MGEFF_ANIMATION handle = NULL;

    TESTPRJ testprj;
    ANIMAPARAM animaparam;

    int startval_int = 50, endval_int = 241;
    int ret_startval_int, ret_endval_int;

    int startval;
    int endval;

    memset (&testprj, 0, sizeof (testprj));
    memset (&animaparam, 0, sizeof (animaparam));

    testprj.val.startval = (void *)&ret_startval_int;
    testprj.val.endval = (void *)&ret_endval_int;

    animaparam.duration = 500;
    animaparam.loopcount = 1;
//    animaparam.fps = 20;
    animaparam.direction = MGEFF_DIR_FORWARD;
    animaparam.startval = (void*)&startval_int;
    animaparam.endval = (void*)&endval_int;

//    assert (!mGEffInit());

    handle = mGEffAnimationCreateEx((void*)&testprj, animation_callback, MGEFF_INT, 
            sizeof(int), varmalloc_callback, calcvalue_callback);

    ASSERT_TRUE( handle != NULL );

    mGEffAnimationSetStartValue (handle, animaparam.startval);
    startval = *(int *)mGEffAnimationGetStartValue (handle);
    EXPECT_EQ (*(int *)animaparam.startval, startval);

    mGEffAnimationSetEndValue (handle, animaparam.endval);
    endval = *(int *)mGEffAnimationGetEndValue (handle);
    EXPECT_EQ (*(int *)animaparam.endval, endval);

    mGEffAnimationSetProperty (handle, MGEFF_PROP_DURATION, animaparam.duration);

    mGEffAnimationSetProperty (handle, MGEFF_PROP_LOOPCOUNT, animaparam.loopcount);

//    mGEffAnimationSetProperty (handle, MGEFF_PROP_FREQUENCY, animaparam.fps);

    mGEffAnimationSetProperty (handle, MGEFF_PROP_DIRECTION, animaparam.direction);

    mGEffAnimationSetFinishedCb (handle, finished_callback);
    mGEffAnimationSetContext (handle, (void *)&animaparam);

    mGEffAnimationSyncRun(handle);

    EXPECT_EQ (*(int*)animaparam.startval, *(int*)testprj.val.startval);
    EXPECT_EQ (*(int*)animaparam.endval, *(int*)testprj.val.endval);

    mGEffAnimationDelete (handle);

//    mGEffDeinit();
}

void test_animation_setgetproperty()
{
    MGEFF_ANIMATION handle = NULL;

    TESTPRJ testprj;
    ANIMAPARAM animaparam;

    int usetime;
    int startval_int = 1, endval_int = 100;
    int ret_startval_int, ret_endval_int;

    int duration;
    int loopcount;
//    int fps;
    int direction;

    memset (&testprj, 0, sizeof (testprj));
    memset (&animaparam, 0, sizeof (animaparam));

    testprj.val.startval = (void *)&ret_startval_int;
    testprj.val.endval = (void *)&ret_endval_int;

    animaparam.duration = 5000;
    animaparam.loopcount = 3;
//    animaparam.fps = 301;
    animaparam.direction = MGEFF_DIR_FORWARD;
    animaparam.startval = (void *)&startval_int;
    animaparam.endval = (void *)&endval_int;

//    assert (!mGEffInit());

    handle = mGEffAnimationCreateEx((void*)&testprj, animation_callback, MGEFF_INT, 
            sizeof(int), varmalloc_callback, calcvalue_callback);

    ASSERT_TRUE( handle != NULL );

    mGEffAnimationSetStartValue (handle, animaparam.startval);
    mGEffAnimationSetEndValue (handle, animaparam.endval);

    mGEffAnimationSetProperty (handle, MGEFF_PROP_DURATION, animaparam.duration);
    duration = mGEffAnimationGetProperty(handle, MGEFF_PROP_DURATION);
    EXPECT_EQ(animaparam.duration, duration);

    mGEffAnimationSetProperty (handle, MGEFF_PROP_LOOPCOUNT, animaparam.loopcount);
    loopcount = mGEffAnimationGetProperty(handle, MGEFF_PROP_LOOPCOUNT);
    EXPECT_EQ(animaparam.loopcount, loopcount);

//    mGEffAnimationSetProperty (handle, MGEFF_PROP_FREQUENCY, animaparam.fps);
//    fps = mGEffAnimationGetProperty(handle, MGEFF_PROP_FREQUENCY);
//    EXPECT_EQ(animaparam.fps, fps);

    mGEffAnimationSetProperty (handle, MGEFF_PROP_DIRECTION, animaparam.direction);
    direction = mGEffAnimationGetProperty(handle, MGEFF_PROP_DIRECTION);
    EXPECT_EQ(animaparam.direction, direction);

    mGEffAnimationSetFinishedCb (handle, finished_callback);
    mGEffAnimationSetContext (handle, (void *)&animaparam);

    mGEffAnimationSetCurLoopChangedCb (handle, calcloopcount_callback);

    mGEffAnimationAsyncRun(handle);

    mGEffAnimationWait ((void *)&g_hwnd, handle);

    EXPECT_EQ (*(int*)animaparam.startval, *(int*)testprj.val.startval);
    EXPECT_EQ (*(int*)animaparam.endval, *(int*)testprj.val.endval);

    usetime = 1000 * (testprj.times.endtime.tv_sec - testprj.times.starttime.tv_sec) + \
                  (testprj.times.endtime.tv_usec - testprj.times.starttime.tv_usec) / 1000;

    //EXPECT_TRUE ((usetime >= (animaparam.duration * animaparam.loopcount)) &&
    //        (usetime < ((animaparam.duration + 80) * animaparam.loopcount)));
    EXPECT_GE (usetime, animaparam.duration * animaparam.loopcount);
    EXPECT_LT (usetime, (animaparam.duration + 80) * animaparam.loopcount);

    EXPECT_EQ (animaparam.loopcount, testprj.val.loopcount);

    mGEffAnimationDelete (handle);

//    mGEffDeinit();
}

void test_animation_setduration()
{
    MGEFF_ANIMATION handle = NULL;

    TESTPRJ testprj;
    ANIMAPARAM animaparam;

    int usetime;
    int startval_int = 1, endval_int = 100;
    int duration, oldduration;

    memset (&testprj, 0, sizeof (testprj));
    memset (&animaparam, 0, sizeof (animaparam));

    animaparam.duration = 5000;
    animaparam.loopcount = 3;
//    animaparam.fps = 301;
    animaparam.direction = MGEFF_DIR_FORWARD;
    animaparam.startval = (void*)&startval_int;
    animaparam.endval = (void*)&endval_int;

//    assert (!mGEffInit());

    handle = mGEffAnimationCreateEx((void*)&testprj, animation_callback, MGEFF_INT, 
            sizeof(int), varmalloc_callback, calcvalue_callback);

    ASSERT_TRUE( handle != NULL );

    mGEffAnimationSetStartValue (handle, animaparam.startval);
    mGEffAnimationSetEndValue (handle, animaparam.endval);

    mGEffAnimationSetDuration (handle, animaparam.duration);
    duration = mGEffAnimationGetProperty(handle, MGEFF_PROP_DURATION);
    EXPECT_EQ(animaparam.duration, duration);

    // reset duration
    oldduration = animaparam.duration;
    animaparam.duration = 10000;

    mGEffAnimationSetDuration (handle, animaparam.duration);
    duration = mGEffAnimationGetProperty(handle, MGEFF_PROP_DURATION);
    EXPECT_EQ (animaparam.duration, duration);
    EXPECT_TRUE (oldduration != duration);
    // end reset duration

    mGEffAnimationSetProperty (handle, MGEFF_PROP_LOOPCOUNT, animaparam.loopcount);

//    mGEffAnimationSetProperty (handle, MGEFF_PROP_FREQUENCY, animaparam.fps);

    mGEffAnimationSetProperty (handle, MGEFF_PROP_DIRECTION, animaparam.direction);

    mGEffAnimationSetFinishedCb (handle, finished_callback);
    mGEffAnimationSetContext (handle, (void *)&animaparam);

    mGEffAnimationSyncRun(handle);

    usetime = 1000 * (testprj.times.endtime.tv_sec - testprj.times.starttime.tv_sec) + \
                  (testprj.times.endtime.tv_usec - testprj.times.starttime.tv_usec) / 1000;

    //EXPECT_TRUE ((usetime >= (animaparam.duration * animaparam.loopcount)) &&
    //        (usetime < ((animaparam.duration + 80) * animaparam.loopcount)));
    EXPECT_GE (usetime, animaparam.duration * animaparam.loopcount);
    EXPECT_LT (usetime, (animaparam.duration + 80) * animaparam.loopcount);

    mGEffAnimationDelete (handle);

//    mGEffDeinit();
}

void test_animation_syncrun (int keepalive)
{
    MGEFF_ANIMATION handle = NULL;

    TESTPRJ testprj;
    ANIMAPARAM animaparam;

    int usetime;
    int startval_int = 3, endval_int = 232;
    int ret_startval_int, ret_endval_int;

    memset (&testprj, 0, sizeof (testprj));
    memset (&animaparam, 0, sizeof (animaparam));

    testprj.val.startval = (void *)&ret_startval_int;
    testprj.val.endval = (void *)&ret_endval_int;

    animaparam.duration = 345;
    animaparam.loopcount = 3;
//    animaparam.fps = 30;
    animaparam.direction = MGEFF_DIR_FORWARD;
    animaparam.startval = (void *)&startval_int;
    animaparam.endval = (void *)&endval_int;

//    assert (!mGEffInit());

    handle = createex_animation ((void*)&testprj, &animaparam, MGEFF_INT,
            sizeof(int), varmalloc_callback, calcvalue_callback);

    mGEffAnimationSetContext (handle, (void *)&animaparam);

    mGEffAnimationSyncRun(handle);

    EXPECT_EQ (*(int*)animaparam.startval, *(int*)testprj.val.startval);
    EXPECT_EQ (*(int*)animaparam.endval, *(int*)testprj.val.endval);

    usetime = 1000 * (testprj.times.endtime.tv_sec - testprj.times.starttime.tv_sec) + \
                  (testprj.times.endtime.tv_usec - testprj.times.starttime.tv_usec) / 1000;

    //EXPECT_TRUE ((usetime >= (animaparam.duration * animaparam.loopcount)) &&
    //        (usetime < ((animaparam.duration + 80) * animaparam.loopcount)));
    EXPECT_GE (usetime, animaparam.duration * animaparam.loopcount);
    EXPECT_LT (usetime, (animaparam.duration + 80) * animaparam.loopcount);

    if (keepalive == 1) {
        mGEffAnimationDelete (handle);
    }

//    mGEffDeinit();
}

void test_animation_asyncrun(int keepalive)
{
    MGEFF_ANIMATION handle = NULL;

    TESTPRJ testprj;
    ANIMAPARAM animaparam;

    int usetime;
    int startval_int = 3, endval_int = 232;
    int ret_startval_int, ret_endval_int;

    memset (&testprj, 0, sizeof (testprj));
    memset (&animaparam, 0, sizeof (animaparam));

    testprj.val.startval = (void *)&ret_startval_int;
    testprj.val.endval = (void *)&ret_endval_int;

    animaparam.duration = 345;
    animaparam.loopcount = 3;
//    animaparam.fps = 301;
    animaparam.direction = MGEFF_DIR_FORWARD;
    animaparam.startval = (void*)&startval_int;
    animaparam.endval = (void*)&endval_int;

//    assert (!mGEffInit());

    handle = createex_animation ((void*)&testprj, &animaparam, MGEFF_INT,
            sizeof(int), varmalloc_callback, calcvalue_callback);

    mGEffAnimationAsyncRun (handle);

    assert (g_hwnd != (HWND)NULL);

    mGEffAnimationWait ((void *)&g_hwnd,handle);

    EXPECT_EQ (*(int*)animaparam.startval, *(int*)testprj.val.startval);
    EXPECT_EQ (*(int*)animaparam.endval, *(int*)testprj.val.endval);

    usetime = 1000 * (testprj.times.endtime.tv_sec - testprj.times.starttime.tv_sec) + \
                  (testprj.times.endtime.tv_usec - testprj.times.starttime.tv_usec) / 1000;

    //EXPECT_TRUE ((usetime >= (animaparam.duration * animaparam.loopcount)) &&
    //        (usetime < ((animaparam.duration + 80) * animaparam.loopcount)));
    EXPECT_GE (usetime, animaparam.duration * animaparam.loopcount);
    EXPECT_LT (usetime, (animaparam.duration + 80) * animaparam.loopcount);

    if (keepalive == 1)
    {
        mGEffAnimationDelete (handle);
    }

//    mGEffDeinit();
}

void animationex_callback1 (MGEFF_ANIMATION handle, void *target, int id, void* value)
{
    animation_callback (handle, target, id, value);
}

void animationex_callback2 (MGEFF_ANIMATION handle, void *target, int id, void* value)
{
    animation_callback (handle, target, id, value);
}

void test_animation_multiasyncrun()
{
    MGEFF_ANIMATION handle_1 = NULL;
    MGEFF_ANIMATION handle_2 = NULL;

    TESTPRJ testprj_1;
    TESTPRJ testprj_2;
    ANIMAPARAM animaparam_1;
    ANIMAPARAM animaparam_2;

    int startval_int_1 = 3, endval_int_1 = 232;
    int startval_int_2 = 40, endval_int_2 = 332;

    int usetime_1;
    int usetime_2;

    memset (&testprj_1, 0, sizeof (testprj_1));
    memset (&testprj_2, 0, sizeof (testprj_2));

    memset (&animaparam_1, 0, sizeof (animaparam_1));
    memset (&animaparam_2, 0, sizeof (animaparam_2));

    animaparam_1.duration = 5000;
    animaparam_1.loopcount = 1;
//    animaparam_1.fps = 301;
    animaparam_1.direction = MGEFF_DIR_FORWARD;
    animaparam_1.startval = (void*)&startval_int_1;
    animaparam_1.endval = (void*)&endval_int_1;

    animaparam_2.duration = 8000;
    animaparam_2.loopcount = 1;
//    animaparam_2.fps = 101;
    animaparam_2.direction = MGEFF_DIR_FORWARD;
    animaparam_2.startval = (void*)&startval_int_2;
    animaparam_2.endval = (void*)&endval_int_2;

//    assert (!mGEffInit());

    handle_1 = createex_animation ((void*)&testprj_1, &animaparam_1, MGEFF_INT,
            sizeof(int), varmalloc_callback, calcvalue_callback,
            animationex_callback1);

    handle_2 = createex_animation ((void*)&testprj_2, &animaparam_2, MGEFF_INT,
            sizeof(int), varmalloc_callback, calcvalue_callback,
            animationex_callback2);

    mGEffAnimationAsyncRun (handle_1);

    mGEffAnimationAsyncRun (handle_2);

    assert (g_hwnd != (HWND)NULL);

    mGEffAnimationWait ((void *)&g_hwnd, handle_1);
    mGEffAnimationWait ((void *)&g_hwnd, handle_2);

    usetime_1 = 1000 * (testprj_1.times.endtime.tv_sec - testprj_1.times.starttime.tv_sec) + \
                  (testprj_1.times.endtime.tv_usec - testprj_1.times.starttime.tv_usec) / 1000;

    //EXPECT_TRUE ((usetime >= (animaparam.duration * animaparam.loopcount)) &&
    //        (usetime < ((animaparam.duration + 80) * animaparam.loopcount)));
    EXPECT_GE (usetime_1, animaparam_1.duration * animaparam_1.loopcount);
    EXPECT_LT (usetime_1, (animaparam_1.duration + 80) * animaparam_1.loopcount);

    usetime_2 = 1000 * (testprj_2.times.endtime.tv_sec - testprj_2.times.starttime.tv_sec) + \
                  (testprj_2.times.endtime.tv_usec - testprj_2.times.starttime.tv_usec) / 1000;

    //EXPECT_TRUE ((usetime >= (animaparam.duration * animaparam.loopcount)) &&
    //        (usetime < ((animaparam.duration + 80) * animaparam.loopcount)));
    EXPECT_GE (usetime_2, animaparam_2.duration * animaparam_2.loopcount);
    EXPECT_LT (usetime_2, (animaparam_2.duration + 80) * animaparam_2.loopcount);

    mGEffAnimationDelete (handle_1);
    mGEffAnimationDelete (handle_2);

//    mGEffDeinit();
}

