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

/**************************************************************
*  This file is generated automatically, don't modify it.
**************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/times.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <mgncs/mgncs.h>
#include "resource.h"
#include "ncs-windows.h"

#include <mgeff/mgeff.h>

static BOOL bfirst_draw_curve = 1;
static BOOL bfirst_draw_block = 1;
static BOOL bfrist_draw_slideball = 1;
#define TIMER_START 100
#define DURATION 2000

static mWidget *g_mainwnd;
static int last_curve;

static struct {
    int type;
    const char *name;
} s_curves [] = {
    { InOutCubic, "InOutCubic" },
    { OutInCubic, "OutInCubic" },
    { InQuart, "InQuart" },
    { Linear, "Linear" },
    { InQuad, "InQuad" },
    { OutQuad, "OutQuad" },
    { InOutQuad, "InOutQuad" },
    { OutInQuad, "OutInQuad" },
    { InCubic, "InCubic" },
    { OutCubic, "OutCubic" },
    { OutQuart, "OutQuart" },
    { InOutQuart, "InOutQuart" },
    { OutInQuart, "OutInQuart" },
    { InQuint, "InQuint" },
    { OutQuint, "OutQuint" },
    { InOutQuint, "InOutQuint" },
    { OutInQuint, "OutInQuint" },
    { OutInSine, "OutInSine" },
    { OutInExpo, "OutInExpo" },
    { InCirc, "InCirc" },
    { OutCirc, "OutCirc" },
    { InOutCirc, "InOutCirc" },
    { OutInCirc, "OutInCirc" },
    { InBack, "InBack" },
    { OutBack, "OutBack" },
    { InOutBack, "InOutBack" },
    { OutInBack, "OutInBack" },
    { InBounce, "InBounce" },
    { OutBounce, "OutBounce" },
    { InOutBounce, "InOutBounce" },
    { OutInBounce, "OutInBounce" },
    { InCurve, "InCurve" },
    { OutCurve, "OutCurve" },

#if 0 /*can not be use*/
    { InSine, "InSine" },
    { OutSine, "OutSine" },
    { InOutSine, "InOutSine" },
    { InExpo, "InExpo" },
    { OutExpo, "OutExpo" },
    { InOutExpo, "InOutExpo" },
    { InElastic, "InElastic" },
    { OutElastic, "OutElastic" },
    { InOutElastic, "InOutElastic" },
    { OutInElastic, "OutInElastic" },
    { SineCurve, "SineCurve" },
    { CosineCurve, "CosineCurve" },
#endif
};

/* curve */
static int setPropertyCurve (MGEFF_ANIMATION animation, mWidget *obj, int id, int *pY)
{
    static POINT lastpoint = {0, 0};
    static clock_t start_time;
    static int total_t=1;
    HDC dc;
    RECT rc;
    int x, y;
    float t;

    y = *pY;
    GetClientRect(obj->hwnd, &rc);

    dc = BeginPaint(obj->hwnd);

    if (bfirst_draw_curve) {
        SetBrushColor(dc, RGB2Pixel(dc, 0x01, 0x1d, 0x2e));
        FillBox(dc, rc.left+2, rc.top+2, RECTW(rc)-4, RECTH(rc)-4);

        start_time = times(NULL);
        total_t = mGEffAnimationGetProperty(animation, MGEFF_PROP_DURATION);
    }

    rc.left += 10;
    rc.right -= 10;
    rc.top += 25;
    rc.bottom -= 25;

    t = 10.0f * (times(NULL) - start_time); // in ms.
    x = rc.left + RECTW(rc) * t / total_t;
    if (x < rc.left) {
        x = rc.left;
    } else if (x > rc.right) {
        x = rc.right;
    }

    if (bfirst_draw_curve) {
        SetPenColor(dc, RGB2Pixel(dc, 0x00, 0xff, 0xff));
        /* X */
        MoveTo (dc, rc.left, rc.bottom);
        LineTo (dc, rc.right, rc.bottom);
        /* Y */
        MoveTo (dc, rc.left, rc.top);
        LineTo (dc, rc.left, rc.bottom);

        bfirst_draw_curve  = 0;
    } else {
        SetPenColor(dc, RGB2Pixel(dc, 0x80, 0x80, 0x80));
        /* H */
        MoveTo (dc, rc.left, lastpoint.y);
        LineTo (dc, lastpoint.x-1, lastpoint.y);

        /* V */
        MoveTo (dc, lastpoint.x, rc.bottom-1);
        LineTo (dc, lastpoint.x, lastpoint.y+1);

        /* Curve */
        SetPenColor(dc, RGB2Pixel(dc, 0xe8, 0xd1, 0x06));
        SetPenWidth (dc, 3);
        LineEx(dc, lastpoint.x, lastpoint.y, x, y);
    }

    SetPenColor(dc, RGB2Pixel(dc, 0xe8, 0xd1, 0x06));
    /* H */
    MoveTo (dc, rc.left, y);
    LineTo (dc, x, y);

    /* V */
    MoveTo (dc, x, rc.bottom);
    LineTo (dc, x, y);

    EndPaint(obj->hwnd, dc);

    lastpoint.x = x;
    lastpoint.y = y;

    return 0;
}

/* block */
static void setPropertyBlock(MGEFF_ANIMATION animation, mWidget *obj, int id, RECT *rc) 
{
    static RECT old_rc;
    HDC dc;
    dc = BeginPaint(obj->hwnd);
    if (bfirst_draw_block) {
        RECT rc;
        GetClientRect(obj->hwnd, &rc);
        rc.left+=2;
        rc.right-=2;
        rc.top+=2;
        rc.bottom-=2;
        SetBrushColor(dc, RGB2Pixel(dc, 0x01, 0x1d, 0x2d));
        FillBox(dc, rc.left, rc.top, RECTW(rc), RECTH(rc));

        bfirst_draw_block = 0;
    }else{
        SetBrushColor(dc, RGB2Pixel(dc, 0x01, 0x1d, 0x2d));
        FillBox(dc, old_rc.left, old_rc.top, RECTW(old_rc), RECTH(old_rc));
    }
    SetBrushColor(dc, RGB2Pixel(dc, 0, 0, 0xff));
    FillBox(dc, rc->left, rc->top, RECTWP(rc), RECTHP(rc));
    EndPaint(obj->hwnd, dc);
    memcpy(&old_rc, rc, sizeof(old_rc));
}

/* circle */
void setProperty2(MGEFF_ANIMATION animation, mWidget *obj, int id, float *value) 
{
#define SLIDERADIUS 8
    static POINT old_slide_circle = {0, 0};
    POINT base_origin = {100, 100};
    POINT slide_ball;
    HDC dc;
    RECT rc;
    int base_radius;

    GetClientRect (obj->hwnd, &rc);
    base_origin.x = (rc.left + rc.right)/2;
    base_origin.y = (rc.top + rc.bottom)/2;
    base_radius = (rc.right - rc.left)/2 - 15;
    
    slide_ball.x = base_origin.x + base_radius * sin (*value);
    slide_ball.y = base_origin.y + base_radius * cos (*value);

    dc = BeginPaint(obj->hwnd);

    if (!bfrist_draw_slideball && ((slide_ball.x != old_slide_circle.x) || (slide_ball.y != slide_ball.x)))
    {
        SetBrushColor(dc, RGB2Pixel(dc, 0x80, 0x80, 0x80));
        FillCircle (dc, old_slide_circle.x, old_slide_circle.y, SLIDERADIUS);
    }
    else
    {
        SetBrushColor(dc, RGB2Pixel(dc, 0x01, 0x1d, 0x2d));
        FillBox(dc, rc.left+2, rc.top+2, RECTW(rc)-4, RECTH(rc)-4);
        bfrist_draw_slideball = 0;
    }

    SetPenColor(dc, RGB2Pixel(dc, 0, 0xff, 0xff));
    Circle (dc, base_origin.x, base_origin.y, base_radius);
    SetBrushColor(dc, RGB2Pixel(dc, 0, 0, 0xff));
    FillCircle (dc, slide_ball.x, slide_ball.y, SLIDERADIUS);
    EndPaint(obj->hwnd, dc);
    memcpy (&old_slide_circle, &slide_ball, sizeof (slide_ball));

}

void  mGEffAnimationSetCurLoopChangedCb(MGEFF_ANIMATION handle,  MGEFF_CURLOOPCHANGED_CB cb);


/* code */
static char code_duration[128];
static char code_curve[128];
static char *codes[] = {
    "MGEFF_ANIMATION animation;\n",
    "RECT start_rc = {50, 60, 50+200, 60+300}, end_rc = {250, 300, 250+250, 300+100};\n",
    "\n",
    "/* create the animation, and set the parameters */\n",
    "animation = mGEffAnimationCreate((void *)button, my_callback, ID_GEOMETRY, MGEFF_RECT);\n",
    "mGEffAnimationSetStartValue(animation, &start_rc);\n",
    "mGEffAnimationSetEndValue(animation, &end_rc);\n",
    code_duration, // "mGEffAnimationSetDuration(animation, %d);\n",
    "\n",
    "/* select a curve */\n",
    code_curve, // "mGEffAnimationSetCurve(animation, duration);\n",
    "\n",
    "/* launch ! */\n",
    "mGEffAnimationAsyncRun(animation);\n",
    "mGEffSchedularRun(MGEFF_MODE_TIMER);\n",
};
static char code_buf[1024];

static const char *gen_code(char *buf, int lines) {
    char *p=buf;
    int i;
    for (i=0; i<lines; i++) {
        p += sprintf(p, "%s", codes[i]);
    }
    return buf;
}

static void setProperty_code(MGEFF_ANIMATION animation, mMlEdit *edit, int id, int *n) {
    _c(edit)->setContent(edit, code_buf, 0, *n);
}


static void setProperty_move(MGEFF_ANIMATION animation, mWidget *widget, int id, RECT *rc) {
    MoveWindow(widget->hwnd, rc->left, rc->top, RECTWP(rc), RECTHP(rc), FALSE);
}

static void DoStart(mWidget *parent);

static BOOL my_ontimer(HWND hwnd, LINT id, DWORD count) {
    mWidget *parent = ncsObjFromHandle(hwnd);
    DoStart(parent);
    KillTimer(hwnd, id);
    return TRUE;
}

static void onGroupFinish(MGEFF_ANIMATION handle) {
    SetTimerEx(g_mainwnd->hwnd, TIMER_START, 100, my_ontimer);
}

/* on Start */
static void DoStart (mWidget *parent) 
{
    int duration = DURATION;
    const char *str_curve;
    MGEFF_ANIMATION animation;
    MGEFF_ANIMATION parallel_group;
    int curve;

    // Set curve
    {
        if (last_curve >= sizeof(s_curves)/sizeof(s_curves[0])) {
            last_curve = 0;
        }
        curve = s_curves[last_curve].type;
        str_curve = s_curves[last_curve].name;
        ++last_curve;
    }

    // Start animations
    {
        mWidget *object1 = (mWidget *) _c(parent)->getChild(parent, ID_RECTANGLE2);
        mWidget *object2 = (mWidget *) _c(parent)->getChild(parent, ID_RECTANGLE3);
        mWidget *object3 = (mWidget *) _c(parent)->getChild(parent, ID_RECT_CURVE);
        RECT rc;
        RECT start_rc;
        RECT end_rc;
        float f;
        int y;

        parallel_group = mGEffAnimationCreateGroup(MGEFF_PARALLEL);

        /* block */
        bfirst_draw_block = 1;
        GetClientRect(object1->hwnd, &rc);
        start_rc.left = (rc.left + rc.right)/2 - 10;
        start_rc.right = (rc.left + rc.right)/2 + 10;
        start_rc.top = rc.bottom - 40;
        start_rc.bottom = rc.bottom - 20;
        end_rc.left = rc.left + 10;
        end_rc.right = rc.right - 10;
        end_rc.top = rc.top + 20;
        end_rc.bottom = rc.top + 40;
        animation = mGEffAnimationCreate(object1, (void *)setPropertyBlock, 0, MGEFF_RECT);
        mGEffAnimationSetStartValue(animation, &start_rc);
        mGEffAnimationSetEndValue(animation, &end_rc);
        mGEffAnimationSetDuration(animation, duration);
        mGEffAnimationSetCurve(animation, curve);
        mGEffAnimationAddToGroup(parallel_group, animation);

        /* ball */
        bfrist_draw_slideball = 1;
        animation = mGEffAnimationCreate(object2, (void *)setProperty2, 0, MGEFF_FLOAT);
        f = 0;
        mGEffAnimationSetStartValue(animation, &f);
        f = 2*M_PI;
        mGEffAnimationSetEndValue(animation, &f);
        mGEffAnimationSetDuration(animation, duration);
        mGEffAnimationSetCurve(animation, curve);
        mGEffAnimationAddToGroup(parallel_group, animation);

        /* curve */
        bfirst_draw_curve = 1;
        animation = mGEffAnimationCreate(object3, (void *)setPropertyCurve, 0, MGEFF_INT);
        GetClientRect(object3->hwnd, &rc);
        y=rc.bottom-25;
        mGEffAnimationSetStartValue(animation, &y);
        y=rc.top+25;
        mGEffAnimationSetEndValue(animation, &y);
        mGEffAnimationSetDuration(animation, duration);
        mGEffAnimationSetCurve(animation, curve);
        mGEffAnimationAddToGroup(parallel_group, animation);

        // code
        {
            mMlEdit *edit;
            int n;
            edit = (mMlEdit *) _c(parent)->getChild(parent, ID_MLEDIT_CODE);
            ncsSetElement(edit, NCS_FGC_WINDOW, MakeRGBA(0xe8, 0xd1, 0x06, 0xff));

            snprintf(code_duration, sizeof(code_duration)/sizeof(code_duration[0]), 
                    "mGEffAnimationSetDuration(animation, %d);\n" , duration);
            code_duration[sizeof(code_duration)/sizeof(code_duration[0]) - 1] = 0;

            snprintf(code_curve, sizeof(code_curve)/sizeof(code_curve[0]), 
                    "mGEffAnimationSetCurve(animation, %s);\n", str_curve);
            code_curve[sizeof(code_curve)/sizeof(code_curve[0]) - 1] = 0;

            animation = mGEffAnimationCreate(edit, (void *)setProperty_code, 0, MGEFF_INT);
            n = 0;
            mGEffAnimationSetStartValue(animation, &n);
            gen_code(code_buf, sizeof(codes)/sizeof(codes[0]));
            n = strlen(code_buf);
            mGEffAnimationSetEndValue(animation, &n);
            mGEffAnimationSetDuration(animation, duration);
            mGEffAnimationSetCurve(animation, curve);
            mGEffAnimationAddToGroup(parallel_group, animation);
        }

        mGEffAnimationSetFinishedCb(parallel_group, onGroupFinish);
        mGEffAnimationAsyncRun(parallel_group);
    }
}

static void setPropertyEllicpe(MGEFF_ANIMATION animation, mWidget *obj, int id, int *val)
{
    RECT rc;
    HDC dc;
    dc = BeginPaint(obj->hwnd);
    int ry, rx, sx, sy;
    static int last_rx = 0;

    GetClientRect (obj->hwnd, &rc);
    sx = (rc.left + rc.right)/2;
    sy = (rc.top + rc.bottom)/2;
    ry = (rc.right - rc.left)/2 - 15;
    rx = *val;

    SetPenColor(dc, RGB2Pixel(dc, 0x01, 0x1d, 0x2e));
    Ellipse (dc, sx, sy, last_rx, ry);

    SetPenColor(dc, RGB2Pixel(dc, 0x08, 0xff, 0xff));
    Ellipse (dc, sx, sy, rx, ry);
    EndPaint (obj->hwnd, dc);

    last_rx = rx;
}

static void setProperty_move_axes (MGEFF_ANIMATION animation, mWidget *obj, int id,  POINT *p) 
{
    RECT rc;
    HDC dc;

    GetClientRect (obj->hwnd, &rc);
    rc.left += 10;
    rc.right -= 10;
    rc.top += 25;
    rc.bottom -= 25;

    dc = BeginPaint(obj->hwnd);
    SetPenColor (dc, RGB2Pixel(dc,0x0, 0xff, 0xff));
    LineEx (dc, rc.left, rc.bottom, rc.left, p->y);
    LineEx (dc, rc.left, rc.bottom, p->x, rc.bottom);

    EndPaint (obj->hwnd, dc);
}

static void the_starting_animation (mWidget* parent)
{
    int duration = 2*DURATION;
    MGEFF_ANIMATION animation;
    MGEFF_ANIMATION parallel_group;
    RECT rc;
    int val; 
    int x=0, y=0;

    {
       mWidget *object1 = (mWidget *) _c(parent)->getChild(parent, ID_RECTANGLE2);
        mWidget *object2 = (mWidget *) _c(parent)->getChild(parent, ID_RECTANGLE3);
        mWidget *object3 = (mWidget *) _c(parent)->getChild(parent, ID_RECT_CURVE);
        mWidget *object4 = (mWidget *) _c(parent)->getChild(parent, ID_MLEDIT_CODE);

        ClientToWindow(parent->hwnd, &x, &y);
        rc.left += x; rc.right += x;
        rc.top += y; rc.bottom += y;

        parallel_group = mGEffAnimationCreateGroup(MGEFF_PARALLEL);

        /*ball circal*/
        GetClientRect(object2->hwnd, &rc);
        animation = mGEffAnimationCreate(object2, (void *)setPropertyEllicpe, 0, MGEFF_INT);
        val = 0;
        mGEffAnimationSetStartValue(animation, &val);
        val = (rc.right - rc.left)/2 - 15;
        mGEffAnimationSetEndValue(animation, &val);
        mGEffAnimationSetDuration(animation, duration);
        mGEffAnimationAddToGroup(parallel_group, animation);

        int i;
        for (i = 0; i < 2; i++)
        {
            RECT start_rc;
            animation = mGEffAnimationCreate((i==0)?object1:object4, (void *)setProperty_move, 0, MGEFF_RECT);
            GetWindowRect((i==0)?object1->hwnd:object4->hwnd, &rc);
            start_rc.left = (rc.left + rc.right)/2 -2;
            start_rc.right = (rc.left + rc.right)/2 +2;
            start_rc.top = (rc.top + rc.bottom)/2-2;
            start_rc.bottom = (rc.top + rc.bottom)/2-2;
            mGEffAnimationSetStartValue(animation, &start_rc);
            mGEffAnimationSetEndValue(animation, &rc);
            MoveWindow((i==0)?object1->hwnd:object4->hwnd, start_rc.left, start_rc.top, RECTW(start_rc), RECTH(start_rc), FALSE);
            mGEffAnimationSetDuration(animation, duration);
            mGEffAnimationAddToGroup(parallel_group, animation);
        }

        /*curve*/
        POINT point;
        animation = mGEffAnimationCreate(object3, (void *)setProperty_move_axes, 0, MGEFF_POINT);
        GetClientRect(object3->hwnd, &rc);
        point.x = rc.left + 10;
        point.y = rc.bottom - 25;
        mGEffAnimationSetStartValue(animation, &point);
        point.x = rc.right - 10;
        point.y = rc.top + 25;
        mGEffAnimationSetEndValue(animation, &point);
        mGEffAnimationSetDuration(animation, duration);
        mGEffAnimationAddToGroup(parallel_group, animation);

        mGEffAnimationSetFinishedCb(parallel_group, onGroupFinish);
        mGEffAnimationAsyncRun(parallel_group);
    }
}

//$func @733648896 onCreate -- Need by merge, don't modify
static BOOL Mainwnd1_onCreate (mWidget* self, DWORD dwAddData) 
{
    mGEffInit();

    g_mainwnd = self;

    the_starting_animation(self);
	return TRUE;
}

//$func @733648896 onDestroy
static void Mainwnd1_onDestroy (mWidget* self, int message) 
{
    mGEffDeinit();
}

//$func @733648896 onLButtonUp
static BOOL Mainwnd1_onLButtonUp (mWidget* self, int message, int x, int y, DWORD key_status) 
{

	//TODO:
	return FALSE; /*continue default message process*/
}

//$func @733648896 onLButtonDown
static BOOL Mainwnd1_onLButtonDown (mWidget* self, int message, int x, int y, DWORD key_status) 
{

	//TODO:
	return FALSE; /*continue default message process*/
}

//$func @733648896 onClose
static BOOL Mainwnd1_onClose (mWidget* self, int message) 
{
    DestroyMainWindow (self->hwnd);
    PostQuitMessage (self->hwnd);
	//TODO:
	return FALSE; /*continue default message process*/
}

//$handle @733648896 -- Need by merge, don't modify
static NCS_EVENT_HANDLER Mainwnd1_handlers [] = {
	{MSG_CREATE, Mainwnd1_onCreate},
	{MSG_CLOSE, Mainwnd1_onClose},
	{MSG_DESTROY, Mainwnd1_onDestroy},
	{MSG_LBUTTONDOWN, Mainwnd1_onLButtonDown},
	{MSG_LBUTTONUP, Mainwnd1_onLButtonUp},
//$user -- TODO add your handlers hear
	{-1,NULL}
};

//$func @4286294016 onPaint -- Need by merge, don't modify
static void Rectangle3_onPaint (mWidget* self, HDC hdc, const PCLIPRGN clip_rgn) 
{

	//TODO:
	
}

//$handle @4286294016 -- Need by merge, don't modify
static NCS_EVENT_HANDLER Rectangle3_handlers [] = {
	{MSG_PAINT, Rectangle3_onPaint},
//$user -- TODO add your handlers hear
	{-1,NULL}
};

//$func @3726995456 onPaint -- Need by merge, don't modify
static void RectCurve_onPaint (mWidget* self, HDC hdc, const PCLIPRGN clip_rgn) 
{

	//TODO:
	
}

//$handle @3726995456 -- Need by merge, don't modify
static NCS_EVENT_HANDLER RectCurve_handlers [] = {
	{MSG_PAINT, RectCurve_onPaint},
//$user -- TODO add your handlers hear
	{-1,NULL}
};

//$mainhandle -- Need by merge, don't modify
NCS_EVENT_HANDLER_INFO mainwnd_Mainwnd1_handlers [] = {
	{ID_RECT_CURVE, RectCurve_handlers},
	{ID_MAINWND1, Mainwnd1_handlers},
	{ID_RECTANGLE3, Rectangle3_handlers},
//$user --TODO: Add your handlers here
	{-1, NULL}
};

//$mainwnd_entry -- don't modify
NCS_WND_EXPORT mMainWnd* ntCreateMainwnd1Ex(HPACKAGE package, HWND hParent, HICON h_icon, HMENU h_menu, DWORD user_data)
{
	// please don't change me
	return ncsCreateMainWindowIndirectFromID(package,
		ID_MAINWND1, /* window id */
		hParent,
		h_icon,
		h_menu,
		mainwnd_Mainwnd1_handlers,
		NULL,
		user_data);
}
