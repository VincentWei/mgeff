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

#ifndef  WINDOW_ANIMATION_INC
#define  WINDOW_ANIMATION_INC

#include <stdio.h>
#include <stdlib.h>

#include "mgeff-window-animation.h"

typedef struct _WindowAnimationContext{
    HDC  m_src_dc; /* First frame of show/hide animation. */
    HDC  m_dst_dc; /* Last frame of show/hide animation. */
    HDC  m_sink_dc; /* The target DC for animation output */
    HWND m_hwnd; /* Animation window handle. */
    int  m_time; /* animation duration. */
    int  m_curve_type; /* move window motion-curve type. */
    int  m_type; /* show/hide animation effector type. */
    HDC  m_old_secdc;  /* Default secondary dc of animation window. */
    RECT m_old_rect; /* Origin location of main window */
    RECT m_new_rect; /* Origin location of main window */
    DWORD m_old_data; /* Origin AdditionalData of main window */
    BOOL m_paint;
    void (*set_background)(HDC hdc, RECT* offset_rc); /* background erase callback. */
    void (*redraw_at_moving)(HDC src, HDC dst, RECT* rc); /* draw window when it was moving. */
    WNDPROC m_old_proc;  /* Origin message processor of main window. */
    RENDER_HANDLER m_render_handler; /* animation property handle. */
} WindowAnimationContext;

static HDC hold_window_for_foreground(WindowAnimationContext* ctxt);
static void drop_window_release_foreground(WindowAnimationContext* ctxt);
static void animation_generate(MGEFF_ANIMATION handle, WindowAnimationContext *context, int id, RECT *rc);
static void effector_animation_play(MGEFF_WindowAnimCtxt* ctxt);
static LRESULT AnimateWindowProcHook(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
#endif   /* ----- #ifndef WINDOW_ANIMATION_INC  ----- */
