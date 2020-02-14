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
#include "mgeff.h"
#include "zoom-effector.h"
#include "eff-common.h"

#ifdef _MGEFF_ZOOMEFFECTOR
static MGEFF_EFFECTOR effzoomeffector_init(MGEFF_EFFECTOR _effector)
{
    EffEffector *effector = (EffEffector *)_effector;
    EffZoomCtxt* zoom_context = (EffZoomCtxt*)calloc(1, sizeof(EffZoomCtxt));
    effector->context = zoom_context;
    return _effector;
}

static void effzoomeffector_finalize (MGEFF_EFFECTOR _effector)
{
    EffEffector *effector = (EffEffector *)_effector;
    free (effector->context);
}

static void effzoomeffector_ondraw (MGEFF_ANIMATION animation, MGEFF_EFFECTOR _effector,
        HDC sink_dc, intptr_t id, void *value)
{
    EffEffector *effector = (EffEffector *)_effector;
    EffZoomCtxt* zoom_context = (EffZoomCtxt*)effector->context;
    EffHDCSource *source1 = (EffHDCSource *) (effector->source_list.next);
    EffHDCSource *source2 = (EffHDCSource *) (source1->list.next);
    RECT rc1, rc2, rc_sink;
    RECT cur_rc;
    RECT prev_rc;

    /* swap */
    if (zoom_context->zoom == MGEFF_ZOOMOUT) {
        EffHDCSource *tmpsource = source1;
        source1= source2;
        source2= tmpsource;
    }
    
    cur_rc = *(RECT *)value;
    prev_rc = zoom_context->prev_rc;

    __mgeffbaseeffector_rect (source1->hdc, &rc1);
    __mgeffbaseeffector_rect (source2->hdc, &rc2);
    __mgeffbaseeffector_rect (sink_dc, &rc_sink);

#if 0
    BitBlt (source1->hdc, rc1.left, rc1.top, RECTW(rc1), RECTH(rc1),
            sink_dc, rc_sink.left, rc_sink.top, 0);
#else
    BitBlt(source1->hdc, prev_rc.left, prev_rc.top, RECTW(prev_rc), RECTH(prev_rc),
            sink_dc, prev_rc.left, prev_rc.top, 0);
#endif

    if (GetGDCapability(source2->hdc, GDCAP_BPP) != 
            GetGDCapability(sink_dc, GDCAP_BPP)) {
        HDC memdc = CreateCompatibleDCEx(source2->hdc, RECTW(rc2), RECTH(rc2));
        assert(GetGDCapability(source2->hdc, GDCAP_BPP) != 32);

        STRETCHBLT(source2->hdc, rc2.left, rc2.top, RECTW(rc2), RECTH(rc2),
                memdc, 0, 0, RECTW(cur_rc), RECTH(cur_rc),
                0);
        BitBlt(memdc, rc2.left, rc2.top, RECTW(rc2), RECTH(rc2),
                sink_dc, cur_rc.left, cur_rc.top, 0);
        DeleteMemDC(memdc);

    }
    else {
        STRETCHBLT(source2->hdc, rc2.left, rc2.top, RECTW(rc2), RECTH(rc2),
                sink_dc, cur_rc.left, cur_rc.top, RECTW(cur_rc), RECTH(cur_rc),
                0);
    }

    zoom_context->prev_rc = cur_rc;
}

static void effzoomeffector_begindraw(MGEFF_ANIMATION animation, MGEFF_EFFECTOR _effector)
{
    EffEffector *effector = (EffEffector *)_effector;
    EffZoomCtxt* zoom_context = (EffZoomCtxt*)effector->context;
    RECT rc_s = {0, 0, 0, 0};
    RECT rc_e = {0, 0, 0, 0};
    if (effector->sink) {
        int w = GetGDCapability(effector->sink->hdc, GDCAP_MAXX) + 1;
        int h = GetGDCapability(effector->sink->hdc, GDCAP_MAXY) + 1;

        rc_e.right = w;
        rc_e.bottom = h;

        if (effector->direction == MGEFF_DIRECTION_CENTER) {
            rc_s.left  = w/2;
            rc_s.right = w/2;
            rc_s.top   = h/2;
            rc_s.bottom = h/2;
        }
        else if (effector->direction == MGEFF_DIRECTION_LDOWN2RUP) {
            rc_s.top    = h;
            rc_s.bottom = h;
        }
        else if (effector->direction == MGEFF_DIRECTION_RDOWN2LUP) {
            rc_s.left  = w;
            rc_s.right = w;
            rc_s.top   = h;
            rc_s.bottom = h;
        }
        else if (effector->direction == MGEFF_DIRECTION_RUP2LDOWN) {
            rc_s.left  = w;
            rc_s.right = w;
        }
        else if (effector->direction == MGEFF_DIRECTION_LUP2RDOWN) {
        }

        if (zoom_context->zoom == MGEFF_ZOOMOUT) {
            RECT tmp_rc = rc_s;
            rc_s = rc_e;
            rc_e = tmp_rc;
        }
    }
    else {
        fprintf(stderr, "%s:effector's sink is not set.\n", __FUNCTION__);
    }
    mGEffAnimationSetStartValue (animation, &rc_s);
    mGEffAnimationSetEndValue (animation, &rc_e);
}

static int effzoomeffector_setproperty(MGEFF_EFFECTOR _effector, int property_id, int value)
{
    EffEffector *effector = (EffEffector *)_effector;
    EffZoomCtxt* zoom_context = (EffZoomCtxt*)effector->context;
    switch (property_id)
    {
        case MGEFF_PROPERTY_ZOOM:
            if (value >= MGEFF_ZOOMIN && 
                    value <= MGEFF_ZOOMOUT)
            {
                zoom_context->zoom = value;
                return 0;
            }
            break;
        default:
            return __mgeffbaseeffector_setproperty(_effector, property_id, value);
    }
    return -1;
}

static int effzoomeffector_getproperty(MGEFF_EFFECTOR _effector, int property_id, int* value)
{
    EffEffector *effector = (EffEffector *)_effector;
    EffZoomCtxt* zoom_context = (EffZoomCtxt*)effector->context;
    switch (property_id)
    {
        case MGEFF_PROPERTY_ZOOM:
        {
            if (value)
                *value = zoom_context->zoom;
            return zoom_context->zoom;
        }

        default:
            return __mgeffbaseeffector_getproperty(_effector, property_id, value);
    }
    return -1;
}

MGEFF_EFFECTOROPS __mgeff_zoom_effector = 
{
    MGEFF_MINOR_zoom, 
    MGEFF_RECT,
    effzoomeffector_init,
    effzoomeffector_finalize,
    effzoomeffector_ondraw,
    effzoomeffector_begindraw,
    NULL,
    effzoomeffector_setproperty,
    effzoomeffector_getproperty

};
#endif
