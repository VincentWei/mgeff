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
#include "mgeff.h"
#include "zip-effector.h"
#include "eff-common.h"

#ifdef  _MGEFF_ZIPEFFECTOR
void effzipeffector_ondraw (MGEFF_ANIMATION animation, MGEFF_EFFECTOR _effector,
           HDC sink_dc, intptr_t id, void *value)
{
    EffEffector *effector = (EffEffector *)_effector;
    EffHDCSource *source1 = (EffHDCSource *) (effector->source_list.next);
    EffHDCSource *source2 = (EffHDCSource *) (source1->list.next);
    RECT rc1, rc2, rc_sink;
    float reach = *(float *)value;
    int startx = 0, starty = 0;
    int x, y, w, h;
    int cur_w, cur_h;

    effbaseeffector_rect (source1->hdc, &rc1);
    effbaseeffector_rect (source2->hdc, &rc2);
    effbaseeffector_rect (sink_dc, &rc_sink);
    x = rc_sink.left;
    y = rc_sink.top;
    w = RECTW(rc_sink);
    h = RECTH(rc_sink);
    cur_w = w;
    cur_h = h;

    switch (effector->direction) {
        case MGEFF_DIRECTION_LEFT2RIGHT:
            cur_w = w * reach;
            startx = 0;
            StretchBlt (source2->hdc, rc2.left, rc2.top, RECTW(rc2), RECTH(rc2),
                    sink_dc, x + startx, y + starty, cur_w, cur_h,
                    0);
            startx = cur_w;
            cur_w = w - cur_w;
            break;
        case MGEFF_DIRECTION_RIGHT2LEFT:
            cur_w = w * reach;
            startx = w - cur_w;
            StretchBlt (source2->hdc, rc2.left, rc2.top, RECTW(rc2), RECTH(rc2),
                    sink_dc, x + startx, y + starty, cur_w, cur_h,
                    0);
            startx = 0;
            cur_w = w - cur_w;
            break;
        case MGEFF_DIRECTION_TOP2BOTTOM:
            cur_h = h * reach;
            starty = 0;
            StretchBlt (source2->hdc, rc2.left, rc2.top, RECTW(rc2), RECTH(rc2),
                    sink_dc, x + startx, y + starty, cur_w, cur_h,
                    0);
            starty = cur_h;
            cur_h = h - cur_h;
            break;
        case MGEFF_DIRECTION_BOTTOM2TOP:
            cur_h = h * reach;
            starty = h - cur_h;
            StretchBlt (source2->hdc, rc2.left, rc2.top, RECTW(rc2), RECTH(rc2),
                    sink_dc, x + startx, y + starty, cur_w, cur_h,
                    0);
            starty = 0;
            cur_h = h - cur_h;
            break;
        default:
            break;
    }

    STRETCHBLT (source1->hdc, rc1.left, rc1.top, RECTW(rc1), RECTH(rc1),
            sink_dc, x + startx, y + starty, cur_w, cur_h,
            0);
}

void effzipeffector_begindraw (MGEFF_ANIMATION handle, MGEFF_EFFECTOR _effector)
{
    float value = 0.0f;
    mGEffAnimationSetStartValue (handle, &value);
    value = 1.0f;
    mGEffAnimationSetEndValue (handle, &value);
}

int effzipeffector_setproperty (MGEFF_EFFECTOR _effector, int property_id, int value)
{
    EffEffector *effector = (EffEffector *)_effector;
    if (property_id == MGEFF_PROPERTY_DIRECTION) {
        if (value >= MGEFF_DIRECTION_LEFT2RIGHT && value <= MGEFF_DIRECTION_BOTTOM2TOP) {
            effector->direction = value;
            return 0;
        }
        return -1;
    }
    return effbaseeffector_setproperty(_effector, property_id, value);
}

MGEFF_EFFECTOROPS zipeffector = 
{
    MGEFF_MINOR_zip, 
    MGEFF_FLOAT,
    NULL,
    NULL,
    effzipeffector_ondraw,
    effzipeffector_begindraw, 
    NULL,
    effzipeffector_setproperty,
    NULL
};
#endif
