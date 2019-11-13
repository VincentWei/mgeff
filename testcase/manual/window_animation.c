///////////////////////////////////////////////////////////////////////////////
//
//                          IMPORTANT NOTICE
//
// The following open source license statement does not apply to any
// entity in the Exception List published by FMSoft.
//
// For more information, please visit:
//
// https://www.fmsoft.cn/exception-list
//
//////////////////////////////////////////////////////////////////////////////
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
#include <mgeff/mgeff.h>
#include <minigui/control.h>
#include <math.h>
#include <string.h>

#define MAX_CUT_LINE 10
#define PARTICLE_MAX (MAX_CUT_LINE * MAX_CUT_LINE)

struct _smash_t {
    POINT pos;
    SIZE size;
    HDC skin;
    RECT rc;
} g_smash[PARTICLE_MAX];

HDC g_screen_store = HDC_INVALID;

void background_generator(HDC hdc, RECT *rc)
{
    if (HDC_INVALID != g_screen_store) {
        BitBlt(g_screen_store, rc->left, rc->top, RECTWP(rc), RECTHP(rc),
                hdc, rc->left, rc->top, -1);
//        FillBox(hdc, rc->left, rc->top, RECTWP(rc), RECTHP(rc));
    }
}
void animation_generator(MGEFF_ANIMATION handle, HWND hwnd, int id, RECT *rc)
{

}
    
MGEFF_ANIMATION smash_animation_generator(HWND sink_hwnd, RECT* start, RECT* end, HDC skin, int index)
{
    MGEFF_ANIMATION hanim = 
        mGEffAnimationCreate((void *)sink_hwnd, (void *)animation_generator, index, MGEFF_RECT);
    return hanim;
}

void smash_animation_gen(MGEFF_ANIMATION handle, HWND hwnd, int id, POINT *pos) {
    int i;
    BitBlt(g_screen_store, g_smash[id].pos.x, g_smash[id].pos.y, g_smash[id].size.cx, g_smash[id].size.cy,
            HDC_SCREEN, g_smash[id].pos.x, g_smash[id].pos.y, -1);
    memcpy(&(g_smash[id].pos), pos, sizeof(POINT));
    if (id == PARTICLE_MAX - 1)
    {
        for (i = 0; i < PARTICLE_MAX; ++i)
        {
            BitBlt(g_smash[i].skin, g_smash[i].rc.left, g_smash[i].rc.top, 
                    RECTW(g_smash[i].rc), RECTH(g_smash[i].rc),
                    HDC_SCREEN, g_smash[i].pos.x, g_smash[i].pos.y, 0);
            fprintf(stderr, "i=%d,%d,%d\n", i, g_smash[i].pos.x, g_smash[i].pos.y);
        }
    }
}

int WindowSmashOut(HWND hwnd)
{
    int i = 0, j = 0;
    RECT wnd_rc;
    MGEFF_ANIMATION hanim;
    MGEFF_ANIMATION hgroup = mGEffAnimationCreateGroup(MGEFF_PARALLEL);
    if (HWND_INVALID != hwnd && IsWindowVisible(hwnd) && GetWindowRect(hwnd, &wnd_rc)) {
        int w_wnd = RECTW(wnd_rc);
        int h_wnd = RECTH(wnd_rc);
        MAINWINCREATE CreateInfo;
        CreateInfo.dwStyle = WS_VISIBLE;
        CreateInfo.dwExStyle = WS_EX_NONE;
        CreateInfo.spCaption = "";
        CreateInfo.hMenu = 0;
        CreateInfo.hCursor = GetSystemCursor(0);
        CreateInfo.hIcon = 0;
        CreateInfo.iBkColor = PIXEL_lightwhite;
        CreateInfo.dwAddData = 0;
        CreateInfo.hHosting = HWND_DESKTOP;

        for (i = 0; i < MAX_CUT_LINE; ++i) {
            for (j = 0; j < MAX_CUT_LINE; ++j) {
                int x = j * w_wnd / MAX_CUT_LINE;
                int y = i * h_wnd / MAX_CUT_LINE;
                RECT rc = {x, y, 
                    x + w_wnd / MAX_CUT_LINE, y + h_wnd / MAX_CUT_LINE};
//                printf("%d, %d, %d, %d\n", rc.left, rc.top, rc.right, rc.bottom);
                if (WindowExtract(hwnd, &rc, &CreateInfo)) {
                    HWND hwnd_new = CreateMainWindow(&CreateInfo);
                    MGEFF_WINDOW_ANIMATION_CONTEXT hctxt =
                        mGEffCreateWindowAnimationContext(1000, MGEFF_EFFECTOR_ALPHA, InQuart, background_generator, NULL);
                    hanim = mGEffMoveWindowPreparation(hwnd_new,
                            i * 50, j * 50, 0, 0, TRUE, hctxt);
                    mGEffAnimationAddToGroup(hgroup, hanim);
                }
            }
        }
        if (HDC_INVALID == g_screen_store) {
            g_screen_store = CreateCompatibleDC(HDC_SCREEN);
        }
        BitBlt(HDC_SCREEN, 0, 0, 0, 0, g_screen_store, 0, 0, -1);
        mGEffAnimationSyncRun(hgroup);
        return 0;
    }
    return -1;
}

#if 0
int WindowSmashIn(HWND hwnd, int time)
{
    int i = 0, j = 0;
    RECT wnd_rc;
    MGEFF_ANIMATION hgroup = mGEffAnimationCreateGroup(MGEFF_PARALLEL);
    if (HWND_INVALID != hwnd && !IsWindowVisible(hwnd) && GetWindowRect(hwnd, &wnd_rc)) {
        int w_wnd = RECTW(wnd_rc);
        int h_wnd = RECTH(wnd_rc);
        MAINWINCREATE CreateInfo;

        IncludeWindowStyle(hwnd, WS_VISIBLE);
        for (i = 0; i < MAX_CUT_LINE; ++i) {
            for (j = 0; j < MAX_CUT_LINE; ++j) {
                int x = j * w_wnd / MAX_CUT_LINE;
                int y = i * h_wnd / MAX_CUT_LINE;
                RECT rc = {x, y, 
                    x + w_wnd / MAX_CUT_LINE, y + h_wnd / MAX_CUT_LINE};
                if (WindowExtract(hwnd, &rc, &CreateInfo)) {
                    int index = i * MAX_CUT_LINE + j;
                    POINT target = {CreateInfo.lx, CreateInfo.ty};
                    MGEFF_ANIMATION animation = mGEffAnimationCreate(hwnd, (void *)smash_animation_gen, index, MGEFF_POINT);
                    g_smash[index].pos.x = rand() % g_rcScr.right;
                    g_smash[index].pos.y = rand() % g_rcScr.bottom;
                    g_smash[index].size.cx = w_wnd / MAX_CUT_LINE;
                    g_smash[index].size.cy = h_wnd / MAX_CUT_LINE;
                    g_smash[index].skin = CreateInfo.dwAddData;
                    mGEffAnimationSetStartValue(animation, &g_smash[index].pos);
                    mGEffAnimationSetEndValue(animation, &target);
                    mGEffAnimationSetDuration(animation, time + i * 25);
                    mGEffAnimationSetCurve(animation, InOutBack);
                    mGEffAnimationAddToGroup(hgroup, animation);
                }
            }
        }
        if (HDC_INVALID == g_screen_store) {
            g_screen_store = CreateCompatibleDC(HDC_SCREEN);
        }
        BitBlt(HDC_SCREEN, 0, 0, 0, 0, g_screen_store, 0, 0, -1);
        mGEffAnimationStart(hgroup, 0);
        mGEffSchedularRun(MGEFF_MODE_BLOCK);

        ExcludeWindowStyle(hwnd, WS_VISIBLE);
        ShowWindow(hwnd, SW_SHOWNORMAL);

        for (i = 0; i < PARTICLE_MAX; ++i) {
            DeleteMemDC(g_smash[i].skin);
        }
    }
    return -1;

}
#else

void WindowSmashIn(MGEFF_WindowAnimCtxt* ctxt)
{
    HWND hwnd;
    int time;
    int i = 0, j = 0;
    RECT wnd_rc;

    MGEFF_ANIMATION hgroup = mGEffAnimationCreateGroup(MGEFF_PARALLEL);

    hwnd = ctxt->hwnd;
    time = 1000;
    if (HWND_INVALID != hwnd && GetWindowRect(hwnd, &wnd_rc)) {
        int w_wnd = RECTW(wnd_rc);
        int h_wnd = RECTH(wnd_rc);
        //MAINWINCREATE CreateInfo;

        for (i = 0; i < MAX_CUT_LINE; ++i) {
            for (j = 0; j < MAX_CUT_LINE; ++j) {
                int x = j * w_wnd / MAX_CUT_LINE;
                int y = i * h_wnd / MAX_CUT_LINE;
                RECT rc = {x, y, 
                    x + w_wnd / MAX_CUT_LINE, y + h_wnd / MAX_CUT_LINE};

                int index = i * MAX_CUT_LINE + j;
                POINT target = {x, y};
                MGEFF_ANIMATION animation = mGEffAnimationCreate(hwnd, (void *)smash_animation_gen, index, MGEFF_POINT);

                WindowToScreen(hwnd, &target.x, &target.y);
                g_smash[index].rc = rc;
                g_smash[index].pos.x = rand() % g_rcScr.right;
                g_smash[index].pos.y = rand() % g_rcScr.bottom;
                g_smash[index].size.cx = w_wnd / MAX_CUT_LINE;
                g_smash[index].size.cy = h_wnd / MAX_CUT_LINE;
                g_smash[index].skin = ctxt->dst_dc;
                mGEffAnimationSetStartValue(animation, &g_smash[index].pos);
                mGEffAnimationSetEndValue(animation, &target);
                mGEffAnimationSetDuration(animation, time + i * 25);
                mGEffAnimationSetCurve(animation, InOutBack);
                mGEffAnimationAddToGroup(hgroup, animation);

            }
        }
        mGEffAnimationSyncRun(hgroup);
    }
}
#endif

void ControlPopup(HWND hWnd, int ctrl_id, MGEFF_WINDOW_ANIMATION_CONTEXT hctxt)
{
    MAINWINCREATE CreateInfo;

    CreateInfo.dwStyle = WS_VISIBLE;
    CreateInfo.dwExStyle = WS_EX_TOPMOST;
    CreateInfo.spCaption = "";
    CreateInfo.hMenu = 0;
    CreateInfo.hCursor = GetSystemCursor(0);
    CreateInfo.hIcon = 0;
    CreateInfo.iBkColor = PIXEL_lightwhite;
    CreateInfo.dwAddData = 0;
    CreateInfo.hHosting = HWND_DESKTOP;
    if (ControlExtract(hWnd, ctrl_id, &CreateInfo)) {
        HWND hwnd_new = CreateMainWindow(&CreateInfo);
        if (HWND_INVALID != hwnd_new) {
            if (HDC_INVALID == g_screen_store) {
                g_screen_store = CreateCompatibleDC(HDC_SCREEN);
            }
            BitBlt(HDC_SCREEN, 0, 0, 0, 0, g_screen_store, 0, 0, -1);
            mGEffMoveWindow(hwnd_new, 200, 150, 300, 200, TRUE, hctxt);
        }
    }
}

static int TestWinProc(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;

    MGEFF_WINDOW_ANIMATION_CONTEXT hctxt = 
        mGEffCreateWindowAnimationContext(1500, MGEFF_EFFECTOR_ALPHA, InQuart, background_generator, NULL);

    switch (message) {
        case MSG_CREATE:
            CreateWindow (CTRL_BUTTON, "ok", 
                          WS_VISIBLE, 
			  50, 55, 95, 45, 45, hWnd, 0);
            break;

//        case MSG_TIMER:
//            sprintf (msg_text, HL_ST_TIMER, 
//                            GetTickCount ());
//            InvalidateRect (hWnd, &msg_rc, TRUE);
//            break;
//            
//        case MSG_LBUTTONDOWN:
//            strcpy (msg_text, HL_ST_LBD);
//            InvalidateRect (hWnd, &msg_rc, TRUE);
//            break;
//
        case MSG_LBUTTONDOWN:
            printf("MSG_LBUTTONDOWN.\n");
            break;
        case MSG_LBUTTONUP:
            mGEffMoveWindow (hWnd, 50, 50, 300, 200, TRUE, hctxt);
            //WindowSmashIn(hWnd, 1000);
            break;

#if 1
        case MSG_COMMAND:
            {
                if ((DWORD)wParam == 50) {
                    ControlPopup(hWnd, wParam, hctxt);
                }
            }
            break;

        case MSG_RBUTTONUP:
            break;

        case MSG_KEYUP:
            {
                MGEFF_ANIMATION animation1 = mGEffMoveWindowPreparation(hWnd,
                        500, 200, 200, 300, TRUE, hctxt);
                mGEffAnimationSyncRun(animation1);
            }
            break;
#endif
        case MSG_PAINT:
            hdc = BeginPaint (hWnd);
//            DrawText (hdc, welcome_text, -1, &welcome_rc, DT_LEFT | DT_WORDBREAK);
//            DrawText (hdc, msg_text, -1, &msg_rc, DT_LEFT | DT_WORDBREAK);
            EndPaint (hWnd, hdc);
            return 0;

        case MSG_CLOSE:
//            KillTimer (hWnd, 100);
            DestroyMainWindow (hWnd);
            PostQuitMessage (hWnd);
            return 0;
    }

    return DefaultMainWinProc(hWnd, message, wParam, lParam);
}

int MiniGUIMain (int argc, const char* argv[])
{
    MSG Msg;
    HWND hMainWnd;
    MAINWINCREATE CreateInfo;
    //MGEFF_WINDOW_ANIMATION_CONTEXT hctxt = mGEffCreateWindowAnimationContext(1200, MGEFF_EFFECTOR_SCROLL, Linear, NULL);
    MGEFF_WINDOW_ANIMATION_CONTEXT hctxt = mGEffCreateWindowAnimationContext(1200, MGEFF_EFFECTOR_PUSH, Linear, NULL, NULL);

    mGEffInit();
#ifdef _MGRM_PROCESSES
    JoinLayer(NAME_DEF_LAYER , "window_animation" , 0 , 0);
#endif
    CreateInfo.dwStyle = 
        WS_VISIBLE | WS_BORDER | WS_CAPTION;
    CreateInfo.dwExStyle = WS_EX_TOPMOST;
    CreateInfo.spCaption = "animation";
    CreateInfo.hMenu = 0;
    CreateInfo.hCursor = GetSystemCursor(0);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = TestWinProc;
    CreateInfo.lx = 50;
    CreateInfo.ty = 50;
    CreateInfo.rx = CreateInfo.lx + 301;
    CreateInfo.by = CreateInfo.ty + 201;
    CreateInfo.iBkColor = COLOR_lightwhite;
    CreateInfo.dwAddData = 0;
    CreateInfo.hHosting = HWND_DESKTOP;
    g_screen_store = CreateCompatibleDC(HDC_SCREEN);
    if (HDC_INVALID != g_screen_store) {
        BitBlt(HDC_SCREEN, 0, 0, 0, 0, g_screen_store, 0, 0, -1);
    }

    //hMainWnd = CreateMainWindow(&CreateInfo);
    hMainWnd = mGEffCreateMainWindowEx(&CreateInfo, hctxt, WindowSmashIn);
    mGEffDestroyWindowAnimationContext(hctxt);
    
    if (hMainWnd == HWND_INVALID)
        return -1;

    while (GetMessage(&Msg, hMainWnd)) {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    MainWindowThreadCleanup (hMainWnd);
    return 0;
}

#ifdef _MGRM_THREADS
#include <minigui/dti.c>
#endif

