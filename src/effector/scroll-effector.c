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
#include <mgplus/mgplus.h>
#include "mgeff.h"
#include "3DCompute.h"
#include "effector.h"
#include "eff-common.h"
#include "scroll-effector.h"
#include "common-effector.h"

#ifdef _MGEFF_SCROLLEFFECTOR
#define SCROLL_COMPLETE (1.0f)

MGEFF_EFFECTOR effscrolleffector_init(MGEFF_EFFECTOR _effector)
{
    EffEffector *effector = (EffEffector *)_effector;
    EffScrollPageCtxt* scrollpage_context =
        (EffScrollPageCtxt*)calloc(1, sizeof(EffScrollPageCtxt));
    effector->context = scrollpage_context;
    return _effector;
}

void effscrolleffector_finalize(MGEFF_EFFECTOR _effector)
{
    EffEffector *effector = (EffEffector *)_effector;
    free (effector->context);
}

void effscrolleffector_begindraw(MGEFF_ANIMATION animation, MGEFF_EFFECTOR _effector)
{
    float value_start = 0.0f;
    float value_end = 1.0f;
    EffEffector *effector = (EffEffector *)_effector;
    EffScrollPageCtxt* scrollpage_context = (EffScrollPageCtxt*)effector->context;
    EffHDCSource *source1 = (EffHDCSource *) (effector->source_list.next);
    EffHDCSource *source2 = (EffHDCSource *) (source1->list.next);
    BITMAP src1_bitmap;
    BITMAP src2_bitmap;
    RECT rc_sink;
    RECT rc_src1;
    RECT rc_src2;

	mGEffAnimationSetStartValue (animation, &value_start);
    mGEffAnimationSetEndValue (animation, &value_end);

	effbaseeffector_rect(effector->sink->hdc, &rc_sink);
    effbaseeffector_rect(source1->hdc, &rc_src1);
    effbaseeffector_rect(source2->hdc, &rc_src2);
    memset (&src1_bitmap, 0x0, sizeof (src1_bitmap));
    memset (&src2_bitmap, 0x0, sizeof (src2_bitmap));

    scrollpage_context->graphics = MGPlusGraphicCreate (RECTW(rc_sink), RECTH(rc_sink));
    BitBlt (effector->sink->hdc,rc_sink.left, rc_sink.top, RECTW(rc_sink), RECTH(rc_sink),
            MGPlusGetGraphicDC (scrollpage_context->graphics), 0, 0, 0);

    /* Bitmap must be compatible with graphic dc.*/
    {
        HDC hdc = CreateCompatibleDC(MGPlusGetGraphicDC(scrollpage_context->graphics));

        BitBlt(source1->hdc, 0, 0, 0, 0, hdc, 0, 0, 0);
        GetBitmapFromDC (hdc, rc_src1.left, rc_src1.top, 
                RECTW(rc_src1), RECTH(rc_src1), &src1_bitmap);
        BitBlt(source2->hdc, 0, 0, 0, 0, hdc, 0, 0, 0);
        GetBitmapFromDC (hdc, rc_src2.left, rc_src2.top, 
                RECTW(rc_src2), RECTH(rc_src2), &src2_bitmap);

        MGPlusGraphicLoadBitmap (scrollpage_context->graphics, 0, &src1_bitmap);
        MGPlusGraphicLoadBitmap (scrollpage_context->graphics, 1, &src2_bitmap);
        DeleteMemDC(hdc);
    }

    scrollpage_context->leftpath = MGPlusPathCreate (MP_PATH_FILL_MODE_WINDING);
    scrollpage_context->middlepath = MGPlusPathCreate (MP_PATH_FILL_MODE_WINDING);
    scrollpage_context->pen = MGPlusPenCreate (2, 0x7F6E6E6E);
    scrollpage_context->brush = MGPlusBrushCreate (MP_BRUSH_TYPE_SOLIDCOLOR);
    MGPlusSetSolidBrushColor (scrollpage_context->brush, 0xCC2D415F);
}

void effscrolleffector_enddraw(MGEFF_ANIMATION animation, MGEFF_EFFECTOR _effector)
{
    EffEffector *effector = (EffEffector *)_effector;
    EffScrollPageCtxt* scrollpage_context = (EffScrollPageCtxt*)effector->context;
    MGPlusGraphicUnLoadBitmap(scrollpage_context->graphics, 0);
    MGPlusGraphicUnLoadBitmap(scrollpage_context->graphics, 1);
    MGPlusPathDelete (scrollpage_context->leftpath);
    MGPlusPathDelete (scrollpage_context->middlepath);
    MGPlusPenDelete (scrollpage_context->pen);
    MGPlusBrushDelete (scrollpage_context->brush);
    MGPlusGraphicDelete (scrollpage_context->graphics);
}

void effscrolleffector_ondraw(MGEFF_ANIMATION animation, MGEFF_EFFECTOR _effector, HDC sink_dc, intptr_t id, void* value)
{
    EffEffector *effector = (EffEffector *)_effector;
    EffScrollPageCtxt* scrollpage_context = (EffScrollPageCtxt*)effector->context;

    EffHDCSource *source1 = (EffHDCSource *) (effector->source_list.next);
    EffHDCSource *source2 = (EffHDCSource *) (source1->list.next);
    MPPOINT CurveRight[4]; //draw right bezier curve
	MPPOINT CurveLeft[4]; //draw left bezier curve
    MPPOINT Line[3];
	int i;
	int nMove = 0;
    int x = 0;   
    int y = 0;
    int w, h; 
    
    RECT rc_sink;
    RECT rc_src1;
    RECT rc_src2;
    effbaseeffector_rect(effector->sink->hdc, &rc_sink);
    effbaseeffector_rect(source1->hdc, &rc_src1);
    effbaseeffector_rect(source2->hdc, &rc_src2);

    w = GetGDCapability (MGPlusGetGraphicDC (scrollpage_context->graphics), GDCAP_MAXX) + 1;
    h = GetGDCapability (MGPlusGetGraphicDC (scrollpage_context->graphics), GDCAP_MAXY) + 1;
    nMove = (h * 2) * (*(float*)value);
	for (i = 0; i < 4; i++)
	{
	    CurveRight[i].x = x + w;
		CurveRight[i].y = y + h;
		CurveLeft[i].x = x + w;
		CurveLeft[i].y = y + h;
	}

	for (i = 0; i < 3; i++)
	{
	    Line[i].x = x + w;
		Line[i].y = y + h;
	}

	//if (m_framesCount < m_totalFrames)
    
	if (*((float*)value) < SCROLL_COMPLETE - 0.00001)
	{
		//1st, right curve
		CurveRight[0].y = CurveRight[0].y - nMove;
		CurveRight[1].y = CurveRight[0].y + (y + h - CurveRight[0].y) / 3;
		
		CurveRight[2].x = (x + w) - (y + h - CurveRight[0].y) / 6;
		CurveRight[2].y = CurveRight[1].y;

	    CurveRight[3].x = CurveRight[2].x;
		CurveRight[3].y = CurveRight[2].y;

		//2nd, left curve
		CurveLeft[0].x = CurveRight[3].x;
		CurveLeft[0].y = CurveRight[3].y;

		CurveLeft[1].x = CurveLeft[0].x;
		CurveLeft[1].y = CurveLeft[0].y;

		CurveLeft[2].x = (x + w) - (y + h - CurveRight[0].y) / 3;
		CurveLeft[2].y = CurveRight[1].y;

		CurveLeft[3].x = CurveLeft[2].x;
		CurveLeft[3].y = CurveRight[0].y;

		Line[0].x = CurveLeft[3].x;
		Line[0].y = CurveLeft[3].y;

		Line[1].x = (x + w) - (y + h - CurveRight[0].y) * 2;

		Line[2].x = CurveLeft[0].x;
		Line[2].y = CurveLeft[0].y;

		MGPlusPathMovetoI (scrollpage_context->leftpath, x + w, 0);
		MGPlusPathLineto (scrollpage_context->leftpath, CurveRight[0].x, CurveRight[0].y);
		MGPlusPathAddBezier (scrollpage_context->leftpath, CurveRight[0].x, CurveRight[0].y, 
			CurveRight[1].x, CurveRight[1].y, CurveRight[2].x, CurveRight[2].y, 
			CurveRight[3].x, CurveRight[3].y);

		if (Line[1].x < 0)
		{
			float m;
			float b;

			m = (y + h) - CurveRight[0].y;
			b = (2 * (2 * m - w) * m) / (3 * (2 *m - m / 6));

			if ((y + h - b) >= 0)
            {
                MGPlusPathMoveto (scrollpage_context->leftpath, Line[2].x, Line[2].y);
                MGPlusPathLineto (scrollpage_context->leftpath, x, (y +  h - b));
                MGPlusPathLineto (scrollpage_context->leftpath, x, y);
                MGPlusPathLineto (scrollpage_context->leftpath, x + w, y);
            }
		}
		else
		{
		    MGPlusPathMoveto (scrollpage_context->leftpath, Line[2].x, Line[2].y);
			MGPlusPathLineto (scrollpage_context->leftpath, Line[1].x, Line[1].y);
			MGPlusPathLineto (scrollpage_context->leftpath, x, y + h);
			MGPlusPathLineto (scrollpage_context->leftpath, x, y);
			MGPlusPathLineto (scrollpage_context->leftpath, x + w, y);
		}
		
        MGPlusGraphicClear (scrollpage_context->graphics, 0xFF000000);

		BitBlt (source2->hdc, rc_src2.left, rc_src2.top, RECTW(rc_src2), RECTH(rc_src2),
                MGPlusGetGraphicDC (scrollpage_context->graphics), 0, 0, 0);
        MGPlusDrawImageWithPath (scrollpage_context->graphics, 0, scrollpage_context->leftpath);

		MGPlusDrawPath (scrollpage_context->graphics, scrollpage_context->pen, scrollpage_context->leftpath);
        MGPlusPathReset (scrollpage_context->leftpath);
		MGPlusPathAddBezier (scrollpage_context->leftpath, CurveLeft[0].x, CurveLeft[0].y, 
			CurveLeft[1].x, CurveLeft[1].y, CurveLeft[2].x, CurveLeft[2].y, 
			CurveLeft[3].x, CurveLeft[3].y);

		MGPlusPathAddBezier (scrollpage_context->middlepath, CurveLeft[0].x, CurveLeft[0].y, 
			CurveLeft[1].x, CurveLeft[1].y, CurveLeft[2].x, CurveLeft[2].y, 
			CurveLeft[3].x, CurveLeft[3].y);


		if (Line[1].x < 0) {
			float m;
			float a, b;

			m = (y + h) - CurveRight[0].y;
			a = ((2 * m - w) * m) / (2 * m - m / 3);
			b = (2 * (2 * m - w) * m) / (3 * (2 *m - m / 6));
			

            MGPlusPathMoveto (scrollpage_context->leftpath, Line[0].x, Line[0].y);
			MGPlusPathLineto (scrollpage_context->leftpath, x, (y + h - a));

		    MGPlusPathMoveto (scrollpage_context->middlepath, Line[0].x, Line[0].y);
			MGPlusPathLineto (scrollpage_context->middlepath, x, (y + h - a));
			MGPlusPathLineto (scrollpage_context->middlepath, x, (y + h - b));
			MGPlusPathLineto (scrollpage_context->middlepath, Line[2].x, Line[2].y);
		}
		else {
			MGPlusPathLineto (scrollpage_context->leftpath, Line[1].x, Line[1].y);
		    MGPlusPathMoveto (scrollpage_context->middlepath, Line[0].x, Line[0].y);
			MGPlusPathLineto (scrollpage_context->middlepath, Line[1].x, Line[1].y);
			MGPlusPathLineto (scrollpage_context->middlepath, Line[2].x, Line[2].y);
		}

		MGPlusDrawPath (scrollpage_context->graphics, scrollpage_context->pen, scrollpage_context->leftpath);
		MGPlusFillPath (scrollpage_context->graphics, scrollpage_context->brush, scrollpage_context->middlepath);

        MGPlusPathReset (scrollpage_context->leftpath);
		MGPlusPathReset (scrollpage_context->middlepath);
	    MGPlusGraphicSave (scrollpage_context->graphics, sink_dc, 0, 0, 0, 0, rc_sink.left, rc_sink.top);
	}
    else {
        BitBlt (source2->hdc, rc_src2.left, rc_src2.top, RECTW(rc_src2), RECTH(rc_src2), sink_dc, rc_sink.left, rc_sink.top, 0);
    }

    //MGPlusGraphicClear (scrollpage_context->graphics, 0xFF000000);
	//BitBlt (source2->hdc(), rectOfSource2.left, rectOfSource2.top, RECTW(rectOfSource2), RECTH(rectOfSource2),
    //MGPlusGetGraphicDC (scrollpage_context->graphics), 0, 0, 0);
	//MGPlusGraphicSave (scrollpage_context->graphics, m_sink->hdc(), 0, 0, 0, 0, rectdest.left, rectdest.top);
}

MGEFF_EFFECTOROPS scrolleffector  = 
{ 
    MGEFF_MINOR_scroll, 
    MGEFF_FLOAT, 
    effscrolleffector_init,
    effscrolleffector_finalize,
    effscrolleffector_ondraw, 
    effscrolleffector_begindraw, 
    effscrolleffector_enddraw,
    NULL,
    NULL
};
#endif
