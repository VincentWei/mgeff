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
#include "animation_cb.h"

static void finish_callback(MGEFF_ANIMATION handle)
{
    TESTPRJ *testprj = (TESTPRJ*)mGEffAnimationGetTarget (handle);
    testprj->val.bfinished = true;
}

static void statechanged_callback(MGEFF_ANIMATION handle, EffState news, EffState olds)
{
    TESTPRJ *testprj = (TESTPRJ*)mGEffAnimationGetTarget (handle);
    testprj->val.bstatechanged = true;
}

static void curloopchanged_callback(MGEFF_ANIMATION handle)
{
    TESTPRJ *testprj = (TESTPRJ*)mGEffAnimationGetTarget (handle);
    testprj->val.bcurloopchanged = true;
}

static void dirchanged_callback(MGEFF_ANIMATION handle)
{
    TESTPRJ *testprj = (TESTPRJ*)mGEffAnimationGetTarget (handle);
    testprj->val.bdirchanged = true;
}

static void setproperty_callback(MGEFF_ANIMATION handle, void *target, int id, void *value)
{
    TESTPRJ *testprj = (TESTPRJ*)mGEffAnimationGetTarget (handle);
    testprj->val.bsetproperty = true;
}

void test_animation_finish_callback()
{
    MGEFF_ANIMATION handle = NULL;

    TESTPRJ testprj;
    ANIMAPARAM animaparam;

    //int usetime;
    int startval_int = 3, endval_int = 232;

    memset (&testprj, 0, sizeof (testprj));
    memset (&animaparam, 0, sizeof (animaparam));

    animaparam.duration = 325;
    animaparam.loopcount = 5;
    animaparam.fps = 13;
    animaparam.direction = MGEFF_DIR_FORWARD;
    animaparam.startval = (void*)&startval_int;
    animaparam.endval = (void*)&endval_int;

    handle = create_animation ((void*)&testprj, &animaparam, MGEFF_INT);

    mGEffAnimationSetFinishedCb (handle, finish_callback);

    mGEffAnimationSyncRun(handle);
    
    EXPECT_TRUE (testprj.val.bfinished);

    mGEffAnimationDelete (handle);
}

void test_animation_statechanged_callback()
{
    MGEFF_ANIMATION handle = NULL;

    TESTPRJ testprj;
    ANIMAPARAM animaparam;

    //int usetime;
    int startval_int = 3, endval_int = 232;

    memset (&testprj, 0, sizeof (testprj));
    memset (&animaparam, 0, sizeof (animaparam));

    animaparam.duration = 325;
    animaparam.loopcount = 5;
    animaparam.fps = 13;
    animaparam.direction = MGEFF_DIR_FORWARD;
    animaparam.startval = (void*)&startval_int;
    animaparam.endval = (void*)&endval_int;

    handle = create_animation ((void*)&testprj, &animaparam, MGEFF_INT);

    mGEffAnimationSetStateChangedCb (handle, statechanged_callback);

    mGEffAnimationSyncRun(handle);
    
    mGEffAnimationSetProperty(handle, MGEFF_PROP_STATE, MGEFF_STATE_PAUSED);

    EXPECT_TRUE (testprj.val.bstatechanged);

    mGEffAnimationDelete (handle);
}

void test_animation_curloopchanged_callback()
{
    MGEFF_ANIMATION handle = NULL;

    TESTPRJ testprj;
    ANIMAPARAM animaparam;

    //int usetime;
    int startval_int = 3, endval_int = 232;

    memset (&testprj, 0, sizeof (testprj));
    memset (&animaparam, 0, sizeof (animaparam));

    animaparam.duration = 325;
    animaparam.loopcount = 5;
    animaparam.fps = 13;
    animaparam.direction = MGEFF_DIR_FORWARD;
    animaparam.startval = (void*)&startval_int;
    animaparam.endval = (void*)&endval_int;

    handle = create_animation ((void*)&testprj, &animaparam, MGEFF_INT);

    mGEffAnimationSetCurLoopChangedCb (handle, curloopchanged_callback);

//    mGEffAnimationSetProperty(handle, MGEFF_PROP_CURLOOP, 3);

    mGEffAnimationSyncRun(handle);
    
    EXPECT_TRUE (testprj.val.bstatechanged);

    mGEffAnimationDelete (handle);
}

void test_animation_dirchanged_callback()
{
    MGEFF_ANIMATION handle = NULL;

    TESTPRJ testprj;
    ANIMAPARAM animaparam;

    //int usetime;
    int startval_int = 3, endval_int = 232;

    memset (&testprj, 0, sizeof (testprj));
    memset (&animaparam, 0, sizeof (animaparam));

    animaparam.duration = 325;
    animaparam.loopcount = 5;
    animaparam.fps = 13;
    animaparam.direction = MGEFF_DIR_FORWARD;
    animaparam.startval = (void*)&startval_int;
    animaparam.endval = (void*)&endval_int;

    handle = create_animation ((void*)&testprj, &animaparam, MGEFF_INT);

    mGEffAnimationSetProperty(handle, MGEFF_PROP_DIRECTION, MGEFF_DIR_BACKWARD);

    mGEffAnimationSetDirChangedCb(handle, dirchanged_callback);

    mGEffAnimationSyncRun(handle);

    EXPECT_TRUE (testprj.val.bdirchanged);

    mGEffAnimationDelete (handle);
}

void test_animation_setproperty_callback()
{
    MGEFF_ANIMATION handle = NULL;

    TESTPRJ testprj;
    ANIMAPARAM animaparam;

    //int usetime;
    int startval_int = 3, endval_int = 232;

    memset (&testprj, 0, sizeof (testprj));
    memset (&animaparam, 0, sizeof (animaparam));

    animaparam.duration = 325;
    animaparam.loopcount = 5;
    animaparam.fps = 13;
    animaparam.direction = MGEFF_DIR_FORWARD;
    animaparam.startval = (void*)&startval_int;
    animaparam.endval = (void*)&endval_int;

    handle = create_animation ((void*)&testprj, &animaparam, MGEFF_INT,
            setproperty_callback);

    mGEffAnimationSyncRun(handle);

    EXPECT_TRUE (testprj.val.bsetproperty);

    mGEffAnimationDelete (handle);
}

void test_animation_varmalloc_calcvalue_callback()
{
    MGEFF_ANIMATION handle = NULL;

    TESTPRJ testprj;
    ANIMAPARAM animaparam;

    //int usetime;
    int startval_int = 3, endval_int = 232;

    memset (&testprj, 0, sizeof (testprj));
    memset (&animaparam, 0, sizeof (animaparam));

    animaparam.duration = 325;
    animaparam.loopcount = 5;
    animaparam.fps = 13;
    animaparam.direction = MGEFF_DIR_FORWARD;
    animaparam.startval = (void*)&startval_int;
    animaparam.endval = (void*)&endval_int;

    handle = createex_animation ((void*)&testprj, &animaparam, MGEFF_INT,
            sizeof(int), varmalloc_callback, calcvalue_callback);

    mGEffAnimationSyncRun(handle);
    
    EXPECT_TRUE (testprj.val.bvarmalloc);
    EXPECT_TRUE (testprj.val.bcalcvalue);

    mGEffAnimationDelete (handle);
}

#if 0
void test_animation_malloc_callback()
{

}
#endif

