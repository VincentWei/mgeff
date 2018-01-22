/*!============================================================================
 * \file window-animation.h 
 * \brief  
 * \author DongKai
 * \version 1.0
 * \date 03/18/2010 03:44:41 PM 
 *
 *  Company: Beijing Feynman Software Technology Co., Ltd.
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

HDC hold_window_for_foreground(WindowAnimationContext* ctxt);
void drop_window_release_foreground(WindowAnimationContext* ctxt);
void animation_generate(MGEFF_ANIMATION handle, WindowAnimationContext *context, int id, RECT *rc);
void effector_animation_play(MGEFF_WindowAnimCtxt* ctxt);
LRESULT AnimateWindowProcHook(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
#endif   /* ----- #ifndef WINDOW_ANIMATION_INC  ----- */
