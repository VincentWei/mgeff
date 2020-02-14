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
#include "alpha-effector.h"
#include "eff-common.h"

#ifdef _MGEFF_ALPHAEFFECTOR

static void effalphaeffector_ondraw(MGEFF_ANIMATION animation, MGEFF_EFFECTOR _effector, 
        HDC sink_dc, intptr_t id, void* value)
{
    EffEffector *effector = (EffEffector *)_effector;

    EffHDCSource *source1 = (EffHDCSource *) (effector->source_list.next);
    EffHDCSource *source2 = (EffHDCSource *) (source1->list.next);
    RECT rc1;
    RECT rc2;
    RECT rc_sink;

    __mgeffbaseeffector_rect(source1->hdc, &rc1);
    __mgeffbaseeffector_rect(source2->hdc, &rc2);
    __mgeffbaseeffector_rect(sink_dc, &rc_sink);

    _DBG_PRINTF("%s:value = %f\n", __FUNCTION__, *(float*)value);

    if (sink_dc != HDC_INVALID) {
        SetMemDCAlpha (source2->hdc, MEMDC_FLAG_SRCALPHA,  (*(float*)value) * 255);
        BitBlt (source1->hdc, rc1.left, rc1.top, RECTW(rc1),
                RECTH(rc1 ), sink_dc, rc_sink .left, rc_sink .top, 0);
        BitBlt (source2->hdc, rc2.left, rc2.top, RECTW(rc2 ),
                RECTH(rc2 ), sink_dc, rc_sink.left, rc_sink.top, 0);
    }
}

MGEFF_EFFECTOROPS alphaeffector = 
{ 
    MGEFF_MINOR_alpha,
    MGEFF_FLOAT, 
    NULL,
    NULL,
    effalphaeffector_ondraw,  
    NULL, 
    NULL,
    NULL
};
#endif
