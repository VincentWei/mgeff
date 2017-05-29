#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/time.h>
#include <time.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

#include <mgeff/mgeff-control.h>

#define M 1
#define F 0.8
#define K 0.005
#define TDSCALE 10
#define FillMyBox(hdc, x) FillBox(hdc, x, 100, 20, 20)

static speedmeter_handle sm_handle;
static int pos;

void drawicon(HWND hwnd, int x)
{
    int old_x;
    HDC hdc = GetClientDC(hwnd);

    old_x = pos;
    SetBrushColor(hdc, RGBA2Pixel(hdc, 0xff, 0xff, 0xff, 0xff));
    FillMyBox(hdc, old_x);
    SetBrushColor(hdc, RGBA2Pixel(hdc, 0xff, 0x00, 0x00, 0xff));
    FillMyBox(hdc, x);
    pos = x;
    ReleaseDC(hdc);
}
void cb(MGEFF_ANIMATION handle, HWND hwnd, int id, float *value)
{
    int old_x;
    int new_x = (int)(*value);
    HDC hdc = GetClientDC(hwnd);

    if (new_x < 0) {
        new_x = -new_x % g_rcScr.right;
    }
    if (new_x > g_rcScr.right) {
        new_x = g_rcScr.right - (new_x % g_rcScr.right);
    }

    old_x = pos;
    SetBrushColor(hdc, RGBA2Pixel(hdc, 0xff, 0xff, 0xff, 0xff));
    FillMyBox(hdc, old_x);
    SetBrushColor(hdc, RGBA2Pixel(hdc, 0xff, 0x00, 0x00, 0xff));
    FillMyBox(hdc, new_x);
    pos = new_x;
    ReleaseDC(hdc);
}

static MGEFF_ANIMATION create_frct_animation(HWND hwnd, float startval, float endval, int duration)
{
    MGEFF_ANIMATION anim = mGEffAnimationCreate((void*)hwnd,
            (void*)cb, 0, MGEFF_FLOAT);
    mGEffAnimationSetStartValue(anim, &startval);
    mGEffAnimationSetEndValue(anim, &endval);
    mGEffAnimationSetDuration(anim, duration);
    mGEffAnimationSetCurve(anim, OutCirc);

    return anim;
}

static MGEFF_ANIMATION create_my_animation(HWND hwnd)
{
    MGEFF_ANIMATION anim;
    float startval = 0, endval = 0, t = 1;
    float v0, s;
    
    v0 = speedmeter_calc(sm_handle);
    motion_frictions(abs(v0), M, F, &s, &t);
    printf("testcase.c: %s, v0 = %f, s = %f, t = %f.\n", __FUNCTION__, v0, s, t);
    startval = pos;
    if (v0 < 0) {
        endval = startval - s;
    }
    else {
        endval = startval + s;
    }
    printf("create fanimation, s = %f, e = %f, t = %f.\n", startval, endval, t); 
    anim = create_frct_animation(hwnd, startval, endval, (int)t * TDSCALE);
    
    return anim;
}

static int TestWinProc(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
    static MGEFF_ANIMATION animation;
    static int lbflag = 0;
    HDC hdc;
    switch(message)
    {
        case MSG_CREATE:
            {
                sm_handle = speedmeter_create(16);
                pos = g_rcScr.right / 2;
                mGEffInit();
            }
            return 0;
        case MSG_PAINT:
            {
                hdc = BeginPaint(hWnd);
                SetBrushColor(hdc, RGBA2Pixel(hdc, 0xff, 0x00, 0x00, 0xff));
                FillMyBox(hdc, pos);
                EndPaint(hWnd, hdc);
            }
            return 0;
        case MSG_LBUTTONDOWN:
            {
                mGEffAnimationPause(animation);
                speedmeter_reset(sm_handle);
                lbflag = 1;
                SetTimer(hWnd, 100, 1);
            }
            return 0;
        case MSG_MOUSEMOVE:
            {
                int x;
                x = LOSWORD(lParam);
                if (GetShiftKeyStatus() & KS_LEFTBUTTON && lbflag) {
                    drawicon(hWnd, x);
                    InvalidateRect(hWnd, NULL, TRUE);
                }
            }

            return 0;
        case MSG_MOUSEMOVEIN:
            {
                BOOL in_out = (BOOL)wParam;
                if (in_out || (!lbflag)) {
                    return 0;
                }
            }
        case MSG_LBUTTONUP:
            {
                KillTimer(hWnd, 100);
                
                animation = create_my_animation(hWnd);
                mGEffAnimationAsyncRun(animation);

                lbflag = 0;
            }
            return 0;
        case MSG_TIMER:
            {
                unsigned int t;
                POINT p;
                t = (unsigned int)lParam;
                GetCursorPos(&p);
                speedmeter_add(sm_handle, p.x, t); 
            }
            return 0;
        case MSG_CLOSE:
            mGEffDeinit();
            DestroyMainWindow(hWnd);
            PostQuitMessage(hWnd);
            MainWindowThreadCleanup(hWnd);
            return 0;
    }

    return DefaultMainWinProc(hWnd, message, wParam, lParam);
}

int MiniGUIMain(int argc, const char* argv[])
{
    MSG Msg;
    HWND hMainWnd;
    MAINWINCREATE CreateInfo;

#ifdef _MGRM_PROCESSES
    JoinLayer(NAME_DEF_LAYER, "hello world", 0, 0);
#endif

    CreateInfo.dwStyle = WS_VISIBLE | WS_BORDER | WS_CAPTION;
    CreateInfo.dwExStyle = WS_EX_NONE;
    CreateInfo.spCaption = "test-ctrlmotion";
    CreateInfo.hMenu = 0;
    CreateInfo.hCursor = GetSystemCursor(0);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = TestWinProc;
    CreateInfo.lx = 0;
    CreateInfo.ty = 0;
    CreateInfo.rx = 800;
    CreateInfo.by = 500;
    CreateInfo.iBkColor = COLOR_lightwhite;
    CreateInfo.dwAddData = 0;
    CreateInfo.hHosting = HWND_DESKTOP;

    hMainWnd = CreateMainWindow(&CreateInfo);
    
    if(hMainWnd == HWND_INVALID)
        return -1;

    ShowWindow(hMainWnd, SW_SHOWNORMAL);

    while(GetMessage(&Msg, hMainWnd))
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    return 0;
}

#ifndef _MGRM_PROCESSES
#include <minigui/dti.c>
#endif

