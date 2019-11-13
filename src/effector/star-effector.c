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

#include <math.h>

#include "mgeff.h"
#include "star-effector.h"
#include "eff-common.h"

#ifdef _MGEFF_FIVEPOINTEDSTAREFFECTOR

#ifndef PI
#define PI 3.1415926
#endif

#define ANG2RAD(ang) ((ang)*(PI)/180)
#define ANG_SIN(ang) (sin(ANG2RAD((ang))))
#define ANG_COS(ang) (cos(ANG2RAD((ang))))

#define DANGLE           15
#define MIN_START_VALUE  25


/* points An is convex polygon vertexs
 * points Bn is concave polygon vertexs 
 */
typedef enum _star_order {
    A1 = 0,
    B1,
    A2,
    B2,
    A3,
    B3,
    A4,
    B4,
    A5,
    B5,
    END_A1,
    STAR_ORDER_MAX
} STAR_ORDER;


MGEFF_EFFECTOR effstareffector_init (MGEFF_EFFECTOR _effector)
{
    EffEffector *effector = (EffEffector *)_effector;
    EffStarCtxt* star_context = (EffStarCtxt*)calloc (1, sizeof(EffStarCtxt));
    star_context->zoom = MGEFF_ZOOMOUT;
    effector->direction = MGEFF_CLOCKWISE;
    effector->context = star_context;
    return _effector;
}

void effstareffector_finalize (MGEFF_EFFECTOR _effector)
{
    EffEffector *effector = (EffEffector *)_effector;
    free (effector->context);
}


void effstareffector_ondraw (MGEFF_ANIMATION animation, MGEFF_EFFECTOR _effector,
        HDC sink_dc, intptr_t id, void *value)
{
    EffEffector *effector = (EffEffector *)_effector;
    EffStarCtxt* star_context = (EffStarCtxt*)effector->context;

    /* source1: source dc
     * source2: target dc
     */
    EffHDCSource* source1;
    EffHDCSource* source2;

    //HDC bufdc = effector->sink->bufdc;
    HPATH path = star_context->path;
    PCLIPRGN pRgn = star_context->pRgn;

    int i, count;
    int tmp_x, tmp_y;
    int ox, oy, r, kr, r2;

    POINT points_i[STAR_ORDER_MAX+1];
    MPPOINT points_f[STAR_ORDER_MAX+1];
    MPPOINT* pts_f = NULL;

    RECT rc1, rc2, rc_sink;

    if (star_context->zoom == MGEFF_ZOOMOUT) {
        source1 = (EffHDCSource *) (effector->source_list.next);
        source2 = (EffHDCSource *) (source1->list.next);
    }
    else {
        source2 = (EffHDCSource *) (effector->source_list.next);
        source1 = (EffHDCSource *) (source2->list.next);
    }

    effbaseeffector_rect (source1->hdc, &rc1);
    effbaseeffector_rect (source2->hdc, &rc2);
    effbaseeffector_rect (sink_dc, &rc_sink);


    ox = RECTW(rc2) / 2;
    oy = RECTH(rc2) / 2;
    r = *(int *)value;
    kr = ANG_SIN(18) * r; 
    r2 = kr / ANG_COS(36);

    /* 1st line points */
    points_f[A1].x = ox;
    points_f[A1].y = oy - r;

    points_f[END_A1].x = points_f[A1].x;
    points_f[END_A1].y = points_f[A1].y;

    /* 2rd line points */
    tmp_x = ANG_COS(18) * r;
    tmp_y = kr;
    points_f[A5].x = ox - tmp_x;
    points_f[A5].y = oy - tmp_y;
    
    points_f[A2].x = ox + tmp_x;
    points_f[A2].y = oy - tmp_y;

    tmp_x = ANG_SIN(36) * r2;
    points_f[B5].x = ox - tmp_x;
    points_f[B5].y = oy - tmp_y;

    points_f[B1].x = ox + tmp_x;
    points_f[B1].y = oy - tmp_y;

    /* 3th line points */
    tmp_x = ANG_SIN(72) * r2;
    tmp_y = ANG_COS(72) * r2;
    points_f[B4].x = ox - tmp_x;
    points_f[B4].y = oy + tmp_y;

    points_f[B2].x = ox + tmp_x;
    points_f[B2].y = oy + tmp_y;

    /* 4th line points */
    tmp_x = ANG_SIN(36) * r;
    tmp_y = ANG_COS(36) * r;
    points_f[A4].x = ox - tmp_x;
    points_f[A4].y = oy + tmp_y;

    points_f[A3].x = ox + tmp_x;
    points_f[A3].y = oy + tmp_y;

    /* 5th line points */
    points_f[B3].x = ox;
    points_f[B3].y = oy + r2;

    /* org */
    points_f[STAR_ORDER_MAX].x = ox;
    points_f[STAR_ORDER_MAX].y = oy;

    MGPlusPathReset (path);
    MGPlusPathAddLines (path, points_f, STAR_ORDER_MAX);

    if (effector->direction == MGEFF_CLOCKWISE) {
        star_context->current_angle += DANGLE;
        star_context->current_angle %= 360;
    }
    else {
        star_context->current_angle -= DANGLE;
        if (star_context->current_angle <= 0)
            star_context->current_angle = 360;
    }

    MGPlusPathRotateAroundPoint (path, &points_f[STAR_ORDER_MAX], star_context->current_angle);
    MGPlusPathTransform (path);

    MGPlusPathGetPoints (path, &count, &pts_f);
    for (i = 0; i < count; i++) {
        if (i >= STAR_ORDER_MAX)
            break;
        points_i[i].x = (int)pts_f[i].x;
        points_i[i].y = (int)pts_f[i].y;
    }

    /* first blt the source dc image */
    BitBlt (source1->hdc, rc1.left, rc1.top, RECTW(rc1), RECTH(rc1), 
            sink_dc, rc_sink.left, rc_sink.top, 0);

    if (star_context->zoom == MGEFF_ZOOMOUT || r != MIN_START_VALUE) {
        /* then calculate the five star clip region.
         * most cost time part, can optimize it? 
         */
        InitPolygonRegion (pRgn, (const POINT*)points_i, STAR_ORDER_MAX);
        SelectClipRegion (sink_dc, pRgn);
        
        /* blt the target dc image with the five star clip region */
        BitBlt (source2->hdc, rc2.left, rc2.top, RECTW(rc2), RECTH(rc2), 
                sink_dc, rc_sink.left, rc_sink.top, 0);

        //BitBlt (bufdc, rc_sink.left, rc_sink.top, RECTW(rc_sink), RECTH(rc_sink), 
        //        sink_dc, rc_sink.left, rc_sink.top, 0);
    }

    if (pts_f != NULL)
        free (pts_f);
}


void effstareffector_begindraw (MGEFF_ANIMATION animation, MGEFF_EFFECTOR _effector)
{
    int r_s, r_e;
    EffEffector *effector = (EffEffector *)_effector;
    EffStarCtxt* star_context = (EffStarCtxt*)effector->context;

    r_s = MIN_START_VALUE;
    r_e = MIN_START_VALUE;

    if (effector->direction == MGEFF_CLOCKWISE)
        star_context->current_angle = 0;
    else
        star_context->current_angle = 360;

    star_context->pRgn = CreateClipRgn (); 

    /* use zero-rule */
    star_context->path = MGPlusPathCreate (MP_PATH_FILL_MODE_WINDING);

    if (effector->sink) {
        int /*kr_e, */r2_e;

        EffHDCSource *source1 = (EffHDCSource *) (effector->source_list.next);
        EffHDCSource *source2 = (EffHDCSource *) (source1->list.next);

        int w = GetGDCapability (source2->hdc, GDCAP_MAXX) + 1;
        int h = GetGDCapability (source2->hdc, GDCAP_MAXY) + 1;

        /* calculate the end value -- the kernal regular pentagons radius.
         * first complete the rectangle to the regular rectangle.
         * then (the regular rectangle radius) * (sqrt(2)) 
         */
        if (star_context->zoom == MGEFF_ZOOMOUT) {
            r2_e = MIN(w/2, h/2) * 1.4;
            //kr_e = r2_e * ANG_COS(36);
            //r_e = kr_e / ANG_COS(72);
            r_e = r2_e / ANG_COS(72);
        }
        else {
            r_s = MIN(w/2, h/2) * 1.4;
            r_e = MIN_START_VALUE;
        }

        /* create sink buffer dc */
        //mGEffSetBufferSink ((MGEFF_SINK)effector->sink, effector->sink->hdc);
    }
    else {
        fprintf(stderr, "%s:effector's sink is not set.\n", __FUNCTION__);
    }

    mGEffAnimationSetStartValue (animation, &r_s);
    mGEffAnimationSetEndValue (animation, &r_e);
}


void effstareffector_enddraw (MGEFF_ANIMATION animation, MGEFF_EFFECTOR _effector)
{
    EffEffector *effector = (EffEffector *)_effector;
    EffStarCtxt* star_context = (EffStarCtxt*)effector->context;

    DestroyClipRgn (star_context->pRgn);
    MGPlusPathDelete (star_context->path);
}


int effstareffector_setproperty (MGEFF_EFFECTOR _effector, int property_id, int value)
{
    EffEffector *effector = (EffEffector *)_effector;
    EffStarCtxt* star_context = (EffStarCtxt*)effector->context;

    switch (property_id)
    {
        case MGEFF_PROPERTY_ZOOM:
            if (value >= MGEFF_ZOOMIN && 
                    value <= MGEFF_ZOOMOUT)
            {
                star_context->zoom = value;
                return 0;
            }
            break;

        case MGEFF_PROPERTY_DIRECTION:
            if (value >= MGEFF_CLOCKWISE && 
                    value <= MGEFF_CLOCKINVERSE)
            {
                effector->direction = value;
                return 0;
            }
            break;

        default:
            return effbaseeffector_setproperty (_effector, property_id, value);
    }
    return -1;
}


int effstareffector_getproperty (MGEFF_EFFECTOR _effector, int property_id, int* value)
{
    EffEffector *effector = (EffEffector *)_effector;
    EffStarCtxt* star_context = (EffStarCtxt*)effector->context;

    switch (property_id)
    {
        case MGEFF_PROPERTY_ZOOM:
            if (value)
                *value = star_context->zoom;
            return star_context->zoom;

        case MGEFF_PROPERTY_DIRECTION:
            if (value)
                *value = effector->direction;
            return effector->direction;

        default:
            return effbaseeffector_getproperty (_effector, property_id, value);
    }

    return -1;
}


MGEFF_EFFECTOROPS stareffector = 
{
    MGEFF_MINOR_fivepointedstar,
    MGEFF_INT,
    effstareffector_init,
    effstareffector_finalize,
    effstareffector_ondraw,
    effstareffector_begindraw,
    effstareffector_enddraw,
    effstareffector_setproperty,
    effstareffector_getproperty
};

#endif
