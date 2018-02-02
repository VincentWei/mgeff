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
#include "effector.h"
#include "eff-common.h"
#include "common-effector.h"

#ifdef _MGEFF_PUSHEFFECTOR
static void effpusheffector_in_rect(MGEFF_EFFECTOR _effector, RECT* cur_rc, RECT* rc_out)
{
    EffEffector *effector = (EffEffector *)_effector;

    int w = RECTWP(cur_rc);
    int h = RECTHP(cur_rc);
    *rc_out = *cur_rc;
    if (effector->direction == MGEFF_DIRECTION_TOP2BOTTOM) {
        rc_out->top = cur_rc->top + h;
        rc_out->bottom = cur_rc->bottom + h;
    }
    else if (effector->direction == MGEFF_DIRECTION_BOTTOM2TOP) {
        rc_out->top = cur_rc->top - h;
        rc_out->bottom = cur_rc->bottom - h;
    }
    else if (effector->direction == MGEFF_DIRECTION_LEFT2RIGHT) {
        rc_out->left = cur_rc->left + w;
        rc_out->right = cur_rc->right + w;
    }
    else if (effector->direction == MGEFF_DIRECTION_RIGHT2LEFT) {
        rc_out->left = cur_rc->left - w;
        rc_out->right = cur_rc->right - w;
    }
}

static void effpusheffector_ondraw(MGEFF_ANIMATION animation, MGEFF_EFFECTOR _effector, HDC sink_dc, intptr_t id, void* value)
{
    EffEffector *effector = (EffEffector *)_effector;

    EffHDCSource *src_in = (EffHDCSource *) (effector->source_list.next);
    EffHDCSource *src_out = (EffHDCSource *) (src_in->list.next);
    RECT rc_sink, rc_out, rc_in;
    RECT* cur_rc = (RECT*)value;

    effbaseeffector_rect(sink_dc, &rc_sink);
    effbaseeffector_rect(src_in->hdc, &rc_out);
    effbaseeffector_rect(src_out->hdc, &rc_in);

    /*new one*/
    BitBlt (src_out->hdc, 0, 0, 0, 0,
            sink_dc, cur_rc->left, cur_rc->top, 0);
    effpusheffector_in_rect(_effector, cur_rc, &rc_in);
    /*old one*/
    BitBlt (src_in->hdc, 0, 0, 0, 0, 
            sink_dc, rc_in.left, rc_in.top, 0);
}

static void effpusheffector_begindraw(MGEFF_ANIMATION handle, MGEFF_EFFECTOR _effector)
{
    EffEffector *effector = (EffEffector *)_effector;
    RECT rc_s = {0};
    RECT rc_e = {0};
    if (effector->sink) {
        int w = GetGDCapability(effector->sink->hdc, GDCAP_MAXX) + 1;
        int h = GetGDCapability(effector->sink->hdc, GDCAP_MAXY) + 1;

        rc_e.right  = w;
        rc_e.bottom = h;
        if (effector->direction == MGEFF_DIRECTION_TOP2BOTTOM) {
            rc_s.top = -h;
        }
        else if (effector->direction == MGEFF_DIRECTION_BOTTOM2TOP) {
            rc_s.top = h;
            rc_s.bottom = 2 * h;
        }
        else if (effector->direction == MGEFF_DIRECTION_LEFT2RIGHT) {
            rc_s.left = -w;
        }
        else if (effector->direction == MGEFF_DIRECTION_RIGHT2LEFT) {
            rc_s.left  = w;
            rc_s.right = 2 * w;
        }
    }
    mGEffAnimationSetStartValue (handle, &rc_s);
    mGEffAnimationSetEndValue (handle, &rc_e);
}

static int effpusheffector_setproperty(MGEFF_EFFECTOR _effector, int property_id, int value)
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

MGEFF_EFFECTOROPS pusheffector = 
{
    MGEFF_MINOR_push,
    MGEFF_RECT, 
    NULL,
    NULL,
    effpusheffector_ondraw,
    effpusheffector_begindraw, 
    NULL,
    effpusheffector_setproperty, 
    NULL, 
};
#endif
