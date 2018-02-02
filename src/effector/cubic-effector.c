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
#include "3DCompute.h"
#include "effector.h"
#include "eff-common.h"
#include "cubic-effector.h"
#include "common-effector.h"

const int ROTATE_MAX = 90;

MGEFF_EFFECTOR effcubiceffector_init (MGEFF_EFFECTOR _effector)
{
    EffEffector *effector = (EffEffector *)_effector;
    EffCubicCtxt *cubic_context =
        (EffCubicCtxt *)calloc (1, sizeof(EffCubicCtxt));
    cubic_context->mode = FAST_MODE;
    effector->context = cubic_context;
    return _effector;
}

MGEFF_EFFECTOR effcubiceffector_mgplus_init (MGEFF_EFFECTOR _effector)
{
    EffEffector *effector = (EffEffector *)_effector;
    EffCubicCtxt *cubic_context =
        (EffCubicCtxt *)calloc (1, sizeof(EffCubicCtxt));
    cubic_context->mode = MGPLUS_MODE;
    effector->context = cubic_context;
    return _effector;
}

void effcubiceffector_finalize (MGEFF_EFFECTOR _effector)
{
    EffEffector *effector = (EffEffector *)_effector;
    free (effector->context);
}

void effcubiceffector_begindraw(MGEFF_ANIMATION animation, MGEFF_EFFECTOR _effector)
{
    float s = 0.0f, e = 1.0f;
    EffEffector *effector = (EffEffector *)_effector;
    EffCubicCtxt *cubic_context = (EffCubicCtxt *)effector->context;

    if (cubic_context->mode == MGPLUS_MODE) {
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
        cubic_context->graphics = MGPlusGraphicCreate (RECTW(rc_sink), RECTH(rc_sink));
        BitBlt (effector->sink->hdc, rc_sink.left, rc_sink.top, RECTW(rc_sink), RECTH(rc_sink),
                MGPlusGetGraphicDC (cubic_context->graphics), 0, 0, 0);
        /* Bitmap must be compatible with graphic dc.*/
        {
            HDC hdc = CreateCompatibleDC(MGPlusGetGraphicDC(cubic_context->graphics));

            BitBlt(source1->hdc, 0, 0, 0, 0, hdc, 0, 0, 0);
            GetBitmapFromDC (hdc, rc_src1.left, rc_src1.top, 
                    RECTW(rc_src1), RECTH(rc_src1), &src1_bitmap);

            BitBlt(source2->hdc, 0, 0, 0, 0, hdc, 0, 0, 0);
            GetBitmapFromDC (hdc, rc_src2.left, rc_src2.top,
                    RECTW(rc_src2), RECTH(rc_src2), &src2_bitmap);
            MGPlusGraphicLoadBitmap (cubic_context->graphics, 0, &src1_bitmap);
            MGPlusGraphicLoadBitmap (cubic_context->graphics, 1, &src2_bitmap);
            DeleteMemDC(hdc);
        }
    }

    mGEffAnimationSetStartValue(animation, &s);
    mGEffAnimationSetEndValue(animation, &e);
}

void effcubiceffector_enddraw(MGEFF_ANIMATION animation, MGEFF_EFFECTOR _effector)
{
    EffEffector *effector = (EffEffector *)_effector;
    EffCubicCtxt *cubic_context = (EffCubicCtxt *)effector->context;
    if (cubic_context->mode == MGPLUS_MODE) {
        MGPlusGraphicUnLoadBitmap(cubic_context->graphics, 0);
        MGPlusGraphicUnLoadBitmap(cubic_context->graphics, 1);
        MGPlusGraphicDelete(cubic_context->graphics);
    }
}

void effcubiceffector_ondraw(MGEFF_ANIMATION animation, MGEFF_EFFECTOR _effector, 
        HDC sink_dc, intptr_t id, void* value)
{
    EffEffector *effector = (EffEffector *)_effector;
    int nZprep, w, h, j, index;
    int m_direction;
    double dAngle;
    HDC src_dc;
    RECT src_rc;
    RECT sink_rc;
    EffCubicCtxt *cubic_context = (EffCubicCtxt *)effector->context;
    EffHDCSource *source1;
    EffHDCSource *source2;
    EffHDCSource* m_sourceList[2];
    TransAffine3D* pTransAffine = NULL;
    Vector3D Points3D[4];
    Vector2D Points2D[4];

    _MGEFF_PRINTF("%s:value = %f\n", __FUNCTION__, *(float*)value);
    if (sink_dc != HDC_INVALID) {
        dAngle = ROTATE_MAX * (*(float*)value);

        if (dAngle > ROTATE_MAX) dAngle = ROTATE_MAX;

        effbaseeffector_rect(effector->sink->hdc, &sink_rc);

        w = RECTW(sink_rc);
        h = RECTH(sink_rc);
        pTransAffine = Init3DSenceTransAffine ();

        if (!pTransAffine) {
            printf ("[3D Sence]Init: can not malloc enough memory!");
            return;
        }

        if (effector->direction <= MGEFF_DIRECTION_RIGHT2LEFT) {
            EffPointF LeftPoints[4];
            EffPointF RightPoints[4];

            nZprep = 3 * w;
            m_direction = (effector->direction == MGEFF_DIRECTION_LEFT2RIGHT) ? 0 : 1;

            Points3D[0].x = 0;
            Points3D[0].y = h;
            Points3D[0].z = -w;

            Points3D[1].x = w;
            Points3D[1].y = h;
            Points3D[1].z = -w;

            Points3D[2].x = w;
            Points3D[2].y = h;
            Points3D[2].z = 0;

            Points3D[3].x = 0;
            Points3D[3].y = h;
            Points3D[3].z = 0;

            Set3DPointOfView (pTransAffine, 0, 0, 0);
            Move3DSence (pTransAffine, -w / 2, 0, w / 2);
            RotateHoriz3DSence (pTransAffine, m_direction == 0 ? ROTATE_MAX - dAngle : dAngle );
            Move3DSence (pTransAffine, w / 2, 0, -w / 2);

            for (j = 0; j < 4; j ++) {
                Generate3DPointBy3DSence (pTransAffine, &Points3D [j]);
                Perspective3DTo2D (&Points3D [j], &Points2D [j], nZprep, 0, 0);
            }

            LeftPoints[3].x = Points2D[3].x;
            LeftPoints[3].y = Points2D[3].y;

            LeftPoints[0].x = LeftPoints[3].x;
            LeftPoints[0].y = h - LeftPoints[3].y;

            LeftPoints[2].x = Points2D[2].x;
            LeftPoints[2].y = Points2D[2].y;

            LeftPoints[1].x = LeftPoints[2].x;
            LeftPoints[1].y = h - LeftPoints[2].y;

            RightPoints[2].x = Points2D[1].x;
            RightPoints[2].y = Points2D[1].y;

            RightPoints[1].x = RightPoints[2].x;
            RightPoints[1].y = h - RightPoints[2].y;

            RightPoints[0].x = LeftPoints[1].x;
            RightPoints[0].y = LeftPoints[1].y;

            RightPoints[3].x = LeftPoints[2].x;
            RightPoints[3].y = LeftPoints[2].y;

            if (cubic_context->mode == FAST_MODE) {
                source1 = (EffHDCSource *) (effector->source_list.next);
                source2 = (EffHDCSource *) (source1->list.next);
                m_sourceList[0] = source1;
                m_sourceList[1] = source2;
                SetBrushColor(sink_dc, RGB2Pixel(sink_dc, 0x0, 0x0, 0x0));
                FillBox(sink_dc, 0, 0, RECTW(sink_rc), RECTH(sink_rc));
                if (LeftPoints[0].x < LeftPoints[1].x) {
                    index = m_direction == 0 ? 1 : 0;
                    src_dc = m_sourceList[index]->hdc;
                    effbaseeffector_rect(src_dc, &src_rc);
                    effcommon_v_fillladder(src_dc, src_rc.left, src_rc.top, RECTW(src_rc), RECTH(src_rc),
                            sink_dc, sink_rc.left, sink_rc.top, sink_rc.right, sink_rc.bottom,
                            LeftPoints[1].x, LeftPoints[1].y, 
                            LeftPoints[2].x, LeftPoints[2].y, 
                            LeftPoints[3].x, LeftPoints[3].y, 
                            LeftPoints[0].x, LeftPoints[0].y);
                }
                if (RightPoints[0].x < RightPoints[1].x) {
                    index = m_direction == 0 ? 0 : 1;
                    src_dc = m_sourceList[index]->hdc;
                    effbaseeffector_rect(src_dc, &src_rc);
                    effcommon_v_fillladder(src_dc, src_rc.left, src_rc.top, RECTW(src_rc), RECTH(src_rc),
                            sink_dc, sink_rc.left, sink_rc.top, sink_rc.right, sink_rc.bottom,
                            RightPoints[1].x, RightPoints[1].y, 
                            RightPoints[2].x, RightPoints[2].y, 
                            RightPoints[3].x, RightPoints[3].y, 
                            RightPoints[0].x, RightPoints[0].y);
                }
            }
            else {
                SetBrushColor(MGPlusGetGraphicDC(cubic_context->graphics), RGB2Pixel(sink_dc, 0x0, 0x0, 0x0));
                FillBox(MGPlusGetGraphicDC(cubic_context->graphics), 0, 0, RECTW(sink_rc), RECTH(sink_rc));
                if (LeftPoints[0].x < LeftPoints[1].x)
                    MGPlusDrawImageWithPoints (cubic_context->graphics, m_direction == 0 ? 1 : 0, (MPPOINT*)LeftPoints, 4);
                if (RightPoints[0].x < RightPoints[1].x)
                    MGPlusDrawImageWithPoints (cubic_context->graphics, m_direction == 0 ? 0 : 1, (MPPOINT*)RightPoints, 4);

                MGPlusGraphicSave (cubic_context->graphics, sink_dc, 0, 0, 0, 0, sink_rc.left, sink_rc.top);
            }
            Terminate3DSenceTransAffine (pTransAffine);
        } else {
            EffPointF TopPoints[4];
            EffPointF BottomPoints[4];

            nZprep = 3 * h;
            m_direction = (effector->direction == MGEFF_DIRECTION_BOTTOM2TOP) ? 0 : 1;

            Points3D[0].x = w;
            Points3D[0].y = h;
            Points3D[0].z = -h;

            Points3D[1].x = w;
            Points3D[1].y = 0;
            Points3D[1].z = -h;

            Points3D[2].x = w;
            Points3D[2].y = 0;
            Points3D[2].z = 0;

            Points3D[3].x = w;
            Points3D[3].y = h;
            Points3D[3].z = 0;

            Set3DPointOfView (pTransAffine, 0, 0, 0);
            Move3DSence (pTransAffine, 0, - h / 2, h / 2);
            RotateVertical3DSence (pTransAffine, m_direction == 0 ? ROTATE_MAX - dAngle : dAngle );
            Move3DSence (pTransAffine, 0, h / 2, -h / 2);

            for (j = 0; j < 4; j ++) {
                Generate3DPointBy3DSence (pTransAffine, &Points3D [j]);
                Perspective3DTo2D (&Points3D [j], &Points2D [j], nZprep, 0, 0);
            }

            TopPoints[2].x = Points2D[3].x;
            TopPoints[2].y = Points2D[3].y;

            TopPoints[3].x = w - TopPoints[2].x;
            TopPoints[3].y = TopPoints[2].y;

            TopPoints[1].x = Points2D[2].x;
            TopPoints[1].y = Points2D[2].y;

            TopPoints[0].x = w - TopPoints[1].x;
            TopPoints[0].y = TopPoints[1].y;

            BottomPoints[2].x = Points2D[2].x;
            BottomPoints[2].y = Points2D[2].y;

            BottomPoints[3].x = w - BottomPoints[2].x;
            BottomPoints[3].y = BottomPoints[2].y;

            BottomPoints[1].x = Points2D[1].x;
            BottomPoints[1].y = Points2D[1].y;

            BottomPoints[0].x = w - BottomPoints[1].x;
            BottomPoints[0].y = BottomPoints[1].y;

            if (cubic_context->mode == FAST_MODE) {
                source1 = (EffHDCSource *) (effector->source_list.next);
                source2 = (EffHDCSource *) (source1->list.next);
                m_sourceList[0] = source1;
                m_sourceList[1] = source2;
                SetBrushColor(sink_dc, RGB2Pixel(sink_dc, 0x0, 0x0, 0x0));
                FillBox(sink_dc, 0, 0, RECTW(sink_rc), RECTH(sink_rc));
                if (TopPoints[3].y > TopPoints[0].y) {
                    index = m_direction == 0 ? 1 : 0;
                    src_dc = m_sourceList[index]->hdc;
                    effbaseeffector_rect(src_dc, &src_rc);
                    effcommon_h_fillladder(src_dc, src_rc.left, src_rc.top, RECTW(src_rc), RECTH(src_rc),
                            sink_dc, sink_rc.left, sink_rc.top, sink_rc.right, sink_rc.bottom,
                            TopPoints[1].x, TopPoints[1].y, 
                            TopPoints[2].x, TopPoints[2].y,
                            TopPoints[3].x, TopPoints[3].y, 
                            TopPoints[0].x, TopPoints[0].y); 
                }
                else {
                //    printf("out of line\n");
                }
                if (BottomPoints[3].y > BottomPoints[0].y) {
                    index = m_direction == 0 ? 0 : 1;
                    src_dc = m_sourceList[index]->hdc;
                    effbaseeffector_rect(src_dc, &src_rc);
                    effcommon_h_fillladder(src_dc, src_rc.left, src_rc.top, RECTW(src_rc), RECTH(src_rc),
                            sink_dc, sink_rc.left, sink_rc.top, sink_rc.right, sink_rc.bottom,
                            BottomPoints[1].x, BottomPoints[1].y, 
                            BottomPoints[2].x, BottomPoints[2].y,
                            BottomPoints[3].x, BottomPoints[3].y, 
                            BottomPoints[0].x, BottomPoints[0].y); 
                }
                else {
                //    printf("out of line\n");
                }
            }
            else {
                SetBrushColor(MGPlusGetGraphicDC(cubic_context->graphics), RGB2Pixel(sink_dc, 0x0, 0x0, 0x0));
                FillBox(MGPlusGetGraphicDC(cubic_context->graphics), 0, 0, RECTW(sink_rc), RECTH(sink_rc));
                if (TopPoints[3].y > TopPoints[0].y)
                    MGPlusDrawImageWithPoints (cubic_context->graphics, m_direction == 0 ? 1 : 0, (MPPOINT*)TopPoints, 4);
                if (BottomPoints[3].y > BottomPoints[0].y)
                    MGPlusDrawImageWithPoints (cubic_context->graphics, m_direction == 0 ? 0 : 1, (MPPOINT*)BottomPoints, 4);

                MGPlusGraphicSave (cubic_context->graphics, sink_dc, 0, 0, 0, 0, sink_rc.left, sink_rc.top);
            }
            Terminate3DSenceTransAffine (pTransAffine);
        }
    }
}

int effcubliceffector_setproperty(MGEFF_EFFECTOR _effector, int property_id, int value)
{
    EffEffector *effector = (EffEffector *)_effector;
    switch (property_id)
    {
        case MGEFF_PROPERTY_DIRECTION:
            if (value >= MGEFF_DIRECTION_LEFT2RIGHT && 
                    value <= MGEFF_DIRECTION_BOTTOM2TOP)
            {
                effector->direction = value;
                return 0;
            }
            break;
        default:
            return effbaseeffector_setproperty(_effector, property_id, value);
    }
    return -1;
}

MGEFF_EFFECTOROPS cubiceffector = 
{ 
    MGEFF_MINOR_cubicrotate, 
    MGEFF_FLOAT, 
    effcubiceffector_init, 
    effcubiceffector_finalize,
    effcubiceffector_ondraw,  
    effcubiceffector_begindraw, 
    effcubiceffector_enddraw,
    effcubliceffector_setproperty, 
    NULL 
};


MGEFF_EFFECTOROPS mgpluscubiceffector = 
{ 
    MGEFF_MINOR_mgplus_cubicrotate,
    MGEFF_FLOAT, 
    effcubiceffector_mgplus_init, 
    effcubiceffector_finalize,
    effcubiceffector_ondraw,  
    effcubiceffector_begindraw, 
    effcubiceffector_enddraw,
    effcubliceffector_setproperty, 
    NULL 
};

