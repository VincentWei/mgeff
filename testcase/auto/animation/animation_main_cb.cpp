
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
#include "animation_cb.h"

HWND g_hwnd = NULL;

#if 1
//test the callback
TEST(AnimationCallback, finish_callback)
{
    test_animation_finish_callback();
}

TEST(AnimationCallback, statechanged_callback)
{
    test_animation_statechanged_callback();
}

TEST(AnimationCallback, dirchanged_callback)
{
    test_animation_dirchanged_callback();
}

TEST(AnimationCallback, varmalloc_calcvalue_callback)
{
    test_animation_varmalloc_calcvalue_callback();
}

TEST(AnimationCallback, curloopchanged_callback)
{
    test_animation_curloopchanged_callback();
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
