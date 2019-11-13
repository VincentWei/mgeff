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

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <sys/time.h>
#include <time.h> 

#include <minigui/common.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/minigui.h>

#include <mgeff/mgeff.h>

#include <gtest/gtest.h>

#include "animation_misc.h"
#include "animation_base.h"
#include "animation_group.h"
#include "animation_int_ex.h"

HWND g_hwnd = NULL;
#if 0
TEST(Animation, SetDirectionKeepAlive)
{
    test_animation_setdirection(1);
}

TEST(Animation, SetDirectionNotKeepAlive)
{
    test_animation_setdirection(0);
}

TEST(Animation, CreateEx)
{
    test_animation_createex();
}

TEST(Animation, Delete)
{
    test_animation_delete();
}

TEST(Animation, StopKeepAlive)
{
    test_animation_stop(1);
}

TEST(Animation, StopNotKeepAlive)
{
    test_animation_stop(0);
}
#endif
TEST(Animation, PauseResume)
{
    test_animation_pauseresume();
}

#if 0
#if 0
TEST(Animation, ReStart)
{
    test_animation_restart();
}
#endif
TEST(Animation, SetGetProperty)
{
    test_animation_setgetproperty();
}

TEST(Animation, SetDuration)
{
    test_animation_setduration();
}

TEST(Animation, SetGetValue)
{
    test_animation_setgetvalue();
}

TEST(Animation, SetCurveEx)
{
    test_animation_setcurveex();
}

TEST(Animation, SyncRunKeepAlive)
{
    test_animation_syncrun(1);
}

TEST(Animation, SyncRunNotKeepAlive)
{
    test_animation_syncrun(0);
}

TEST(Animation, AsyncRunKeepAlive)
{
    test_animation_asyncrun(1);
}

TEST(Animation, AsyncRunNotKeepAlive)
{
    test_animation_asyncrun(0);
}

TEST(Animation, RunTime)
{
    test_animation_runtime();
}

TEST(Animation, MultiAsyncRun)
{
    test_animation_multiasyncrun();
}
#endif

static int mainWindowProc(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
    int ret;
    switch (message)
    {
        case MSG_CREATE:
            SetTimer(hWnd, TIMER_ID, 1);
            mGEffInit();
            break;
        case MSG_TIMER:
            if (wParam == TIMER_ID){
                KillTimer(hWnd, TIMER_ID);

                ret = RUN_ALL_TESTS();

                //PostMessage(hWnd, MSG_CLOSE, 0, 0);
            }
            break;
        case MSG_CLOSE:
            mGEffDeinit();
            DestroyMainWindow (hWnd);
            PostQuitMessage (hWnd);
            break;
        default:
            break;
    }
    return DefaultMainWinProc(hWnd, message, wParam, lParam);
}

int MiniGUIMain(int argc, const char *argv[])
{
    MSG msg;
    HWND hMainHwnd;
    MAINWINCREATE createInfo;

    ::testing::InitGoogleTest(&argc,(char **)argv);
#ifdef _MGRM_PROCESSES   
    JoinLayer(NAME_DEF_LAYER, "animation", 0, 0);
#endif

    createInfo.dwStyle = WS_VISIBLE | WS_BORDER | WS_CAPTION;
    createInfo.dwExStyle = WS_EX_NONE;
    createInfo.spCaption = "animation";
    createInfo.hMenu = 0;
    createInfo.hCursor = GetSystemCursor(0);
    createInfo.hIcon = 0;
    createInfo.MainWindowProc = mainWindowProc;
    createInfo.lx = 0;  
    createInfo.ty = 0;  
    createInfo.rx = g_rcScr.right; 
    createInfo.by = g_rcScr.bottom; 
    createInfo.iBkColor = COLOR_black;
    createInfo.dwAddData = 0;
    createInfo.hHosting = HWND_DESKTOP;
    hMainHwnd = CreateMainWindow(&createInfo);
    if (hMainHwnd == HWND_INVALID)
    {
        return -1;
    }

    g_hwnd = hMainHwnd;
    ShowWindow(hMainHwnd, SW_SHOWNORMAL);

    while (GetMessage(&msg, hMainHwnd))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    MainWindowThreadCleanup(hMainHwnd);

    return 0;
}
