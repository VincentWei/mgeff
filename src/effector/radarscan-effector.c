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

#include <math.h>
#include <string.h>
#include "mgeff.h"
#include "eff-common.h"
#include "common-effector.h"
#include "effector.h"
#include "radarscan-effector.h"

#ifdef _MGEFF_RADARSCANEFFECTOR

#define pi 3.1415926
#define tana(a) ((a == 90 || a == 270) ? 0 : tan(a / 180.0 *pi))
#define ctana(a) ((a == 90 || a == 270) ? 0 : 1 / tana(a))

static int check_special_point(int current_angle, int mangle)
{
    if (current_angle == mangle) {
        return 1;
    } else if (current_angle == 180-mangle) {
        return 2;
    } else if (current_angle == 180+mangle) {
        return 3;
    } else if (current_angle == 360-mangle) {
        return 4;
    }
    return 0;
}

static POINT init_point(int current_angle, int mangle, POINT* special_points,
        RECT rc)
{
    POINT pt = {0,0};
    double temp;
    int w = RECTW(rc);
    int h = RECTH(rc);
    int vtx = check_special_point(current_angle, mangle);

    if (vtx) {
        pt.x = special_points[vtx -1].x;
        pt.y = special_points[vtx -1].y;
    } else {
        if (current_angle < mangle || current_angle > 360 -mangle) {
            temp = tana((double)current_angle) * w / 2 + 0.5;
            pt.x = rc.left + w;
            pt.y = rc.top + h/2 + (int)temp;
        } else if (current_angle < 180 -mangle) {
            temp = ctana((double)current_angle) * h / 2 + 0.5;
            pt.x = rc.left + w/2 + (int)temp;
            pt.y = rc.top + h;
        } else if (current_angle < mangle + 180) {
            temp = tana((double)current_angle) * w / 2 + 0.5;
            pt.x = rc.left;
            pt.y = rc.top + h/2 - (int)temp;
        } else if (current_angle < 360 -mangle) {
            temp = ctana((double)current_angle) * h / 2 + 0.5;
            pt.x = rc.left + w/2 - (int)temp;
            pt.y = rc.top;
        }
    }
    return pt;
}


static int scan_vertex(int start_angle, int end_angle, int mangle,
        int direction, POINT* special_points, RECT rc, POINT *pts)
{
    int vtx, i;
    int w = RECTW(rc);
    int h = RECTH(rc);

    if (end_angle == start_angle) {
        return 0;
    }
    start_angle = ((start_angle % 360) + 360) % 360;
    end_angle = ((end_angle % 360) + 360) % 360;
    if (start_angle == end_angle) {
        memcpy(pts, special_points, sizeof(POINT) * 4);
        return 4;
    }
    i = 0;
    pts[i].x = rc.left + w/2;
    pts[i++].y = rc.top + h/2;
    pts[i++] = init_point(start_angle, mangle, special_points, rc);
    start_angle = (start_angle +1) % 360;
    if (direction == MGEFF_CLOCKWISE) {
        while (start_angle != end_angle) {
            vtx = check_special_point(start_angle, mangle);
            if (vtx) {
                pts[i].x = special_points[vtx -1].x;
                pts[i].y = special_points[vtx -1].y;
                i++;
            }
            start_angle = (start_angle +1) % 360;
        }
    } else {
        while (start_angle != end_angle) {
            vtx = check_special_point(start_angle, mangle);
            if (vtx) {
                pts[i].x = special_points[vtx -1].x;
                pts[i].y = special_points[vtx -1].y;
                i++;
            }
            start_angle--;
            if (start_angle < 0) {
                start_angle = 359;
            }
        }
    }
    pts[i++] = init_point(start_angle, mangle, special_points, rc);
    return i;
}

MGEFF_EFFECTOR effradarscaneffector_init(MGEFF_EFFECTOR _effector)
{
    EffEffector *effector = (EffEffector *)_effector;

    EffRadarScanCtxt *radar_context = 
        (EffRadarScanCtxt*)calloc(1, sizeof(EffRadarScanCtxt));
    radar_context->start_angle = 0;
    radar_context->direction = MGEFF_CLOCKWISE;
    radar_context->npieces = 1;
    effector->context = radar_context;
    return _effector;
}

void effradarscaneffector_finalize (MGEFF_EFFECTOR _effector)
{
    EffEffector *effector = (EffEffector *)_effector;
    free (effector->context);
}

void effradarscaneffector_ondraw(MGEFF_ANIMATION animation,
        MGEFF_EFFECTOR _effector, HDC sink_dc, intptr_t id, void* value)
{
    EffEffector *effector = (EffEffector *)_effector;
    EffRadarScanCtxt *radar_context = (EffRadarScanCtxt*)effector->context;

    EffHDCSource *source1 = (EffHDCSource *)(effector->source_list.next);
    EffHDCSource *source2 = (EffHDCSource *)(source1->list.next);

    RECT rc;
    CLIPRGN rgn;
    //int delta_angle = *(int *)value;
    //fix angle
    int delta_angle = *(int *)value + 1;
    int vertex = 0;
    int start_angle = 0, end_angle = 0;
    int i;

	effbaseeffector_rect(source1->hdc, &rc);
    for (i = 0; i < radar_context->npieces; i++) {
        start_angle = radar_context->start_angle + i * 360 /radar_context->npieces;
        if (radar_context->direction == MGEFF_CLOCKWISE) {
            end_angle = start_angle + delta_angle;
        } else {
            end_angle = start_angle - delta_angle;
        }

        vertex = scan_vertex(start_angle,
                end_angle, radar_context->middle_angle,
                radar_context->direction, radar_context->special_points,
                rc, radar_context->pts);
        InitClipRgn(&rgn, &radar_context->cliprc_heap);
        InitPolygonRegion(&rgn, radar_context->pts, vertex);
        UnionRegion(&radar_context->cliprgn, &radar_context->cliprgn, &rgn);
        EmptyClipRgn(&rgn);
    }

    if (vertex) {
        BitBlt(source1->hdc, rc.left, rc.top, RECTW(rc), RECTH(rc),
                sink_dc, 0, 0, 0);
        SelectClipRegion(sink_dc, &radar_context->cliprgn);
        BitBlt(source2->hdc, rc.left, rc.top, RECTW(rc), RECTH(rc),
                sink_dc, 0, 0, 0);
    }
}

void effradarscaneffector_begindraw(MGEFF_ANIMATION animation, MGEFF_EFFECTOR _effector)
{
    EffEffector *effector = (EffEffector *)_effector;
    EffRadarScanCtxt *radar_context = (EffRadarScanCtxt*)effector->context;
    EffHDCSource *source1 = (EffHDCSource *)(effector->source_list.next);

    RECT rc;
    int startval = 0;
    int endval;
    effbaseeffector_rect(source1->hdc, &rc);

    InitFreeClipRectList(&radar_context->cliprc_heap, 100);
    InitClipRgn(&radar_context->cliprgn, &radar_context->cliprc_heap);
    radar_context->middle_angle = (int)(atan((double)RECTH(rc) / RECTW(rc)) * 180 / pi + 0.5);
    
    endval = 360 / radar_context->npieces;
    mGEffAnimationSetStartValue(animation, &startval);
    mGEffAnimationSetEndValue(animation, &endval);
    
    radar_context->special_points[0].x = rc.right;
    radar_context->special_points[0].y = rc.bottom;
    radar_context->special_points[1].x = rc.left;
    radar_context->special_points[1].y = rc.bottom;
    radar_context->special_points[2].x = rc.left;
    radar_context->special_points[2].y = rc.top;
    radar_context->special_points[3].x = rc.right;
    radar_context->special_points[3].y = rc.top;
}

void effradarscaneffector_enddraw(MGEFF_ANIMATION animation, MGEFF_EFFECTOR _effector)
{
    EffEffector *effector = (EffEffector *)_effector;
    EffRadarScanCtxt *radar_context = (EffRadarScanCtxt*)effector->context;

    EffHDCSource *source1 = (EffHDCSource *)(effector->source_list.next);
    EffHDCSource *source2 = (EffHDCSource *)(source1->list.next);
    HDC sink_dc = effsink_get(effector->sink);
    RECT rc;
    effbaseeffector_rect(source2->hdc, &rc);
    SelectClipRect(sink_dc, &rc);
    BitBlt(source2->hdc, rc.left, rc.top, RECTW(rc), RECTH(rc),
            sink_dc, 0, 0, 0);
    effsink_release(effector->sink, sink_dc);
    EmptyClipRgn(&radar_context->cliprgn);
    DestroyFreeClipRectList(&radar_context->cliprc_heap);
}

int effradarscaneffector_setproperty(MGEFF_EFFECTOR _effector,
        int property_id, int value)
{
    EffEffector *effector = (EffEffector *)_effector;
    EffRadarScanCtxt *radar_context = (EffRadarScanCtxt*)effector->context;

    switch (property_id)
    {   
        case MGEFF_PROPERTY_DIRECTION:
            radar_context->direction = value;
            return 0;
        case MGEFF_PROPERTY_PIECES:
            if (value <= 0)
                value = 1;
            else if (value > 360)
                value = 360;
            radar_context->npieces = value;
            return 0;
        case MGEFF_PROPERTY_STARTANGLE:
            radar_context->start_angle = value;
            return 0;
        default:
            return effbaseeffector_setproperty(_effector, property_id, value);
    }
    return -1;
}

int effradarscaneffector_getproperty (MGEFF_EFFECTOR _effector,
        int property_id, int *value)
{
    EffEffector *effector = (EffEffector *)_effector;
    EffRadarScanCtxt *radar_context = (EffRadarScanCtxt*)effector->context;
    switch (property_id)
    {   
        case MGEFF_PROPERTY_DIRECTION:
            *value = radar_context->direction;
            return *value;
        case MGEFF_PROPERTY_PIECES:
            *value = radar_context->npieces;
            return *value;
        case MGEFF_PROPERTY_STARTANGLE:
            *value = radar_context->start_angle;
            return *value;
        default:
            return effbaseeffector_getproperty(_effector, property_id, value);
    }
    return -1;
}

MGEFF_EFFECTOROPS radarscaneffector = 
{
    MGEFF_MINOR_radarscan, 
    MGEFF_INT, 
    effradarscaneffector_init, 
    effradarscaneffector_finalize, 
    effradarscaneffector_ondraw, 
    effradarscaneffector_begindraw,
    effradarscaneffector_enddraw,
    effradarscaneffector_setproperty,
    effradarscaneffector_getproperty,
};
#endif
