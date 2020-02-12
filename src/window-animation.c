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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <assert.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

#include "mgeff.h"
#include "animation.h"
#include "window-animation.h"

extern HDC GetSecondarySubDC (HDC secondary_dc, HWND hwnd_child, BOOL client);
extern void ReleaseSecondarySubDC (HDC secondary_subdc);

void default_background_generator(HDC hdc, RECT *rc)
{
    gal_pixel color = GetWindowElementPixelEx (HWND_DESKTOP, hdc, WE_BGC_DESKTOP);
    gal_pixel old_color = SetBrushColor(hdc, color);
    if (HDC_SCREEN == hdc) {
        FillBox(hdc, rc->left, rc->top, RECTWP(rc), RECTHP(rc));
    }
    else {
        FillBox(hdc, 0, 0, RECTWP(rc), RECTHP(rc));
    }
    SetBrushColor(hdc, old_color);
}

/* create window animation context.*/
MGEFF_WINDOW_ANIMATION_CONTEXT 
mGEffCreateWindowAnimationContext(int time, int effector_type, int curve_type, SET_BKGND_CB func, DRAW_WND_CB drawer)
{
    WindowAnimationContext * ctxt = (WindowAnimationContext *)calloc(1, sizeof(*ctxt));
    if (NULL != ctxt) {
        ctxt->m_hwnd = HWND_NULL;
        ctxt->m_src_dc = HDC_INVALID;
        ctxt->m_dst_dc = HDC_INVALID;
        ctxt->m_old_secdc = HDC_INVALID;
        ctxt->m_old_data = 0;
        ctxt->m_old_proc = NULL;
        ctxt->m_render_handler = NULL;
        ctxt->m_paint = FALSE;
        ctxt->m_time = time;
        ctxt->m_type = effector_type;
        ctxt->m_curve_type = curve_type;
        ctxt->set_background = func ? func : default_background_generator;
        ctxt->redraw_at_moving = drawer;
    }
    return (MGEFF_WINDOW_ANIMATION_CONTEXT)ctxt;
}

RENDER_HANDLER mGEffSetEffectorPropertyHandler(MGEFF_WINDOW_ANIMATION_CONTEXT hctxt, RENDER_HANDLER new_cb)
{
    WindowAnimationContext * ctxt = (WindowAnimationContext*)hctxt;
    RENDER_HANDLER old_handle = NULL;
    if (NULL != ctxt) {
        old_handle = ctxt->m_render_handler;
        ctxt->m_render_handler = new_cb;
    }
    return old_handle;
}

/* destroy window animation context.*/
void mGEffDestroyWindowAnimationContext(MGEFF_WINDOW_ANIMATION_CONTEXT hctxt)
{
    WindowAnimationContext * ctxt = (WindowAnimationContext*)hctxt;
    free(ctxt);
}

/* get foreground of the window  */
HDC hold_window_for_foreground(WindowAnimationContext* ctxt) 
{
    if (NULL != ctxt && HDC_INVALID == ctxt->m_old_secdc && HWND_INVALID != ctxt->m_hwnd)
    {
        HDC sec_dc = GetSecondaryDC(ctxt->m_hwnd);
        BOOL has_secondary_dc = (sec_dc != HDC_SCREEN);
        HDC memdc = sec_dc;
        // Set memdc as secondary dc, and make the secondary dc update function to do nothing.
        if (!has_secondary_dc) {
            memdc = CreateSecondaryDC(ctxt->m_hwnd);
        }
        ctxt->m_old_secdc = SetSecondaryDC (ctxt->m_hwnd, memdc, ON_UPDSECDC_DONOTHING);
        UpdateWindow(ctxt->m_hwnd, TRUE);
        return memdc;
    }
    assert(FALSE);
    return HDC_INVALID;
}

void drop_window_release_foreground(WindowAnimationContext* ctxt)
{
    if (NULL != ctxt && HDC_INVALID != ctxt->m_old_secdc) {
        if (HDC_SCREEN != GetSecondaryDC(ctxt->m_hwnd)) {
            if (HDC_SCREEN != ctxt->m_old_secdc) { // Make sure the window setted secondary dc ever.
                SetSecondaryDC(ctxt->m_hwnd, ctxt->m_old_secdc, ON_UPDSECDC_DEFAULT);
            }
            else {
                DeleteSecondaryDC(ctxt->m_hwnd);
            }
            ctxt->m_old_secdc = HDC_INVALID;
        }
    }
}

HDC mGEffGetWindowForeground(HWND hwnd, BOOL set_foreground)
{
    if (HWND_INVALID != hwnd) {
        HDC sec_dc = GetSecondaryDC(hwnd);
        HDC memdc = HDC_INVALID; 
        BOOL has_secondary_dc = (sec_dc != HDC_SCREEN);
        if (!has_secondary_dc) {
            sec_dc = CreateSecondaryDC(hwnd);
        }
        memdc = CreateCompatibleDC(sec_dc);
        // Set memdc as secondary dc, and make the secondary dc update function to do nothing.
        SetSecondaryDC(hwnd, sec_dc, ON_UPDSECDC_DONOTHING);
        if (set_foreground) {
            ShowWindow(hwnd, SW_SHOW);
            UpdateWindow(hwnd, TRUE);
        }
        else
        {
            if (!IsWindowVisible(hwnd)) {
                IncludeWindowStyle(hwnd, WS_VISIBLE);
                UpdateWindow(hwnd, TRUE);
                ExcludeWindowStyle(hwnd, WS_VISIBLE);
            }
            else {
                UpdateWindow(hwnd, TRUE);
            }
        }
        if (HDC_INVALID != memdc) {
            BitBlt(sec_dc, 0, 0, -1, -1, memdc, 0, 0, -1);
        }
        /* If window haven't secondary dc ago.*/
        if (!has_secondary_dc) {
            DeleteSecondaryDC(hwnd);
        }
        else {
            SetSecondaryDC(hwnd, sec_dc, ON_UPDSECDC_DEFAULT);
        }
        return memdc;
    }
    return HDC_INVALID;
}

HDC mGEffHoldWindowForeground(HWND hwnd, MGEFF_WINDOW_ANIMATION_CONTEXT hctxt)
{
    WindowAnimationContext * ctxt = (WindowAnimationContext *)hctxt;
    if (NULL != ctxt) {
        ctxt->m_hwnd = hwnd;
    }
    return hold_window_for_foreground(ctxt);
}

void mGEffDropWindowForeground(MGEFF_WINDOW_ANIMATION_CONTEXT hctxt)
{
    drop_window_release_foreground((WindowAnimationContext*)hctxt);
}

// hook for user msg-proc
LRESULT AnimateWindowProcHook(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
        case MSG_NCCREATE:
            {
                WindowAnimationContext* ctxt = (WindowAnimationContext*)GetWindowAdditionalData(hWnd);
                if (NULL != ctxt) ctxt->m_old_proc(hWnd, message, wParam, lParam);
            }
            break;
        case MSG_CREATE:
            {
                WindowAnimationContext* ctxt = (WindowAnimationContext*)GetWindowAdditionalData(hWnd);
                SetWindowAdditionalData(hWnd, ctxt->m_old_data); // restore AdditionalData.
                ctxt->m_old_proc(hWnd, message, wParam, lParam);
                // restore msg-proc function.
                SetWindowCallbackProc(hWnd, ctxt->m_old_proc);
                return 0;
            }
            break;
        case MSG_NCPAINT:
            return 0;
        default:break;
    }
    return DefaultWindowProc(hWnd, message, wParam, lParam);
}

void effector_animation_play(MGEFF_WindowAnimCtxt* _ctxt)
{
    MGEFF_ANIMATION anim;
    WindowAnimationContext* ctxt = (WindowAnimationContext*)_ctxt;
    MGEFF_EFFECTOR effector = mGEffEffectorCreateEx(ctxt->m_src_dc, ctxt->m_dst_dc, 
            ctxt->m_sink_dc, ctxt->m_type);
    mGEffSetBufferSink(mGEffEffectorGetSink(effector), ctxt->m_src_dc);
    if (NULL != ctxt->m_render_handler) {
        ctxt->m_render_handler(effector);
    }
    anim = mGEffAnimationCreateWithEffector(effector);
    mGEffAnimationSetDuration(anim, ctxt->m_time);
    mGEffAnimationSetCurve(anim, ctxt->m_curve_type);
    mGEffAnimationSyncRun(anim);
    mGEffEffectorDelete(effector);
}

HWND mGEffCreateMainWindow(PMAINWINCREATE pCreateInfo, MGEFF_WINDOW_ANIMATION_CONTEXT hctxt)
{
    WindowAnimationContext * ctxt = (WindowAnimationContext *)hctxt;
    if (NULL != pCreateInfo && NULL != ctxt)
    {
        HWND hwnd = HWND_INVALID;
        int w = pCreateInfo->rx - pCreateInfo->lx;
        int h = pCreateInfo->by - pCreateInfo->ty;

        ctxt->m_old_secdc = HDC_INVALID;
        ctxt->m_old_proc = pCreateInfo->MainWindowProc; // store the window msg-proc
        pCreateInfo->MainWindowProc = AnimateWindowProcHook; // hook the msg-proc function
        ctxt->m_old_data = pCreateInfo->dwAddData; // store AdditionalData of window
        ctxt->m_src_dc = CreateCompatibleDCEx(HDC_SCREEN, w, h);
        if (HDC_INVALID != ctxt->m_src_dc) {
            BitBlt(HDC_SCREEN, pCreateInfo->lx, pCreateInfo->ty, 
                    w, h, ctxt->m_src_dc, 0, 0, -1);
            pCreateInfo->dwAddData = (DWORD)ctxt; // replace the window AdditionalData
            hwnd = CreateMainWindow(pCreateInfo);

            if (HWND_INVALID != hwnd) {
                RECT rc;
                HDC sub_dc;
                GetWindowRect(hwnd, &rc);

                sub_dc = GetDC(hwnd);
                ctxt->m_hwnd = hwnd; // store the hwnd.
                ctxt->m_dst_dc = mGEffGetWindowForeground(hwnd, TRUE);
                // Create cache dc witch have same size of window for store begin frame.
                ctxt->m_sink_dc = sub_dc;
                effector_animation_play((MGEFF_WindowAnimCtxt*)ctxt);
                DeleteMemDC(ctxt->m_dst_dc);  // Release window foreground.
                ReleaseDC(sub_dc);
            }
            DeleteMemDC(ctxt->m_src_dc);
            return hwnd;
        }

    }
    return HWND_INVALID;
}

HWND mGEffCreateMainWindowEx(PMAINWINCREATE pCreateInfo, 
        MGEFF_WINDOW_ANIMATION_CONTEXT hctxt, MGEFF_WINDOWANIM_CB cb)
{
    WindowAnimationContext * ctxt = (WindowAnimationContext *)hctxt;
    if (NULL != pCreateInfo && NULL != ctxt)
    {
        HWND hwnd = HWND_INVALID;
        int w = pCreateInfo->rx - pCreateInfo->lx;
        int h = pCreateInfo->by - pCreateInfo->ty;

        ctxt->m_old_secdc = HDC_INVALID;
        ctxt->m_old_proc = pCreateInfo->MainWindowProc; // store the window msg-proc
        pCreateInfo->MainWindowProc = AnimateWindowProcHook; // hook the msg-proc function
        ctxt->m_old_data = pCreateInfo->dwAddData; // store AdditionalData of window
        ctxt->m_src_dc = CreateCompatibleDCEx(HDC_SCREEN, w, h);
        if (HDC_INVALID != ctxt->m_src_dc) {
            BitBlt(HDC_SCREEN, pCreateInfo->lx, pCreateInfo->ty, 
                    w, h, ctxt->m_src_dc, 0, 0, -1);
            pCreateInfo->dwAddData = (DWORD)ctxt; // replace the window AdditionalData
            hwnd = CreateMainWindow(pCreateInfo);

            if (HWND_INVALID != hwnd) {
                RECT rc;
                HDC sub_dc;
                GetWindowRect(hwnd, &rc);

                sub_dc = GetDC(hwnd);
                ctxt->m_hwnd = hwnd; // store the hwnd.
                ctxt->m_dst_dc = mGEffGetWindowForeground(hwnd, TRUE);
                // Create cache dc witch have same size of window for store begin frame.
                ctxt->m_sink_dc = sub_dc;

                if (!cb)
                    cb = (MGEFF_WINDOWANIM_CB)effector_animation_play;
                cb((MGEFF_WindowAnimCtxt*)ctxt);

                DeleteMemDC(ctxt->m_dst_dc); // Release window foreground.
                ReleaseDC(sub_dc);
            }
            DeleteMemDC(ctxt->m_src_dc);
            return hwnd;
        }

    }
    return HWND_INVALID;
}

typedef struct _CTXT{
    HDC  dc1;
    HDC  dc2;
    HDC  dst_dc;
    HWND hwnd;
    int w;
    int h;
    POINT prev_pt;
}CTXT;

#if 0
static void ondraw(MGEFF_ANIMATION handle, void* _ctxt, int id, void* value)
{
    CTXT* ctxt = (CTXT*)_ctxt;
    POINT* pt = (POINT*)value;
    BitBlt(ctxt->dc2, ctxt->prev_pt.x, ctxt->prev_pt.y, ctxt->w, ctxt->h, 
            ctxt->dst_dc, ctxt->prev_pt.x, ctxt->prev_pt.y, 0);
    BitBlt(ctxt->dc1, 0, 0, 0, 0, ctxt->dst_dc, pt->x, pt->y, 0);
    ctxt->prev_pt.x = pt->x;
    ctxt->prev_pt.y = pt->y;
}

static void control_animation_play(MGEFF_WindowAnimCtxt* _ctxt)
{
    POINT pt_s = {0, 0};
    POINT pt_e = {0, 0};
    RECT  rc;
    CTXT* ctxt = (CTXT*)_ctxt;
    MGEFF_ANIMATION anim = mGEffAnimationCreate(ctxt, ondraw, 0, MGEFF_POINT);
    GetWindowRect(ctxt->hwnd, &rc);
    ctxt->w = RECTW(rc);
    ctxt->h = RECTH(rc);
    ClientToScreen(ctxt->hwnd, &pt_e.x, &pt_e.y);
    //fprintf(stderr, "rc.left=%d,rc.top=%d,e.x=%d,e.y=%d.\n", rc.left, rc.top, pt_e.x, pt_e.y);
    mGEffAnimationSetStartValue(anim, &pt_s);
    mGEffAnimationSetEndValue(anim, &pt_e);
    mGEffAnimationSetDuration(anim, 1000);
    mGEffAnimationSyncRun(anim);
}

static CTXT* hold_control_for_foreground(HWND hWnd, HWND hControl, CTXT* ctxt)
{
    HDC secondary_dc = CreateSecondaryDC(hWnd);
    HDC memdc = CreateCompatibleDC(HDC_SCREEN);
    HDC tmp_button_dc;
    HDC button_dc;
    HDC old_secondary_dc = GetSecondaryDC(hWnd);

    SetSecondaryDC(hWnd, secondary_dc, ON_UPDSECDC_DONOTHING);
    tmp_button_dc = GetSecondarySubDC(secondary_dc, hControl, FALSE);
    button_dc = CreateCompatibleDC(tmp_button_dc);

    IncludeWindowStyle(hControl, WS_VISIBLE);
    UpdateWindow(hControl, TRUE);
    ExcludeWindowStyle(hControl, WS_VISIBLE); /* hide, else will disaply when update hParent. */
    BitBlt(tmp_button_dc, 0, 0, 0, 0, button_dc, 0, 0, 0);

    SetSecondaryDC(hWnd, secondary_dc, ON_UPDSECDC_DEFAULT);
    ReleaseSecondarySubDC(tmp_button_dc);
    UpdateWindow(hWnd, TRUE);

    ctxt->dc1 = button_dc;
    ctxt->dc2 = memdc;
    ctxt->hwnd = hControl;
    BitBlt(HDC_SCREEN, 0, 0, 0, 0, memdc, 0, 0, 0);
    if (!old_secondary_dc || old_secondary_dc == HDC_SCREEN) {
        DeleteSecondaryDC(hWnd);
    }
    return ctxt;
}

static void drop_control_release_foreground(CTXT* ctxt)
{
    if (NULL != ctxt) {
        DeleteMemDC(ctxt->dc1);
        DeleteMemDC(ctxt->dc2);
    }
}
#endif

// hook for user msg-proc
int AnimateControlProcHook(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
        case MSG_NCCREATE:
            {
                WindowAnimationContext* ctxt = (WindowAnimationContext*)GetWindowAdditionalData(hWnd);
                if (NULL != ctxt) ctxt->m_old_proc(hWnd, message, wParam, lParam);
            }
            break;
        case MSG_CREATE:
            {
                WindowAnimationContext* ctxt = (WindowAnimationContext*)GetWindowAdditionalData(hWnd);
                SetWindowAdditionalData(hWnd, ctxt->m_old_data); // restore AdditionalData.
                ctxt->m_old_proc(hWnd, message, wParam, lParam);
                // restore msg-proc function.
                SetWindowCallbackProc(hWnd, ctxt->m_old_proc);
                return 0;
            }
            break;
        case MSG_NCPAINT:
            return 0;
        default:break;
    }
    return DefaultControlProc(hWnd, message, wParam, lParam);
}

BOOL mGEffShowWindow (HWND hwnd, int iCmdShow, MGEFF_WINDOW_ANIMATION_CONTEXT hctxt)
{
    WindowAnimationContext * ctxt = (WindowAnimationContext*)hctxt;
    RECT rc;
    HDC sub_dc;
    GetWindowRect(hwnd, &rc);
    sub_dc = GetDC(hwnd);

    if (HDC_INVALID != sub_dc && NULL != ctxt) {
        switch (iCmdShow)
        {
            case SW_SHOWNORMAL:
            case SW_SHOW:
                ctxt->m_hwnd = hwnd;
                ctxt->m_src_dc = CreateCompatibleDCEx(HDC_SCREEN, RECTW(rc), RECTH(rc));
                BitBlt(HDC_SCREEN, rc.left, rc.top, RECTW(rc), RECTH(rc), ctxt->m_src_dc, 0, 0, -1);
                ctxt->m_dst_dc = mGEffGetWindowForeground(hwnd, TRUE); // get the window foreground content.
                ctxt->m_sink_dc = sub_dc;
                effector_animation_play((MGEFF_WindowAnimCtxt*)ctxt);
                if (SW_SHOWNORMAL == iCmdShow) {
                    ShowWindow(hwnd, SW_SHOWNORMAL);
                }

                DeleteMemDC(ctxt->m_dst_dc); // Release window foreground.
                DeleteMemDC(ctxt->m_src_dc);
                break;

            case SW_HIDE:
                ctxt->m_src_dc = CreateCompatibleDC(sub_dc);
                BitBlt(sub_dc, 0, 0, -1, -1, ctxt->m_src_dc, 0, 0, -1);
                ctxt->m_dst_dc = CreateCompatibleDCEx(HDC_SCREEN, RECTW(rc), RECTH(rc));
                if (ctxt->set_background) {
                    // set background via callback.
                    ctxt->set_background(ctxt->m_dst_dc, &rc);
                }
                ctxt->m_sink_dc = sub_dc;
                // animation generated from context.
                effector_animation_play((MGEFF_WindowAnimCtxt*)ctxt);
                ShowWindow(hwnd, SW_HIDE);

                DeleteMemDC(ctxt->m_src_dc);
                DeleteMemDC(ctxt->m_dst_dc);
                break;
        }

#if 0
        if (iCmdShow) { // Show window.
            ctxt->m_hwnd = hwnd;
            ctxt->m_src_dc = CreateCompatibleDCEx(HDC_SCREEN, RECTW(rc), RECTH(rc));
            BitBlt(HDC_SCREEN, rc.left, rc.top, RECTW(rc), RECTH(rc), ctxt->m_src_dc, 0, 0, -1);
            ctxt->m_dst_dc = mGEffGetWindowForeground(hwnd, TRUE); // get the window foreground content.
            ctxt->m_sink_dc = sub_dc;
            effector_animation_play(ctxt);
            DeleteMemDC(ctxt->m_dst_dc); // Release window foreground.
            DeleteMemDC(ctxt->m_src_dc);
        }
        else { // Hide window.
            ctxt->m_src_dc = CreateCompatibleDC(sub_dc);
            BitBlt(sub_dc, 0, 0, -1, -1, ctxt->m_src_dc, 0, 0, -1);
            ctxt->m_dst_dc = CreateCompatibleDCEx(HDC_SCREEN, RECTW(rc), RECTH(rc));
            if (ctxt->set_background) {
                // set background via callback.
                ctxt->set_background(ctxt->m_dst_dc, &rc);
            }
            ctxt->m_sink_dc = sub_dc;
            // animation generated from context.
            effector_animation_play(ctxt);
            ShowWindow(hwnd, SW_HIDE);

            DeleteMemDC(ctxt->m_src_dc);
            DeleteMemDC(ctxt->m_dst_dc);
        }
#endif
        ReleaseDC(sub_dc);
        return TRUE;
    }
    return FALSE;
}

BOOL mGEffShowWindowEx (HWND hwnd, int iCmdShow, 
        MGEFF_WINDOW_ANIMATION_CONTEXT hctxt, MGEFF_WINDOWANIM_CB cb)
{
    WindowAnimationContext* ctxt = (WindowAnimationContext*)hctxt;
    RECT rc;
    HDC sub_dc;
    GetWindowRect(hwnd, &rc);
    sub_dc = GetDC(hwnd);

    if (HDC_INVALID != sub_dc && NULL != ctxt) {
        switch (iCmdShow)
        {
            case SW_SHOWNORMAL:
            case SW_SHOW:
                ctxt->m_hwnd = hwnd;
                ctxt->m_src_dc = CreateCompatibleDCEx(HDC_SCREEN, RECTW(rc), RECTH(rc));
                BitBlt(HDC_SCREEN, rc.left, rc.top, RECTW(rc), RECTH(rc), ctxt->m_src_dc, 0, 0, -1);
                ctxt->m_dst_dc = mGEffGetWindowForeground(hwnd, TRUE); // get the window foreground content.
                ctxt->m_sink_dc = sub_dc;

                // animation generated from context.
                if (!cb)
                    cb = effector_animation_play;
                cb((MGEFF_WindowAnimCtxt*)ctxt);
                if (SW_SHOWNORMAL == iCmdShow) {
                    ShowWindow(hwnd, SW_SHOWNORMAL);
                }

                DeleteMemDC(ctxt->m_dst_dc);
                DeleteMemDC(ctxt->m_src_dc);

                break;
            case SW_HIDE:
                ctxt->m_src_dc = CreateCompatibleDC(sub_dc);
                BitBlt(sub_dc, 0, 0, -1, -1, ctxt->m_src_dc, 0, 0, -1);
                ctxt->m_dst_dc = CreateCompatibleDCEx(HDC_SCREEN, RECTW(rc), RECTH(rc));
                if (ctxt->set_background) {
                    // set background via callback.
                    ctxt->set_background(ctxt->m_dst_dc, &rc);
                }
                ctxt->m_sink_dc = sub_dc;

                // animation generated from context.
                if (!cb)
                    cb = effector_animation_play;
                cb((MGEFF_WindowAnimCtxt*)ctxt);

                ShowWindow(hwnd, SW_HIDE);

                DeleteMemDC(ctxt->m_src_dc);
                DeleteMemDC(ctxt->m_dst_dc);
                break;
        }

#if 0
        if (iCmdShow) { // Show window.
            ctxt->m_hwnd = hwnd;
            ctxt->m_src_dc = CreateCompatibleDCEx(HDC_SCREEN, RECTW(rc), RECTH(rc));
            BitBlt(HDC_SCREEN, rc.left, rc.top, RECTW(rc), RECTH(rc), ctxt->m_src_dc, 0, 0, -1);
            ctxt->m_dst_dc = mGEffGetWindowForeground(hwnd, TRUE); // get the window foreground content.
            ctxt->m_sink_dc = sub_dc;

            // animation generated from context.
            if (!cb)
                cb = (MGEFF_WINDOWANIM_CB)effector_animation_play;
            cb((MGEFF_WindowAnimCtxt*)ctxt);

            DeleteMemDC(ctxt->m_dst_dc);
            DeleteMemDC(ctxt->m_src_dc);
        }
        else { // Hide window.
            ctxt->m_src_dc = CreateCompatibleDC(sub_dc);
            BitBlt(sub_dc, 0, 0, -1, -1, ctxt->m_src_dc, 0, 0, -1);
            ctxt->m_dst_dc = CreateCompatibleDCEx(HDC_SCREEN, RECTW(rc), RECTH(rc));
            if (ctxt->set_background) {
                // set background via callback.
                ctxt->set_background(ctxt->m_dst_dc, &rc);
            }
            ctxt->m_sink_dc = sub_dc;

            // animation generated from context.
            if (!cb)
                cb = (MGEFF_WINDOWANIM_CB)effector_animation_play;
            cb((MGEFF_WindowAnimCtxt*)ctxt);

            ShowWindow(hwnd, SW_HIDE);

            DeleteMemDC(ctxt->m_src_dc);
            DeleteMemDC(ctxt->m_dst_dc);
        }
#endif
        ReleaseDC(sub_dc);
        return TRUE;
    }
    return FALSE;



}

void animation_generate(MGEFF_ANIMATION handle, WindowAnimationContext *context, int id, RECT *rc) 
{
    if (1 == id) {
        RECT old_rc = context->m_new_rect;
        RECT diff_rc[4];
        int rc_num = 0, i = 0;
        rc_num = SubtractRect(diff_rc, &old_rc, rc);
        // calc dirty area after window move, and invoke callback redraw them.
        for (; i < rc_num; ++i) {
            context->set_background(HDC_SCREEN, &diff_rc[i]);
        }
        context->m_new_rect = *rc;
        if (RECTWP(rc) != RECTW(context->m_old_rect) 
                || RECTHP(rc) != RECTH(context->m_old_rect)) {
//            SendAsyncMessage(context->m_hwnd, MSG_CHANGESIZE, (WPARAM)rc, (LPARAM)(&rc_result));
            if (MoveWindow(context->m_hwnd, rc->left, rc->top, 
                    RECTWP(rc), RECTHP(rc), FALSE)) {
                DeleteMemDC(context->m_src_dc);
                /* Refetch window content. */
                context->m_src_dc = mGEffGetWindowForeground(context->m_hwnd, FALSE);
                assert(HDC_INVALID != context->m_src_dc);
            }
        }
        /* Allow user draw window at moving. */
        if (NULL != context->redraw_at_moving) {
            context->redraw_at_moving(context->m_src_dc, HDC_SCREEN, rc);
        }
        else {
            BitBlt(context->m_src_dc, 0, 0, -1, -1, HDC_SCREEN, rc->left, rc->top, -1);
        }
    }
}

void  move_anim_finished(MGEFF_ANIMATION handle)
{
    WindowAnimationContext* ctxt = (WindowAnimationContext*)mGEffAnimationGetContext(handle);
    if (NULL != ctxt) {
        MoveWindow(ctxt->m_hwnd, ctxt->m_new_rect.left, ctxt->m_new_rect.top, 
                RECTW(ctxt->m_new_rect), RECTH(ctxt->m_new_rect), ctxt->m_paint);
        if (GetWindowExStyle(ctxt->m_hwnd) & WS_EX_TOPMOST) {
            /* Can't replace to SW_SHOW process method because WS_EX_TOPMOST need adjust Z-order
             * via internal invoke. */
            ShowWindow(ctxt->m_hwnd, SW_SHOWNORMAL);
        }
        else {
            //zhaochengzhang: ShowWindow is a must to enable the window to receive mouse input
            ShowWindow(ctxt->m_hwnd, SW_SHOW);
            /* DK:Replace ShowWindow to below codes for avoid flicker.  */
            /*IncludeWindowStyle(ctxt->m_hwnd, WS_VISIBLE);
            SendAsyncMessage (ctxt->m_hwnd, MSG_NCPAINT, 0, 0);
            InvalidateRect (ctxt->m_hwnd, NULL, FALSE);
            SendNotifyMessage (ctxt->m_hwnd, MSG_SHOWWINDOW, (WPARAM)SW_SHOW, 0); */
        }
        DeleteMemDC(ctxt->m_src_dc);
        mGEffDestroyWindowAnimationContext((MGEFF_WINDOW_ANIMATION_CONTEXT)ctxt);
    }
}

void  ready_for_move(MGEFF_ANIMATION handle, enum EffState newEffState, enum EffState oldEffState)
{
    WindowAnimationContext* ctxt = (WindowAnimationContext*)mGEffAnimationGetContext(handle);
    if (MGEFF_STATE_READY == oldEffState 
            && MGEFF_STATE_RUNNING == newEffState
            && NULL != ctxt) {
        ShowWindow(ctxt->m_hwnd, SW_HIDE);
    }
}

MGEFF_ANIMATION mGEffMoveWindowPreparation(HWND hWnd, int x, int y, int w, int h, 
        BOOL fPaint, MGEFF_WINDOW_ANIMATION_CONTEXT hctxt)
{
    WindowAnimationContext * ctxt = (WindowAnimationContext*)hctxt;
    RECT source_rc;
    RECT target_rc = {x, y, x + w, y + h};
    GetWindowRect(hWnd, &source_rc);
    if (source_rc.left == x && source_rc.top == y &&
            RECTW(source_rc) == w && RECTH(source_rc) == h) {
        return (MGEFF_ANIMATION)NULL;
    }
    if (NULL != ctxt) {
        ctxt->m_src_dc = mGEffGetWindowForeground(hWnd, FALSE);
        if (HDC_INVALID != ctxt->m_src_dc) {
            WindowAnimationContext *ctxt_copy = (WindowAnimationContext*)malloc(sizeof(*ctxt_copy));
            if (NULL != ctxt_copy) {
                MGEFF_ANIMATION animation = mGEffAnimationCreate((void*)ctxt_copy, 
                        (void *)animation_generate, 1, MGEFF_RECT);
                memcpy(ctxt_copy, ctxt, sizeof(*ctxt_copy));
                ctxt_copy->m_new_rect = ctxt_copy->m_old_rect = source_rc;
                ctxt_copy->m_paint = fPaint;
                ctxt_copy->m_hwnd = hWnd;
                mGEffAnimationSetStartValue(animation, &source_rc);
                mGEffAnimationSetEndValue(animation, &target_rc);
                mGEffAnimationSetDuration(animation, ctxt_copy->m_time);
                mGEffAnimationSetCurve(animation, ctxt_copy->m_curve_type);

                mGEffAnimationSetContext(animation, ctxt_copy);
                mGEffAnimationSetFinishedCb(animation, move_anim_finished);
                mGEffAnimationSetStateChangedCb(animation, ready_for_move);
                return animation;
            }
            else {
                DeleteMemDC(ctxt->m_src_dc);
            }
        }
        else {
            fprintf(stderr, "mGEffGetWindowForeground failed.\n");
        }
    }

    return (MGEFF_ANIMATION)NULL;
}

BOOL mGEffMoveWindow (HWND hWnd, int x, int y, int w, int h, BOOL fPaint, MGEFF_WINDOW_ANIMATION_CONTEXT hctxt)
{
    WindowAnimationContext * ctxt = (WindowAnimationContext*)hctxt;
    if (NULL != ctxt) {
        MGEFF_ANIMATION animation = mGEffMoveWindowPreparation(hWnd, x, y, w, h, 
                fPaint, hctxt);
        if (animation) {
            mGEffAnimationSyncRun(animation);
            mGEffAnimationStop(animation);
            mGEffAnimationDelete(animation);
            return TRUE;
        }
        mGEffDestroyWindowAnimationContext(hctxt);
    }
    return FALSE;
}


LRESULT ExtractWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
        case MSG_PAINT:
            {
                HDC hdc = BeginPaint (hWnd);
                HDC foreground = (HDC)GetWindowAdditionalData(hWnd);
                if (foreground && HDC_INVALID != foreground) {
                    StretchBlt(foreground, 0, 0, 0, 0, hdc, 0, 0, 0, 0, -1);
                }
                EndPaint(hWnd, hdc);
            }
            return 0;
        case MSG_CLOSE:
            {
                HDC hdc = (HDC)GetWindowAdditionalData(hWnd);
                if (hdc && HDC_INVALID != hdc) {
                    DeleteMemDC(hdc);
                }
                DestroyMainWindow (hWnd);
                PostQuitMessage (hWnd);
            }
            break;
        default:break;
    }
    return DefaultWindowProc(hWnd, message, wParam, lParam);
}

/* Extract a control from main window, and create a new main window take that control content. */
BOOL ControlExtract(HWND mainHwnd, int ctrl_id, PMAINWINCREATE pCreateInfo)
{
    BOOL ret = FALSE;
    if (HWND_NULL != mainHwnd && NULL != pCreateInfo) {
        HWND hctrl = GetDlgItem(mainHwnd, ctrl_id);
        if (HWND_NULL != hctrl) {
            HDC hdc = HDC_INVALID;
            HDC sub_dc = HDC_INVALID;
            HDC clone_dc = HDC_INVALID;
            RECT rc;
            MGEFF_WINDOW_ANIMATION_CONTEXT hctxt = 
                mGEffCreateWindowAnimationContext(1000, 0, 0, NULL, NULL);
            WindowAnimationContext* ctxt = (WindowAnimationContext*)hctxt;
            ctxt->m_hwnd = mainHwnd;
            hdc = hold_window_for_foreground(ctxt);

            if (HDC_INVALID != hdc) {
                sub_dc = GetSecondarySubDC (hdc, hctrl, FALSE);
                clone_dc = CreateCompatibleDC(sub_dc);
                if (HDC_INVALID != clone_dc) {
                    if (GetClientRect(hctrl, &rc)) {
                        int x = rc.left;
                        int y = rc.top;
                        ClientToScreen(hctrl, &x, &y);
                        BitBlt(sub_dc, 0, 0, 0, 0, clone_dc, 0, 0, -1);
                        ReleaseSecondarySubDC(sub_dc);
                        pCreateInfo->MainWindowProc = ExtractWindowProc;
                        pCreateInfo->lx = x;
                        pCreateInfo->ty = y;
                        pCreateInfo->rx = pCreateInfo->lx + RECTW(rc);
                        pCreateInfo->by = pCreateInfo->ty + RECTH(rc);
                        pCreateInfo->dwAddData = (DWORD)clone_dc;
                        ret = TRUE;
                    }
                    else {
                        DeleteMemDC(clone_dc);
                        fprintf(stderr, "[ControlExtract] Can't take the window rect!\n");
                    }
                }

                drop_window_release_foreground(ctxt);
            }
        }
    }
    return ret;
}

BOOL WindowExtract(HWND mainHwnd, RECT *rc, PMAINWINCREATE pCreateInfo)
{
    BOOL ret = FALSE;
    if (HWND_INVALID != mainHwnd && NULL != rc && NULL != pCreateInfo) {
        HDC hdc;
        MGEFF_WINDOW_ANIMATION_CONTEXT hctxt = 
            mGEffCreateWindowAnimationContext(1000, 0, 0, NULL, NULL);
        WindowAnimationContext* ctxt = (WindowAnimationContext*)hctxt;
        ctxt->m_hwnd = mainHwnd;
        hdc = hold_window_for_foreground(ctxt);
        if (HDC_INVALID != hdc) {
            HDC clone_dc = CreateCompatibleDCEx(hdc, RECTWP(rc), RECTHP(rc));
            RECT pos_rc;
            GetWindowRect(mainHwnd, &pos_rc);
            BitBlt(hdc, rc->left, rc->top, RECTWP(rc), RECTHP(rc), clone_dc, 0, 0, -1);
//            BitBlt(hdc, 0, 0, 0, 0, HDC_SCREEN, 0, 0, -1);
            pCreateInfo->MainWindowProc = ExtractWindowProc;
            pCreateInfo->lx = pos_rc.left + rc->left;
            pCreateInfo->ty = pos_rc.top + rc->top;
            pCreateInfo->rx = pCreateInfo->lx + RECTWP(rc);
            pCreateInfo->by = pCreateInfo->ty + RECTHP(rc);
            pCreateInfo->dwAddData = (DWORD)clone_dc;
            ret = TRUE;
        }
        else {
            fprintf(stderr, "[WindowExtract] Can't take the foreground\n");
        }
        drop_window_release_foreground(ctxt);
    }
    else {
        fprintf(stderr, "[WindowExtract] Paraments invalid\n");
    }
    return ret;
}
