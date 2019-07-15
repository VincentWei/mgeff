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
 *   <http://www.minigui.com/en/about/licensing-policy/>.
 */
#include <mgplus/mgplus.h>
#include "mgeff.h"
#include "eff-common.h"
#include "flip-effector.h"
#include "common-effector.h"

#ifdef _MGEFF_FLIPEFFECTOR

#define FLIP_COMPLETE  (1.0f) 

const int FLIP_ANGLE = 180;

MGEFF_EFFECTOR effflipeffector_init(MGEFF_EFFECTOR _effector)
{
    EffEffector *effector = (EffEffector *)_effector;
    EffFlipCtxt* flip_context =
        (EffFlipCtxt*)calloc(1, sizeof(EffFlipCtxt));
    flip_context->mode = FAST_MODE;
    effector->context = flip_context;
    return _effector;
}

MGEFF_EFFECTOR effflipeffector_mgplus_init(MGEFF_EFFECTOR _effector)
{
    EffEffector *effector = (EffEffector *)_effector;
    EffFlipCtxt* flip_context =
        (EffFlipCtxt*)calloc(1, sizeof(EffFlipCtxt));
    flip_context->mode = MGPLUS_MODE;
    effector->context = flip_context;
    return _effector;
}

void effflipeffector_finalize(MGEFF_EFFECTOR _effector)
{
    EffEffector *effector = (EffEffector *)_effector;
    free (effector->context);
}

void effflipeffector_begindraw(MGEFF_ANIMATION animation, MGEFF_EFFECTOR _effector)
{
    float v_s = 0.0f, v_e = 1.0f;
    EffEffector *effector = (EffEffector *)_effector;
    EffFlipCtxt* flip_context = (EffFlipCtxt*)effector->context;

    if (flip_context->mode == MGPLUS_MODE) {
        EffHDCSource *source1 = (EffHDCSource *) (effector->source_list.next);
        EffHDCSource *source2 = (EffHDCSource *) (source1->list.next);

        RECT rc_sink;
        RECT rc_src1;
        RECT rc_src2;
        BITMAP src1_bitmap;
        BITMAP src2_bitmap;

        effbaseeffector_rect(effector->sink->hdc, &rc_sink);
        effbaseeffector_rect(source1->hdc, &rc_src1);
        effbaseeffector_rect(source2->hdc, &rc_src2);

        memset (&src1_bitmap, 0x0, sizeof (src1_bitmap));
        memset (&src2_bitmap, 0x0, sizeof (src2_bitmap));


        flip_context->graphics = MGPlusGraphicCreate (RECTW(rc_sink), RECTH(rc_sink));
        BitBlt (effector->sink->hdc, rc_sink.left, rc_sink.top, RECTW(rc_sink), RECTH(rc_sink),
                MGPlusGetGraphicDC (flip_context->graphics), 0, 0, 0);
        /* Bitmap must be compatible with graphic dc.*/
        {
            HDC hdc = CreateCompatibleDC(MGPlusGetGraphicDC(flip_context->graphics));

            BitBlt(source1->hdc, 0, 0, 0, 0, hdc, 0, 0, 0);
            GetBitmapFromDC (hdc, rc_src1.left, rc_src1.top, 
                    RECTW(rc_src1), RECTH(rc_src1), &src1_bitmap);

            BitBlt(source2->hdc, 0, 0, 0, 0, hdc, 0, 0, 0);
            GetBitmapFromDC (hdc, rc_src2.left, rc_src2.top,
                    RECTW(rc_src2), RECTH(rc_src2), &src2_bitmap);
            MGPlusGraphicLoadBitmap (flip_context->graphics, 0, &src1_bitmap);
            MGPlusGraphicLoadBitmap (flip_context->graphics, 1, &src2_bitmap);
            DeleteMemDC(hdc);
        }
    }
    flip_context->path = MGPlusPathCreate (MP_PATH_FILL_MODE_WINDING);

    mGEffAnimationSetStartValue (animation, &v_s);
    mGEffAnimationSetEndValue (animation, &v_e);
}

void effflipeffector_enddraw(MGEFF_ANIMATION animation, MGEFF_EFFECTOR _effector) 
{
    EffEffector *effector = (EffEffector *)_effector;
    EffFlipCtxt* flip_context = (EffFlipCtxt*)effector->context;
    if (flip_context->mode == MGPLUS_MODE) {
        MGPlusGraphicUnLoadBitmap(flip_context->graphics, 0);
        MGPlusGraphicUnLoadBitmap(flip_context->graphics, 1);
        MGPlusGraphicDelete (flip_context->graphics);
    }
    MGPlusPathDelete (flip_context->path);
}
void effflipeffector_ondraw(MGEFF_ANIMATION animation, MGEFF_EFFECTOR _effector, HDC sink_dc, intptr_t id, void* value)
{
    EffEffector *effector = (EffEffector *)_effector;
    EffFlipCtxt* flip_context = (EffFlipCtxt*)effector->context;

    float curval =  *(float*)value;
    EffHDCSource *source1 = (EffHDCSource *) (effector->source_list.next);
    EffHDCSource *source2 = (EffHDCSource *) (source1->list.next);
    float angle;
    int n_total;

    MPPOINT LeftPoints[4] = {{0,0}};
    MPPOINT RightPoints[4] = {{0,0}};
    MPPOINT center_point;
    MPPOINT* path_point;

    RECT rc_sink;
    RECT rc_src1;
    RECT rc_src2;

    _MGEFF_PRINTF("%s:value = %f\n", __FUNCTION__, *(float*)value);

    effbaseeffector_rect(sink_dc, &rc_sink);
    effbaseeffector_rect(source1->hdc, &rc_src1);
    effbaseeffector_rect(source2->hdc, &rc_src2);

    center_point.x = 0 + ((RECTW(rc_sink)) >> 1); 
    center_point.y = 0; 

    MGPlusPathReset (flip_context->path);
    angle = FLIP_ANGLE * curval;

    if (angle > FLIP_ANGLE) angle = FLIP_ANGLE;

    MGPlusPathRotateAroundPoint (flip_context->path, &center_point, angle);
    MGPlusPathMovetoI (flip_context->path, 0, 0);
    MGPlusPathMovetoI (flip_context->path, RECTW(rc_sink), 0);
    MGPlusPathGetPoints (flip_context->path, &n_total, &path_point);

    if (curval < FLIP_COMPLETE /2) {
        LeftPoints[0].x = path_point[0].x;
        LeftPoints[0].y = path_point[0].y;
        LeftPoints[1].x = path_point[1].x;
        LeftPoints[1].y = path_point[1].y;

        LeftPoints[2].x = path_point[1].x;
        LeftPoints[2].y = path_point[1].y + RECTH(rc_sink);
        LeftPoints[3].x = path_point[0].x;
        LeftPoints[3].y = path_point[0].y + RECTH(rc_sink);
    }
    else {
        RightPoints[0].x = path_point[1].x;
        RightPoints[0].y = path_point[1].y;
        RightPoints[1].x = path_point[0].x;
        RightPoints[1].y = path_point[0].y;

        RightPoints[2].x = path_point[0].x;
        RightPoints[2].y = path_point[0].y + RECTH(rc_sink);
        RightPoints[3].x = path_point[1].x;
        RightPoints[3].y = path_point[1].y + RECTH(rc_sink);
    }
    free (path_point);

    if (flip_context->mode == FAST_MODE) {
        SetBrushColor(sink_dc, RGB2Pixel(sink_dc, 0x0, 0x0, 0x0));
        FillBox(sink_dc, rc_sink.left, rc_sink.top, RECTW(rc_sink), RECTH(rc_sink));
        if (curval < FLIP_COMPLETE /2) {
            if (LeftPoints[0].x < LeftPoints[1].x) {
                const RECT *src_rc = &rc_src1;
                effcommon_v_fillladder(source1->hdc, src_rc->left, src_rc->top, RECTWP(src_rc), RECTHP(src_rc),
                        sink_dc, rc_sink.left, rc_sink.top, rc_sink.right, rc_sink.bottom,
                        LeftPoints[1].x, LeftPoints[1].y,
                        LeftPoints[2].x, LeftPoints[2].y,
                        LeftPoints[3].x, LeftPoints[3].y,
                        LeftPoints[0].x, LeftPoints[0].y);
            }
        }
        else {
            if (RightPoints[0].x < RightPoints[1].x) {
                const RECT *src_rc = &rc_src2;
                effcommon_v_fillladder(source2->hdc, src_rc->left, src_rc->top, RECTWP(src_rc), RECTHP(src_rc),
                        sink_dc, rc_sink.left, rc_sink.top, rc_sink.right, rc_sink.bottom,
                        RightPoints[1].x, RightPoints[1].y,
                        RightPoints[2].x, RightPoints[2].y,
                        RightPoints[3].x, RightPoints[3].y,
                        RightPoints[0].x, RightPoints[0].y);
            }
        }
    }
    else {
        SetBrushColor(MGPlusGetGraphicDC(flip_context->graphics), RGB2Pixel(sink_dc, 0x0, 0x0, 0x0));
        FillBox(MGPlusGetGraphicDC(flip_context->graphics), rc_sink.left, rc_sink.top, RECTW(rc_sink), RECTH(rc_sink));
        if (curval < FLIP_COMPLETE /2)
            MGPlusDrawImageWithPoints (flip_context->graphics, 0, LeftPoints, 4);
        else
            MGPlusDrawImageWithPoints (flip_context->graphics, 1, RightPoints, 4);

        MGPlusGraphicSave (flip_context->graphics, sink_dc, 0, 0, 0, 0, rc_sink.left, rc_sink.top);
    }
}

MGEFF_EFFECTOROPS flipeffector  = 
{ 
    MGEFF_MINOR_flip, 
    MGEFF_FLOAT, 
    effflipeffector_init, 
    effflipeffector_finalize,
    effflipeffector_ondraw,  
    effflipeffector_begindraw, 
    effflipeffector_enddraw,
    NULL,
    NULL
};


MGEFF_EFFECTOROPS mgplusflipeffector  = 
{ 
    MGEFF_MINOR_mgplus_flip, 
    MGEFF_FLOAT, 
    effflipeffector_mgplus_init, 
    effflipeffector_finalize,
    effflipeffector_ondraw,  
    effflipeffector_begindraw, 
    effflipeffector_enddraw,
    NULL,
    NULL
};
#endif
