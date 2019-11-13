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
#include <gtest/gtest.h>
#include <mgeff/mgeff.h>
#include <sys/times.h>
#include <stdlib.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#include <sys/times.h>

#define MGEFF_MINOR_dummy "dummy"
#define MGEFF_EFFECTOR_DUMMY mGEffStr2Key(MGEFF_MINOR_dummy)

int effType = MGEFF_INT;
unsigned int status = 0;
    HWND hMainWnd;

#define INIT 0x1
#define BEGINDRAW 0x2
#define ONDRAW 0x4
#define ENDDRAW 0x8
#define FINALIZE 0x10

static MGEFF_EFFECTOR dummy_init (MGEFF_EFFECTOR effector)
{
    EXPECT_TRUE ((status & INIT) == 0);
    unsigned int flag = 0;
    EXPECT_TRUE ((status & flag ) == flag);

    status |= INIT;
    return effector;
}

static void
dummy_finalize (MGEFF_EFFECTOR effector)
{
    EXPECT_TRUE ((status & FINALIZE) == 0);
    unsigned int flag = INIT | BEGINDRAW | ONDRAW | ENDDRAW;
    EXPECT_TRUE ((status & flag ) == flag);
    status |= FINALIZE;
}

static int
dummy_setproperty (MGEFF_EFFECTOR effector, int property_id, int value)
{
    return 0;
}

static int
dummy_getproperty (MGEFF_EFFECTOR effector, int property_id, int *value)
{
    return 0;
}
static void
dummy_begindraw (MGEFF_ANIMATION anim, MGEFF_EFFECTOR effector)
{
    EXPECT_TRUE ((status & BEGINDRAW) == 0);
    unsigned int flag = INIT;
    EXPECT_TRUE ((status & flag ) == flag);
    status |= BEGINDRAW;
}

static void
dummy_ondraw (MGEFF_ANIMATION anim, MGEFF_EFFECTOR effector, HDC sink_dc, int id, void *value)
{
    unsigned int flag = INIT | BEGINDRAW;
    EXPECT_TRUE ((status & flag) == flag);
    status |= ONDRAW;
}

static void
dummy_enddraw (MGEFF_ANIMATION anim, MGEFF_EFFECTOR effector)
{
    EXPECT_TRUE ((status & ENDDRAW) == 0);

    unsigned int flag = INIT | BEGINDRAW | ONDRAW;
    EXPECT_TRUE ((status & flag) == flag);
    status |= ENDDRAW;
}

MGEFF_EFFECTOROPS dummy_effector = {
    MGEFF_MINOR_dummy,
    MGEFF_FLOAT,
    dummy_init,
    dummy_finalize,
    dummy_ondraw,
    dummy_begindraw,
    dummy_enddraw,
    dummy_setproperty,
    dummy_getproperty,
};

void neweffapi_testing_int (int runtype) 
{
    int ret = 0;
    unsigned int flag = 0;

    int duration = 1000;

    status = 0;
    ASSERT_TRUE(!mGEffInit());
    mGEffEffectorRegister (&dummy_effector);
    MGEFF_EFFECTOR effector = mGEffEffectorCreate (MGEFF_EFFECTOR_DUMMY);
    flag |= INIT;
    EXPECT_TRUE (status == flag);
    ASSERT_TRUE (effector != NULL);

    HDC srcdc1 = CreateMemDC(1, 1, 32, MEMDC_FLAG_SWSURFACE, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    HDC srcdc2 = CreateMemDC(1, 1, 32, MEMDC_FLAG_SWSURFACE, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    HDC sinkdc = CreateMemDC(1, 1, 32, MEMDC_FLAG_SWSURFACE, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);

    MGEFF_SOURCE src1 = mGEffCreateSource (srcdc1);
    MGEFF_SOURCE src2 = mGEffCreateSource (srcdc2);
    MGEFF_SINK   sink = mGEffCreateHDCSink (sinkdc);

    mGEffEffectorAppendSource (effector, src1);
    mGEffEffectorAppendSource (effector, src2);

    mGEffSetBufferSink (sink, srcdc1);
    mGEffEffectorSetSink (effector, sink);

    MGEFF_ANIMATION anim = mGEffAnimationCreateWithEffector (effector);
    mGEffAnimationSetDuration (anim, duration);

    if (runtype == 0) {
        mGEffAnimationAsyncRun (anim);
        ret = mGEffAnimationWait (&hMainWnd, anim);
    }
    else {
        mGEffAnimationSyncRun (anim);
    }

    flag |= BEGINDRAW | ONDRAW | ENDDRAW;
    EXPECT_TRUE ((flag & status) == flag);
    mGEffEffectorDelete (effector);
    flag |= FINALIZE;
    EXPECT_TRUE ((flag & status) == flag);
    mGEffEffectorUnRegister (&dummy_effector);
    DeleteMemDC(srcdc1);
    DeleteMemDC(srcdc2);
    DeleteMemDC(sinkdc);

}

TEST(EffectorInterfaceTest, ASyncRun) {
    neweffapi_testing_int (0);
}

TEST(EffectorInterfaceTest, SyncRun) {
    neweffapi_testing_int (1);
}

int main (int argc, char* argv[]) 
{
    int ret;
    //MSG Msg;
    InitGUI (0, NULL);

    MAINWINCREATE CreateInfo;

#ifdef  _MGRM_PROCESSES
    JoinLayer (NAME_DEF_LAYER, "my effectors", 0, 0);
#endif

    //CreateInfo.dwStyle = WS_DISABLED;
    CreateInfo.dwStyle = 0;
    CreateInfo.dwExStyle = WS_EX_NONE;
    CreateInfo.spCaption = "hiding win";
    CreateInfo.hMenu = 0;
    CreateInfo.hCursor = GetSystemCursor (0);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = DefaultMainWinProc;
    CreateInfo.lx = 0;
    CreateInfo.ty = 0;
    CreateInfo.rx = 320;
    CreateInfo.by = 240;
    CreateInfo.iBkColor = COLOR_lightwhite;
    CreateInfo.dwAddData = 0;
    CreateInfo.hHosting = HWND_DESKTOP;

    hMainWnd = CreateMainWindow (&CreateInfo);

    ::testing::InitGoogleTest(&argc, argv);
    ret = RUN_ALL_TESTS();

    /*
    while (GetMessage (&Msg, hMainWnd)) {
        TranslateMessage (&Msg);
        DispatchMessage (&Msg);
    }
    */

    return ret;
}

