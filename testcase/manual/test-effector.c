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
#include    <minigui/common.h>
#include    <minigui/minigui.h>
#include    <minigui/gdi.h>
#include    <minigui/window.h>
#include    <minigui/control.h>

#include    <mgeff/mgeff.h>

#define dbg() printf("%s %d\n", __FUNCTION__, __LINE__)

#define MATRIX_W    800
#define MATRIX_H    600

static HDC g_hdc_pic[2];

static HDC createDCByPicture(HDC hdc, const char* path, int w, int h)
{
    BITMAP bmp;
    HDC dc;

    if (LoadBitmap(hdc, &bmp, path)) {
        return HDC_INVALID;
    }
    
    if (w == 0)
        w = bmp.bmWidth;

    if (h == 0)
        h = bmp.bmHeight;

    dc = CreateCompatibleDCEx(hdc, w, h);
    FillBoxWithBitmap(dc, 0, 0, w, h, &bmp);
    UnloadBitmap(&bmp);
    
    return dc;
}

#ifdef _MGEFF_EFFECTOR
//static int index = 0;

char* effector_index[] = {
    MGEFF_MINOR_leafwindow,
    MGEFF_MINOR_alpha,
    MGEFF_MINOR_push,
    MGEFF_MINOR_zip, 
    MGEFF_MINOR_scroll,
    MGEFF_MINOR_centersplit,
    MGEFF_MINOR_fivepointedstar,
    MGEFF_MINOR_radarscan,
    MGEFF_MINOR_zoom,
    MGEFF_MINOR_flip,
    MGEFF_MINOR_clear,
    MGEFF_MINOR_cubicrotate
};

int mGEffFillAnimation(HWND hWnd, HDC src1_dc, HDC src2_dc, HDC dst_dc, int _index, int duration)
{
    unsigned long key = mGEffStr2Key(effector_index[_index]);
    MGEFF_EFFECTOR effector = mGEffEffectorCreate (key);
    MGEFF_SOURCE source1 = mGEffCreateSource (src1_dc);
    MGEFF_SOURCE source2 = mGEffCreateSource (src2_dc);
    MGEFF_SINK sink = mGEffCreateHDCSink (dst_dc);
    MGEFF_ANIMATION handle;

    mGEffEffectorAppendSource (effector, source1);
    mGEffEffectorAppendSource (effector, source2);
    mGEffSetBufferSink(sink, src1_dc);
    mGEffEffectorSetSink (effector, sink);
#if 0
    switch (type) {
        case MGEFF_EFFECTOR_PUSH:
            mGEffEffectorSetProperty (effector, MGEFF_PROPERTY_DIRECTION, MGEFF_DIRECTION_BOTTOM2TOP);
            break;
        case MGEFF_EFFECTOR_LEAFWINDOW:
            break;
        case MGEFF_EFFECTOR_ZOOM:
            mGEffEffectorSetProperty (effector, MGEFF_PROPERTY_DIRECTION, MGEFF_DIRECTION_LDOWN2RUP);
            mGEffEffectorSetProperty (effector, MGEFF_PROPERTY_ZOOM, MGEFF_ZOOMOUT);
            break;
        case MGEFF_EFFECTOR_ZIP:
            mGEffEffectorSetProperty (effector, MGEFF_PROPERTY_DIRECTION, MGEFF_DIRECTION_LEFT2RIGHT);
            break;
        case MGEFF_EFFECTOR_CENTERSPLIT:
            mGEffEffectorSetProperty (effector, MGEFF_PROPERTY_DIRECTION, MGEFF_DIRECTION_CENTER2OUTSIDE_4);
            break;
        case MGEFF_EFFECTOR_FLIP:
            break;
        case MGEFF_EFFECTOR_SCROLL:
            break; 
        case MGEFF_EFFECTOR_RECT_ROTATE:
            break;
        case MGEFF_EFFECTOR_STAR:
            mGEffEffectorSetProperty (effector, MGEFF_PROPERTY_ZOOM, MGEFF_ZOOMOUT);
            mGEffEffectorSetProperty (effector, MGEFF_PROPERTY_DIRECTION, MGEFF_CLOCKWISE);
        case MGEFF_EFFECTOR_RADARSCAN:
            mGEffEffectorSetProperty (effector, MGEFF_PROPERTY_PIECES, 6);
            mGEffEffectorSetProperty (effector, MGEFF_PROPERTY_STARTANGLE, 30);
            break;
        default:
            if (type < MGEFF_EFFECTOR_LEAFWINDOW ||
                    type > MGEFF_EFFECTOR_MAX) {
                return -1;
            }
    }
#else
    if (key == MGEFF_EFFECTOR_PUSH) {
        mGEffEffectorSetProperty (effector, MGEFF_PROPERTY_DIRECTION, MGEFF_DIRECTION_BOTTOM2TOP);
    }
    else if (key == MGEFF_EFFECTOR_ZOOM) {
        mGEffEffectorSetProperty (effector, MGEFF_PROPERTY_DIRECTION, MGEFF_DIRECTION_LDOWN2RUP);
        mGEffEffectorSetProperty (effector, MGEFF_PROPERTY_ZOOM, MGEFF_ZOOMOUT);
    }
    else if (key == MGEFF_EFFECTOR_ZIP) {
        mGEffEffectorSetProperty (effector, MGEFF_PROPERTY_DIRECTION, MGEFF_DIRECTION_LEFT2RIGHT);
    }
    else if(key == MGEFF_EFFECTOR_CENTERSPLIT) {
        mGEffEffectorSetProperty (effector, MGEFF_PROPERTY_DIRECTION, MGEFF_DIRECTION_CENTER2OUTSIDE_4);
    }
    else if (key == MGEFF_EFFECTOR_FIVEPOINTEDSTAR) {
        mGEffEffectorSetProperty (effector, MGEFF_PROPERTY_ZOOM, MGEFF_ZOOMOUT);
        mGEffEffectorSetProperty (effector, MGEFF_PROPERTY_DIRECTION, MGEFF_CLOCKWISE);
    }
    else if (key == MGEFF_EFFECTOR_RADARSCAN) {
        mGEffEffectorSetProperty (effector, MGEFF_PROPERTY_PIECES, 6);
        mGEffEffectorSetProperty (effector, MGEFF_PROPERTY_STARTANGLE, 30);
    }
#endif

    handle = mGEffAnimationCreateWithEffector (effector);

    mGEffAnimationSetDuration (handle, duration);
    if( _index % 2 )
    {
        mGEffAnimationAsyncRun (handle);
        mGEffAnimationWait(&hWnd, handle);
    }
    else
    {
        mGEffAnimationSyncRun (handle);
    }

    mGEffEffectorDelete(effector);
    return 0;
}
#endif

static void TestEffector( HWND hwnd )
{
    HDC hdc;
    int i = 0 ;
    hdc = GetDC (hwnd);
    g_hdc_pic[0] = createDCByPicture (hdc, "res/1.png", 0, 0);
    g_hdc_pic[1] = createDCByPicture (hdc, "res/2.png", 0, 0);
    if (g_hdc_pic[0] == HDC_INVALID || g_hdc_pic[1] == HDC_INVALID) 
        return ;

    mGEffInit ();
    ReleaseDC(hdc);
    hdc = BeginPaint(hwnd);

#ifdef _MGEFF_EFFECTOR
    for( i = 0 ; i < TABLESIZE(effector_index ) ; i++ ) 
    {
        printf("mGEffFillAnimation:%d\n", i);
        mGEffFillAnimation(hwnd, g_hdc_pic[0], g_hdc_pic[1], hdc, i, 1000);
    }
#endif
    EndPaint(hwnd, hdc);

/*
    DeleteMemDC (g_hdc_pic[0]);
    DeleteMemDC (g_hdc_pic[1]);
*/
    mGEffDeinit ();
       

} 

static int TestWinProc (HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
    return DefaultMainWinProc (hWnd, message, wParam, lParam);
}
int MiniGUIMain (int argc, const char*  argv[])
{
//    MSG Msg;
    HWND hMainWnd;
    MAINWINCREATE CreateInfo;

#ifdef _MGRM_PROCESSES
    JoinLayer(NAME_DEF_LAYER , "mgeff test" , 0 , 0);
#endif


    CreateInfo.dwStyle = WS_VISIBLE;// | WS_BORDER | WS_CAPTION;
    CreateInfo.dwExStyle = WS_EX_NONE;
    CreateInfo.spCaption = "mgeff test";
    CreateInfo.hMenu = 0;
    CreateInfo.hCursor = GetSystemCursor(0);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = TestWinProc;
    CreateInfo.lx = 0;
    CreateInfo.ty = 0;
    CreateInfo.rx = 320;
    CreateInfo.by = 240;
    CreateInfo.iBkColor = COLOR_lightwhite;
    CreateInfo.dwAddData = 0;
    CreateInfo.hHosting = HWND_DESKTOP;

    hMainWnd = CreateMainWindow (&CreateInfo);

    if (hMainWnd == HWND_INVALID)
        return -1;

    ShowWindow(hMainWnd, SW_SHOWNORMAL);

/*
    while (GetMessage(&Msg, hMainWnd)) {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
*/
    TestEffector( hMainWnd ) ;

    MainWindowThreadCleanup (hMainWnd);

    return 0;
}

