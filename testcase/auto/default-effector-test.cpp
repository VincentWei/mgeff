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
#include <getopt.h>
#include <stdarg.h>
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

int duration = 200;
HDC g_hdc_pic[2];
HWND g_hwnd;
BOOL g_display=false;

static HDC createDCByPicture (HDC hdc, const char *path, int w, int h)
{
    BITMAP bmp;
    HDC dc;

    if (LoadBitmap (hdc, &bmp, path))
        return HDC_INVALID;

    if (w == 0)
        w = bmp.bmWidth;
    if (h == 0)
        h = bmp.bmHeight;

    dc = CreateCompatibleDCEx (hdc, w, h);
    FillBoxWithBitmap (dc, 0, 0, w, h, &bmp);
    UnloadBitmap (&bmp);
    return dc;
}

void running_effector_window (const char *messagestr, int runtype, int effectortype, BOOL bBufSink, BOOL usedc,
        int property_id, int value)
{
    int ret = 0;
    MGEFF_SINK sink;
    HDC windc;

    MGEFF_EFFECTOR effector = mGEffEffectorCreate (effectortype);
    EXPECT_TRUE (effector != NULL);

    MGEFF_SOURCE src1 = mGEffCreateSource (g_hdc_pic[0]);
    MGEFF_SOURCE src2 = mGEffCreateSource (g_hdc_pic[1]);
    
    if (property_id) {
        int readval;
        mGEffEffectorSetProperty (effector, property_id, value);
        mGEffEffectorGetProperty (effector, property_id, &readval);
        EXPECT_EQ (value, readval);
    }

    usedc = 1;
    if (usedc) {
        windc = BeginPaint (g_hwnd);
        sink = mGEffCreateHDCSink (windc);
    }
    else {
        sink = mGEffCreateHwndSink (g_hwnd);
    }

    mGEffEffectorAppendSource (effector, src1);
    mGEffEffectorAppendSource (effector, src2);

    if (bBufSink)
        mGEffSetBufferSink (sink, g_hdc_pic[0]);

    mGEffEffectorSetSink (effector, sink);

    MGEFF_ANIMATION anim = mGEffAnimationCreateWithEffector (effector);
    mGEffAnimationSetDuration (anim, duration);

    if (runtype == 0) {
        mGEffAnimationAsyncRun (anim);
        ret = mGEffAnimationWait (&g_hwnd, anim);
    }
    else {
        mGEffAnimationSyncRun (anim);
    }

    if (usedc) {
        EndPaint (g_hwnd, windc);
    }
    mGEffEffectorDelete (effector);
    
    if (g_display)
    {
        ret = MessageBox (g_hwnd, messagestr, "Asking", MB_YESNO);
        EXPECT_TRUE (ret == IDYES);
    }
}

void running_effector_window (const char *messagestr, int runtype, int effectortype, BOOL bBufSink, BOOL usedc)
{
    running_effector_window (messagestr, runtype, effectortype, bBufSink, usedc, 0, 0);
}


void deinit_effector_test (void)
{
    DeleteMemDC (g_hdc_pic[0]);
    DeleteMemDC (g_hdc_pic[1]);
    DestroyMainWindow (g_hwnd);
}

HWND init_effector_test (void)
{
    MAINWINCREATE CreateInfo;

#ifdef  _MGRM_PROCESSES
    JoinLayer (NAME_DEF_LAYER, "my effectors", 0, 0);
#endif

    CreateInfo.dwStyle = WS_VISIBLE;
    CreateInfo.dwExStyle = WS_EX_NONE;
    CreateInfo.spCaption = "main test page";
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

    g_hwnd = CreateMainWindow (&CreateInfo);

    g_hdc_pic[0] = createDCByPicture (HDC_SCREEN, "res/1.png", 0, 0);
    g_hdc_pic[1] = createDCByPicture (HDC_SCREEN, "res/2.png", 0, 0);

    assert (!mGEffInit());

#if 0
    MSG Msg;
    while (GetMessage (&Msg, g_hwnd)) {
        TranslateMessage (&Msg);
        DispatchMessage (&Msg);
    }
#endif

    return g_hwnd;
}

/**/
TEST(EffectorZip, SyncRun) {
    running_effector_window ("test zip of sync, can't you see it?", 0, MGEFF_EFFECTOR_ZIP, 0, 0);
}
TEST(EffectorZip, ASyncRun) {
    running_effector_window ("test zip of async, can't you see it?", 1, MGEFF_EFFECTOR_ZIP, 0, 0);
}
TEST(EffectorZip, SyncRun_sinkbuf) {
    running_effector_window ("test zip of sync with sinkbuf, can't you see it?", 1, MGEFF_EFFECTOR_ZIP, 1, 0);
}
TEST(EffectorZip, SyncRun_sinkbuf_dcsink) {
    running_effector_window ("test zip of sync with sinkbuf, use dc sink, can't you see it?", 1, MGEFF_EFFECTOR_ZIP, 1, 1);
}
TEST(EffectorZip, SyncRun_sinkbuf_dcsink_left2right) {
    running_effector_window ("test zip of sync with sinkbuf, use dc sink, left to right, can't you see it?", \
            1, MGEFF_EFFECTOR_ZIP, 1, 1,\
            MGEFF_PROPERTY_DIRECTION, MGEFF_DIRECTION_LEFT2RIGHT);
}
TEST(EffectorZip, SyncRun_sinkbuf_dcsink_right2left) {
    running_effector_window ("test zip of sync with sinkbuf, use dc sink, right to left, can't you see it?", \
            1, MGEFF_EFFECTOR_ZIP, 1, 1,\
            MGEFF_PROPERTY_DIRECTION, MGEFF_DIRECTION_RIGHT2LEFT);
}
TEST(EffectorZip, SyncRun_sinkbuf_dcsink_top2bottom) {
    running_effector_window ("test zip of sync with sinkbuf, use dc sink, top to right, can't you see it?", \
            1, MGEFF_EFFECTOR_ZIP, 1, 1,\
            MGEFF_PROPERTY_DIRECTION, MGEFF_DIRECTION_TOP2BOTTOM);
}
TEST(EffectorZip, SyncRun_sinkbuf_dcsink_bottom2top) {
    running_effector_window ("test zip of sync with sinkbuf, use dc sink, bottom to top, can't you see it?", \
            1, MGEFF_EFFECTOR_ZIP, 1, 1,\
            MGEFF_PROPERTY_DIRECTION, MGEFF_DIRECTION_BOTTOM2TOP);
}

/**/
TEST(EffectorFlip, SyncRun) {
    running_effector_window ("test flip of sync, can't you see it?", 0, MGEFF_EFFECTOR_FLIP, 0, 0);
}
TEST(EffectorFlip, ASyncRun) {
    running_effector_window ("test flip of async, can't you see it?", 1, MGEFF_EFFECTOR_FLIP, 0, 0);
}
TEST(EffectorFlip, SyncRun_sinkbuf) {
    running_effector_window ("test flip of sync sinkbuf, can't you see it?", 0, MGEFF_EFFECTOR_FLIP, 1, 0);
}
TEST(EffectorFlip, SyncRun_sinkbuf_dcsink) {
    running_effector_window ("test flip of sync sinkbuf, use dc to sink, can't you see it?", \
            0, MGEFF_EFFECTOR_FLIP, 1, 1);
}

/**/
TEST(EffectorAlpha, SyncRun) {
    running_effector_window ("test alpha of sync, can't you see it?", 0, MGEFF_EFFECTOR_ALPHA, 0, 0);
}
TEST(EffectorAlpha, ASyncRun) {
    running_effector_window ("test alpha of async, can't you see it?", 1, MGEFF_EFFECTOR_ALPHA, 0, 0);
}
TEST(EffectorAlpha, SyncRun_sinkbuf) {
    running_effector_window ("test alpha of sync with sinkbuf, can't you see it?", 0, MGEFF_EFFECTOR_ALPHA, 1, 0);
}
TEST(EffectorAlpha, SyncRun_sinkbuf_dcsink) {
    running_effector_window ("test alpha of sync with sinkbuf, use dc to sink, can't you see it?", \
            0, MGEFF_EFFECTOR_ALPHA, 1, 1);
}

/**/
TEST(EffectorScroll, SyncRun) {
    running_effector_window ("test scroll of sync, can't you see it?", 0, MGEFF_EFFECTOR_SCROLL, 0, 0);
}
TEST(EffectorScroll, ASyncRun) {
    running_effector_window ("test scroll of async, can't you see it?", 1, MGEFF_EFFECTOR_SCROLL, 0, 0);
}
TEST(EffectorScroll, SyncRun_sinkbuf) {
    running_effector_window ("test scroll of sync with sinkbuf, can't you see it?", 0, MGEFF_EFFECTOR_SCROLL, 0, 0);
}
TEST(EffectorScroll, SyncRun_sinkbuf_dcsink) {
    running_effector_window ("test scroll of sync with sinkbuf, use dc to sink, can't you see it?", 
            0, MGEFF_EFFECTOR_SCROLL, 0, 1);
}

/**/
TEST(EffectorRotate, SyncRun) {
    running_effector_window ("test rotate of sync, can't you see it?", 0, MGEFF_EFFECTOR_CUBIC_ROTATE, 0, 0);
}
TEST(EffectorRotate, ASyncRun) {
    running_effector_window ("test rotate of async, can't you see it?", 1, MGEFF_EFFECTOR_CUBIC_ROTATE, 0, 0);
}
TEST(EffectorRotate, SyncRun_sinkbuf) {
    running_effector_window ("test rotate of sync with sinkbuf, can't you see it?", 1, MGEFF_EFFECTOR_CUBIC_ROTATE, 1, 0);
}
TEST(EffectorRotate, SyncRun_sinkbuf_dcsink) {
    running_effector_window ("test rotate of sync with sinkbuf, use dc to sink, can't you see it?", \
            1, MGEFF_EFFECTOR_CUBIC_ROTATE, 1, 1);
}

/**/
TEST(EffectorZoom, SyncRun) {
    running_effector_window ("test zoom of sync, can't you see it?", 0, MGEFF_EFFECTOR_ZOOM, 0, 0);
}
TEST(EffectorZoom, ASyncRun) {
    running_effector_window ("test zoom of async, can't you see it?", 1, MGEFF_EFFECTOR_ZOOM, 0, 0);
}
TEST(EffectorZoom, SyncRun_sinkbuf) {
    running_effector_window ("test zoom of sync with sinkbuf, can't you see it?", 0, MGEFF_EFFECTOR_ZOOM, 1, 0);
}
TEST(EffectorZoom, SyncRun_sinkbuf_dcsink) {
    running_effector_window ("test zoom of sync with sinkbuf, use dc to sink, can't you see it?", \
            0, MGEFF_EFFECTOR_ZOOM, 1, 1);
}
TEST(EffectorZoom, SyncRun_sinkbuf_dcsink_zoomin) {
    running_effector_window ("test zoom of sync with sinkbuf, use dc to sink, zoomin, can't you see it?", \
            0, MGEFF_EFFECTOR_ZOOM, 1, 1, MGEFF_PROPERTY_ZOOM, MGEFF_ZOOMIN);
}
TEST(EffectorZoom, SyncRun_sinkbuf_dcsink_zoomout) {
    running_effector_window ("test zoom of sync with sinkbuf, use dc to sink, zoomout, can't you see it?", \
            0, MGEFF_EFFECTOR_ZOOM, 1, 1, MGEFF_PROPERTY_ZOOM, MGEFF_ZOOMOUT);
}

/**/
TEST(EffectorPush, SyncRun) {
    running_effector_window ("test push of sync, can't you see it?", 0, MGEFF_EFFECTOR_PUSH, 0, 0);
}
TEST(EffectorPush, ASyncRun) {
    running_effector_window ("test apush of sync, can't you see it?", 1, MGEFF_EFFECTOR_PUSH, 0, 0);
}
TEST(EffectorPush, SyncRun_sinkbuf) {
    running_effector_window ("test push of sync with sinkbuf, can't you see it?", 0, MGEFF_EFFECTOR_PUSH, 1, 0);
}
TEST(EffectorPush, SyncRun_sinkbuf_dcsink) {
    running_effector_window ("test push of sync with sinkbuf, use dc to sink, can't you see it?", \
            0, MGEFF_EFFECTOR_PUSH, 1, 0);
}
TEST(EffectorPush, SyncRun_left2right) {
    running_effector_window ("test push of sync, left to right, can't you see it?", 
            0, MGEFF_EFFECTOR_PUSH, 0, 0, MGEFF_PROPERTY_DIRECTION, MGEFF_DIRECTION_LEFT2RIGHT);
}
TEST(EffectorPush, SyncRun_right2left) {
    running_effector_window ("test push of sync, right to left, can't you see it?", 
            0, MGEFF_EFFECTOR_PUSH, 0, 0, MGEFF_PROPERTY_DIRECTION, MGEFF_DIRECTION_RIGHT2LEFT);
}
TEST(EffectorPush, SyncRun_top2bottom) {
    running_effector_window ("test push of sync, top to bottom, can't you see it?", 
            0, MGEFF_EFFECTOR_PUSH, 0, 0, MGEFF_PROPERTY_DIRECTION, MGEFF_DIRECTION_TOP2BOTTOM);
}
TEST(EffectorPush, SyncRun_bottom2top) {
    running_effector_window ("test push of sync, bottom to top, can't you see it?", 
            0, MGEFF_EFFECTOR_PUSH, 0, 0, MGEFF_PROPERTY_DIRECTION, MGEFF_DIRECTION_BOTTOM2TOP);
}

/**/
TEST(EffectorClear, SyncRun) {
    running_effector_window ("test clear of sync, can't you see it?", 0, MGEFF_EFFECTOR_CLEAR, 0, 0);
}
TEST(EffectorClear, ASyncRun) {
    running_effector_window ("test clear of async, can't you see it?", 1, MGEFF_EFFECTOR_CLEAR, 0, 0);
}
TEST(EffectorClear, SyncRun_sinkbuf) {
    running_effector_window ("test clear of sync, can't you see it?", 0, MGEFF_EFFECTOR_CLEAR, 1, 0);
}
TEST(EffectorClear, SyncRun_sinkbuf_dcsink) {
    running_effector_window ("test clear of sync, use dc to sink, can't you see it?", \
            0, MGEFF_EFFECTOR_CLEAR, 1, 1);
}

/**/
TEST(EffectorCubicRotate, SyncRun) {
    running_effector_window ("test cubic rotate of sync, can't you see it?", 0, MGEFF_EFFECTOR_CUBIC_ROTATE, 0, 0);
}
TEST(EffectorCubicRotate, ASyncRun) {
    running_effector_window ("test cubic rotate of async, can't you see it?", 1, MGEFF_EFFECTOR_CUBIC_ROTATE, 0, 0);
}
TEST(EffectorCubicRotate, SyncRun_sinkbuf) {
    running_effector_window ("test cubic rotate of sync with sinkbuf, can't you see it?", 0, MGEFF_EFFECTOR_CUBIC_ROTATE, 1, 0);
}
TEST(EffectorCubicRotate, SyncRun_sinkbuf_dcsink) {
    running_effector_window ("test cubic rotate of sync with sinkbuf, use dc to sink, can't you see it?", \
            0, MGEFF_EFFECTOR_CUBIC_ROTATE, 1, 1);
}

/**/
TEST(EffectorCenterSplit, SyncRun) {
    running_effector_window ("test centersplit of sync, can't you see it?", 0, MGEFF_EFFECTOR_CENTERSPLIT, 0, 0);
}
TEST(EffectorCenterSplit, ASyncRun) {
    running_effector_window ("test centersplit of async, can't you see it?", 1, MGEFF_EFFECTOR_CENTERSPLIT, 0, 0);
}
TEST(EffectorCenterSplit, SyncRun_sinkbuf) {
    running_effector_window ("test centersplit of sync with, can't you see it?", 0, MGEFF_EFFECTOR_CENTERSPLIT, 1, 0);
}
TEST(EffectorCenterSplit, SyncRun_sinkbuf_dcsink) {
    running_effector_window ("test centersplit of sync with, use dc to sink, can't you see it?", \
            0, MGEFF_EFFECTOR_CENTERSPLIT, 1, 1);
}

/**/
TEST(EffectorRadarScan, SyncRun) {
    running_effector_window ("test radarscan of sync, can't you see it?", 0, MGEFF_EFFECTOR_RADARSCAN, 0, 0);
}
TEST(EffectorRadarScan, ASyncRun) {
    running_effector_window ("test radarscan of async, can't you see it?", 1, MGEFF_EFFECTOR_RADARSCAN, 0, 0);
}
TEST(EffectorRadarScan, SyncRun_sinkbuf) {
    running_effector_window ("test radarscan of sync with sinkbuf, can't you see it?", 0, MGEFF_EFFECTOR_RADARSCAN, 1, 0);
}
TEST(EffectorRadarScan, SyncRun_sinkbuf_dcsink) {
    running_effector_window ("test radarscan of sync with sinkbuf, use dc to sink, can't you see it?", 0, MGEFF_EFFECTOR_RADARSCAN, 1, 0);
}
TEST(EffectorRadarScan, SyncRun_clockwise) {
    running_effector_window ("test radarscan of sync, clock wise, can't you see it?", 0, MGEFF_EFFECTOR_RADARSCAN, 0, 0,
            MGEFF_PROPERTY_DIRECTION, MGEFF_CLOCKWISE);
}
TEST(EffectorRadarScan, SyncRun_clockinverse) {
    running_effector_window ("test radarscan of sync, clock inverse, can't you see it?", 0, MGEFF_EFFECTOR_RADARSCAN, 0, 0,
            MGEFF_PROPERTY_DIRECTION, MGEFF_CLOCKINVERSE);
}
TEST(EffectorRadarScan, SyncRun_10pices) {
    running_effector_window ("test radarscan of sync, 10 pieces, can't you see it?", 0, MGEFF_EFFECTOR_RADARSCAN, 0, 0,
            MGEFF_PROPERTY_PIECES, 10);
}
TEST(EffectorRadarScan, SyncRun_starttangle30) {
    running_effector_window ("test radarscan of sync, starttangel at 30 degree, can't you see it?", 0, 
            MGEFF_EFFECTOR_RADARSCAN, 0, 0, 
            MGEFF_PROPERTY_STARTANGLE, 333);
}

#if 0
/**/
TEST(EffectorRoller, SyncRun) {
    running_effector_window ("test roller of sync, can't you see it?", 0, MGEFF_EFFECTOR_ROLLER, 0, 0);
}
TEST(EffectorRoller, ASyncRun) {
    running_effector_window ("test roller of async, can't you see it?", 1, MGEFF_EFFECTOR_ROLLER, 0, 0);
}
TEST(EffectorRoller, SyncRun_sinkbuf) {
    running_effector_window ("test roller of sync with sinkbuf, can't you see it?", 0, MGEFF_EFFECTOR_ROLLER, 1, 0);
}
TEST(EffectorRoller, SyncRun_sinkbuf_dcsink) {
    running_effector_window ("test roller of sync with sinkbuf, use dc to sink, can't you see it?", \
            0, MGEFF_EFFECTOR_ROLLER, 1, 1);
}
#endif

#if 0
/**/
TEST(EffectorBlockFlip, SyncRun) {
    running_effector_window ("test block flip of sync, can't you see it?", 0, MGEFF_EFFECTOR_BLOCKFLIP, 0, 0);
}
TEST(EffectorBlockFlip, ASyncRun) {
    running_effector_window ("test block flip of async, can't you see it?", 1, MGEFF_EFFECTOR_BLOCKFLIP, 0, 0);
}
TEST(EffectorBlockFlip, SyncRun_sinkbuf) {
    running_effector_window ("test block flip of sync with sinkbuf, can't you see it?", 0, MGEFF_EFFECTOR_BLOCKFLIP, 1, 0);
}
TEST(EffectorBlockFlip, SyncRun_sinkbuf_dcsink) {
    running_effector_window ("test block flip of sync with sinkbuf, use dc to sink, can't you see it?", \
            0, MGEFF_EFFECTOR_BLOCKFLIP, 1, 1);
}
#endif

#if 0
/**/
TEST(EffectorMgplusFlip, SyncRun) {
    running_effector_window ("test flip effector of sync, can't you see it?", 0, MGEFF_EFFECTOR_MGPLUS_FLIP, 0, 0);
}
TEST(EffectorMgplusFlip, ASyncRun) {
    running_effector_window ("test flip effector of async, can't you see it?", 1, MGEFF_EFFECTOR_MGPLUS_FLIP, 0, 0);
}
TEST(EffectorMgplusFlip, SyncRun_sinkbuf) {
    running_effector_window ("test flip effector of sync with sinkbuf, can't you see it?", 0, MGEFF_EFFECTOR_MGPLUS_FLIP, 1, 0);
}
TEST(EffectorMgplusFlip, SyncRun_sinkbuf_dcsink) {
    running_effector_window ("test flip effector of sync with sinkbuf, use dc to sink, can't you see it?", \
            0, MGEFF_EFFECTOR_MGPLUS_FLIP, 1, 1);
}
#endif

#if 0
/**/
TEST(EffectorCubicWindow, SyncRun) {
    running_effector_window ("test cubic rotate of sync, can't you see it?", \
            0, MGEFF_EFFECTOR_MGPLUS_CUBIC_ROTATE, 0, 0);
}
TEST(EffectorCubicWindow, ASyncRun) {
    running_effector_window ("test cubic rotate of async, can't you see it?", \
            1, MGEFF_EFFECTOR_MGPLUS_CUBIC_ROTATE, 0, 0);
}
TEST(EffectorCubicWindow, SyncRun_sinkbuf) {
    running_effector_window ("test cubic rotate of sync with sinkbuf, can't you see it?", \
            0, MGEFF_EFFECTOR_MGPLUS_CUBIC_ROTATE, 1, 0);
}
TEST(EffectorCubicWindow, SyncRun_sinkbuf_dcsink) {
    running_effector_window ("test cubic rotate of sync with sinkbuf, use dc to sink, can't you see it?", \
            0, MGEFF_EFFECTOR_MGPLUS_CUBIC_ROTATE, 1, 1);
}
#endif

#if 0
/**/
TEST(EffectorMgplusRotate, SyncRun) {
    running_effector_window ("test mgplus rotate effector of sync, can't you see it?", 0, MGEFF_EFFECTOR_MGPLUS_ROTATE, 0, 0);
}
TEST(EffectorMgplusRotate, ASyncRun) {
    running_effector_window ("test mgplus rotate effector of async, can't you see it?", 1, MGEFF_EFFECTOR_MGPLUS_ROTATE, 0, 0);
}
TEST(EffectorMgplusRotate, SyncRun_sinkbuf) {
    running_effector_window ("test mgplus rotate effector of sync with sinkbuf, can't you see it?", \
            0, MGEFF_EFFECTOR_MGPLUS_ROTATE, 1, 0);
}
TEST(EffectorMgplusRotate, SyncRun_sinkbuf_dcsink) {
    running_effector_window ("test mgplus rotate effector of sync with sinkbuf, use dc to sink, can't you see it?", \
            0, MGEFF_EFFECTOR_MGPLUS_ROTATE, 1, 1);
}
#endif

/**/
TEST(EffectorPointStarEffector, SyncRun) {
    running_effector_window ("test five point star effector of sync, can't you see it?", \
            0, MGEFF_EFFECTOR_FIVEPOINTEDSTAR, 0, 0);
}
TEST(EffectorPointStarEffector, ASyncRun) {
    running_effector_window ("test five point star effector of async, can't you see it?", \
            1, MGEFF_EFFECTOR_FIVEPOINTEDSTAR, 0, 0);
}
TEST(EffectorPointStarEffector, SyncRun_sinkbuf) {
    running_effector_window ("test five point star effector of sync with sinkbuf, can't you see it?", \
            0, MGEFF_EFFECTOR_FIVEPOINTEDSTAR, 1, 0);
}
TEST(EffectorPointStarEffector, SyncRun_sinkbuf_dcsink) {
    running_effector_window ("test five point star effector of sync with sinkbuf, use dc to sink, can't you see it?", \
            0, MGEFF_EFFECTOR_FIVEPOINTEDSTAR, 1, 1);
}
TEST(EffectorPointStarEffector, SyncRun_zoomin) {
    running_effector_window ("test five point star effector of sync, zoom in, can't you see it?", \
            0, MGEFF_EFFECTOR_FIVEPOINTEDSTAR, 0, 0, MGEFF_PROPERTY_ZOOM, MGEFF_ZOOMIN);
}
TEST(EffectorPointStarEffector, SyncRun_zoomout) {
    running_effector_window ("test five point star effector of sync, zoom out, can't you see it?", \
            0, MGEFF_EFFECTOR_FIVEPOINTEDSTAR, 0, 0, MGEFF_PROPERTY_ZOOM, MGEFF_ZOOMOUT);
}
TEST(EffectorPointStarEffector, SyncRun_clockwise) {
    running_effector_window ("test five point star effector of sync, clock wise, zoom out, can't you see it?", \
            0, MGEFF_EFFECTOR_FIVEPOINTEDSTAR, 0, 0, MGEFF_PROPERTY_DIRECTION, MGEFF_CLOCKWISE);
}
TEST(EffectorPointStarEffector, SyncRun_clockinverse) {
    running_effector_window ("test five point star effector of sync, clock inverse, zoom out, can't you see it?", \
            0, MGEFF_EFFECTOR_FIVEPOINTEDSTAR, 0, 0, MGEFF_PROPERTY_DIRECTION, MGEFF_CLOCKINVERSE);
}

int main (int argc, char* argv[]) 
{
    int ret;
    static struct option long_options[] = {
        {"help", 0, 0, 'h'},
        {"display", 0, 0, 's'},
        {0, 0, 0, 0}
    };

    while (1) {
        int c;
        int option_index;

        c = getopt_long(argc, argv, "hs", long_options, &option_index);
        if (c == -1) {
            break;
        }
        switch (c) {
            case 'h':
                printf ("\nUsage: \n");
                printf ("    -h|--help  show this message!\n");
                printf ("    -s|--display  display then win, otherwise run auto!\n\n");
                exit (0);
                break;
            case 's':
                printf ("You can see the result at your screen.\n");
                g_display= true;
                break;
        }
    }

    InitGUI (0, NULL);
    init_effector_test ();

    if (g_display) {
        MessageBox (g_hwnd, "are you ready?", "Asking", 0);
    }

    ::testing::InitGoogleTest(&argc, argv);
    ret = RUN_ALL_TESTS();
    deinit_effector_test ();

    return ret;
}
