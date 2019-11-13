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

#include "animation_base.h"
#include "animation_group_ex.h"

HWND g_hwnd = NULL;
pthread_t thread_id;
int g_quit = 1;
void *power_pthread (void *arg)
{
    while (g_quit)
    {
        assert(0);
        // EffSchedularOnTimer();
        usleep (20000);
    }
    return NULL;
}

void create_schedular_pthread (void)
{
    pthread_create(&thread_id, NULL, power_pthread, NULL);
}

#if 0
//test the group
TEST(AnimationGroup, ParallelCreate)
{
    test_animation_group_create(MGEFF_PARALLEL);
}

TEST(AnimationGroup, ParallelDelete)
{
    test_animation_group_delete(MGEFF_PARALLEL);
}

TEST(AnimationGroup, ParallelStop)
{
    test_animation_group_stop(MGEFF_PARALLEL);
}
TEST(AnimationGroup, ParallelPauseResume)
{
    test_animation_group_pause_resume(MGEFF_PARALLEL);
}

TEST(AnimationGroup, ParallelWait)
{
    test_animation_group_wait(MGEFF_PARALLEL);
}
#endif

TEST(AnimationGroup, SequentialCreate)
{
    timeval stime, etime;

    gettimeofday(&stime, NULL);
    test_animation_group_create(MGEFF_SEQUENTIAL);
    gettimeofday(&etime, NULL);
}

#if 1
TEST(AnimationGroup, SequentialDelete)
{
    test_animation_group_delete(MGEFF_SEQUENTIAL);
}
TEST(AnimationGroup, SequentialStop)
{
    test_animation_group_stop(MGEFF_SEQUENTIAL);
}
TEST(AnimationGroup, SequentialPauseResume)
{
    test_animation_group_pause_resume(MGEFF_SEQUENTIAL);
}

TEST(AnimationGroup, SequentialWait)
{
    test_animation_group_wait(MGEFF_SEQUENTIAL);
}
#endif


static int mainWindowProc(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
    int ret;
    switch (message)
    {
        case MSG_CREATE:
            SetTimer(hWnd, TIMER_ID, 5);
            mGEffInit();
            break;

        case MSG_TIMER:
            if (wParam == TIMER_ID){
                KillTimer(hWnd, TIMER_ID);
                ret = RUN_ALL_TESTS();
            }
            break;

        case MSG_KEYDOWN:
            if(wParam == SCANCODE_ESCAPE)
                PostMessage(hWnd, MSG_CLOSE, 0, 0);
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
