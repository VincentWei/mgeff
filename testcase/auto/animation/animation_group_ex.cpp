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
#include "animation_group_ex.h"

extern HWND g_hwnd;
static pthread_t thread_id;

void *pause_resume(void *arg)
{
    MGEFF_ANIMATION handle;
    int fb1, fb2;

    handle = (MGEFF_ANIMATION)arg;

    mGEffAnimationPause(handle);

    fb1 = mGEffAnimationGetProperty(handle, MGEFF_PROP_CURFRAME);

    usleep(1000*1000);
    
    fb2 = mGEffAnimationGetProperty(handle, MGEFF_PROP_CURFRAME);

    printf("%d %d\n", fb1, fb2);
    EXPECT_TRUE(fb1 == fb2);

    mGEffAnimationResume(handle);
    usleep(1000*1000);

    fb2 = mGEffAnimationGetProperty(handle, MGEFF_PROP_CURFRAME);

    EXPECT_TRUE(fb1 != fb2);

    return NULL;
} 

static void check_pauseresume_callback(MGEFF_ANIMATION handle, void* target, int id, void* value)
{
    TESTPRJ *testprj = (TESTPRJ*)mGEffAnimationGetTarget (handle);
    /*public part*/
    if (mGEffAnimationGetProperty (handle, MGEFF_PROP_CURFRAME) == 0)
    {
        gettimeofday (&testprj->times.starttime, NULL);
    }

    gettimeofday (&testprj->times.endtime, NULL);
    ++testprj->val.loopcount;

    if(mGEffAnimationGetProperty(handle, MGEFF_PROP_CURFRAME) == 10)
    {
        pthread_create(&thread_id, NULL, pause_resume, (void *)handle);
    }
}

void **p;

MGEFF_ANIMATION create_group(enum EffAnimationType type, MGEFF_SETPROPERTY_CB setproperty_callback)
{
    int i;
    TESTPRJ testprj[MGEFF_MAX];
    MGEFF_ANIMATION handles[MGEFF_MAX];
    ANIMAPARAM animaparam[MGEFF_MAX];
    MGEFF_ANIMATION animation_group = NULL;
    double startval_double = 0, endval_double = 1000000;
    float startval_float = 0.0f, endval_float = 10.0f;
    int startval_int = 3, endval_int = 232;

    double ret_startval_double, ret_endval_double;
    float ret_startval_float, ret_endval_float;
    int ret_startval_int, ret_endval_int;

    printf ("c %u\n", (unsigned int)&testprj[0] );
    memset ((void *)testprj, 0, sizeof(testprj));
    memset ((void *)animaparam, 0, sizeof(animaparam));

//    mGEffInit();

    /* MGEFF_INT */
    {
        testprj[0].val.loopcount = 10;
        p = &(testprj[0].val.startval);
        testprj[0].val.startval = (void *)&ret_startval_int;
        testprj[0].val.endval = (void *)&ret_endval_int;
        printf ("%u\n", (unsigned int)testprj[0].val.startval);
        printf ("%u\n", (unsigned int)&(testprj[0].val));
        printf ("%u\n", (unsigned int)&ret_startval_int);
        printf ("%u\n", (unsigned int)&ret_endval_int);

        animaparam[0].duration = 700;
        animaparam[0].loopcount = 1;
        animaparam[0].fps = 874;
        animaparam[0].startval = (void*)&startval_int;
        animaparam[0].endval = (void*)&endval_int;
        if(setproperty_callback != NULL)
        {
            handles[0] = createex_animation ((void*)&testprj[0], &animaparam[0], MGEFF_INT,
                                         sizeof(int), varmalloc_callback, calcvalue_callback,
                                         setproperty_callback);
        } else {
            handles[0] = createex_animation ((void*)&testprj[0], &animaparam[0], MGEFF_INT,
                                         sizeof(int), varmalloc_callback, calcvalue_callback);
        }
        mGEffAnimationSetContext (handles[0], (void *)&animaparam[0]);
    }

    /* MGEFF_FLOAT */
    {
        testprj[1].val.startval = (void *)&ret_startval_float;
        testprj[1].val.endval = (void *)&ret_endval_float;

        animaparam[1].duration = 1000;
        animaparam[1].loopcount = 8;
        animaparam[1].fps = 113;
        animaparam[1].startval = (void *)&startval_float;
        animaparam[1].endval = (void *)&endval_float;
        if(setproperty_callback != NULL)
        {
            handles[1] = createex_animation ((void*)&testprj[1], &animaparam[1], MGEFF_FLOAT,
                                         sizeof(float), varmalloc_callback, calcvalue_callback,
                                         setproperty_callback);
        } else {
            handles[1] = createex_animation ((void*)&testprj[1], &animaparam[1], MGEFF_FLOAT,
                                         sizeof(float), varmalloc_callback, calcvalue_callback);
        }
        mGEffAnimationSetContext (handles[1], (void *)&animaparam[1]);
    }

    /* MGEFF_DOUBLE */
    {
        testprj[2].val.startval = (void *)&ret_startval_double;
        testprj[2].val.endval = (void *)&ret_endval_double;

        animaparam[2].duration = 500;
        animaparam[2].loopcount = 4;
        animaparam[2].fps = 213;
        animaparam[2].startval = (void *)&startval_double;
        animaparam[2].endval = (void *)&endval_double;
        if(setproperty_callback != NULL)
        {
            handles[2] = createex_animation ((void*)&testprj[2], &animaparam[2], MGEFF_DOUBLE,
                                         sizeof(double), varmalloc_callback, calcvalue_callback,
                                         setproperty_callback);
        } else {
            handles[2] = createex_animation ((void*)&testprj[2], &animaparam[2], MGEFF_DOUBLE,
                                         sizeof(double), varmalloc_callback, calcvalue_callback);
        }
        mGEffAnimationSetContext (handles[2], (void *)&animaparam[2]);
    }

    animation_group = mGEffAnimationCreateGroup(type);

    for (i=0; i<3; i++)
        mGEffAnimationAddToGroup (animation_group, handles[i]);

    return animation_group;
}

void test_animation_group_create(enum EffAnimationType type)
{
    MGEFF_ANIMATION handle = NULL;

    handle = create_group(type, NULL); 

    EXPECT_TRUE (handle);

    mGEffAnimationSyncRun (handle);

    mGEffAnimationDelete (handle);
//    mGEffDeinit();
}

void test_animation_group_pause_resume(enum EffAnimationType type)
{
    MGEFF_ANIMATION handle = NULL;
    
    handle = create_group (type, check_pauseresume_callback); 

    mGEffAnimationSyncRun (handle);

    pthread_join (thread_id, NULL);

    mGEffAnimationDelete (handle);
}

static void *check_stop_command_pthread (void *arg)
{
    int stopframe1 = 0;
    int stopframe2 = 0;

    MGEFF_ANIMATION handle = (MGEFF_ANIMATION)arg;
    assert (handle != NULL);

    TESTPRJ *testprj = (TESTPRJ*)mGEffAnimationGetTarget (handle);

    mGEffAnimationStop(handle);
    usleep (0);

    stopframe1 = mGEffAnimationGetProperty(handle, MGEFF_PROP_CURFRAME);

    usleep (1000 * 1000);/*sleep 1s*/

    stopframe2 = mGEffAnimationGetProperty(handle, MGEFF_PROP_CURFRAME);

    EXPECT_EQ(stopframe1, stopframe2);

    EXPECT_TRUE(testprj->val.bfinished == TRUE);

    return NULL;
}

static void check_stop_callback (MGEFF_ANIMATION handle, void* target, int id, void* value)
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
    if (mGEffAnimationGetProperty(handle, MGEFF_PROP_CURFRAME) == 10) {
        pthread_create(&thread_id, NULL, check_stop_command_pthread, (void *)handle);
    }
}

void test_animation_group_stop(enum EffAnimationType type)
{
    MGEFF_ANIMATION handle = NULL;

    handle = create_group(type, check_stop_callback);

    ASSERT_TRUE( handle != NULL );

//    mGEffAnimationSetFinishedCb (handle, finished_callback);

    mGEffAnimationSyncRun (handle);

    pthread_join(thread_id, NULL);

    mGEffAnimationDelete (handle);
//    mGEffDeinit();
}

void test_animation_group_delete(enum EffAnimationType type)
{
    MGEFF_ANIMATION handle = NULL;
    
    handle = create_group(type, NULL);

    mGEffAnimationSyncRun (handle);

    mGEffAnimationDelete (handle);

    //ASSERT_DEATH(mGEffAnimationDelete (handle), "");

//    mGEffDeinit();
}

void test_animation_group_wait(enum EffAnimationType type)
{
    MGEFF_ANIMATION h1 = NULL;
    MGEFF_ANIMATION h2 = NULL;
    int fb1, fb2;

    assert (g_hwnd != (HWND)NULL);

    h1 = create_group(type, NULL);
    h2 = create_group(type, NULL);

    mGEffAnimationAsyncRun(h1);

    mGEffAnimationAsyncRun(h2);

    mGEffAnimationWait((void *)&g_hwnd, h1);
    mGEffAnimationWait((void *)&g_hwnd, h2);

    fb1 = mGEffAnimationGetProperty(h1, MGEFF_PROP_CURFRAME);
    usleep(1000*1000);
    fb2 = mGEffAnimationGetProperty(h2, MGEFF_PROP_CURFRAME);

    EXPECT_TRUE(fb1 == fb2);

    mGEffAnimationDelete (h1);
    mGEffAnimationDelete (h2);
//    mGEffDeinit();
}




