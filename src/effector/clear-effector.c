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
#include "effector.h"
#include "eff-common.h"
#include "common-effector.h"
#include "clear-effector.h"

#ifdef _MGEFF_CLEAREFFECTOR

static void effcleareffector_begindraw(MGEFF_ANIMATION animation, MGEFF_EFFECTOR _effector)
{
    EffEffector *effector = (EffEffector *)_effector;

    POINT pt_s = {0, 0};
    POINT pt_e = {0, 0};
    if (effector->sink) {
        int w = GetGDCapability(effector->sink->hdc, GDCAP_MAXX) + 1;
        int h = GetGDCapability(effector->sink->hdc, GDCAP_MAXY) + 1;

        if (effector->direction == MGEFF_DIRECTION_LEFT2RIGHT) {
            pt_s.x = -w;
        }
        else if (effector->direction == MGEFF_DIRECTION_RIGHT2LEFT) {
            pt_s.x = w;
        }
        else if (effector->direction == MGEFF_DIRECTION_TOP2BOTTOM) {
            pt_s.y = -h;
        }
        else if (effector->direction == MGEFF_DIRECTION_BOTTOM2TOP) {
            pt_s.y = h;
        }
    }
    else {
        fprintf(stderr, "%s:effector's sink is not set.\n", __FUNCTION__);
    }
    mGEffAnimationSetStartValue(animation, &pt_s);
    mGEffAnimationSetEndValue(animation, &pt_e);
}

static void effcleareffector_ondraw(MGEFF_ANIMATION animation, MGEFF_EFFECTOR _effector, HDC sink_dc, intptr_t id, void* value)
{
    EffEffector *effector = (EffEffector *)_effector;
    EffClearCtxt* cleareffector = (EffClearCtxt*)effector->context;

    EffHDCSource *src_old = (EffHDCSource *) (effector->source_list.next);
    EffHDCSource *src_new = (EffHDCSource *) (src_old->list.next);
    RECT rc_sink, rc_src_old, rc_src_new;

    effbaseeffector_rect(sink_dc, &rc_sink);
    effbaseeffector_rect(src_old->hdc, &rc_src_old);
    effbaseeffector_rect(src_new->hdc, &rc_src_new);

#if 0
    /*blit the new picture*/
    BitBlt (src_new->hdc, rc_src_new.left, rc_src_new.top, RECTW(rc_src_new), RECTH(rc_src_new),
            sink_dc, rc_sink.left, rc_sink.top, 0);
    /*blit the old one*/
    BitBlt (src_old->hdc, ((POINT*)value)->x,((POINT*)value)->y, RECTW(rc_src_old), RECTH(rc_src_old),
            sink_dc, rc_sink.left, rc_sink.top, 0);
#else 
    BitBlt (src_new->hdc, cleareffector->prev_x, cleareffector->prev_y, RECTW(rc_src_old), RECTH(rc_src_old),
            sink_dc, cleareffector->prev_x, cleareffector->prev_y, 0);
    /*blit the old one*/
    BitBlt (src_old->hdc, rc_src_old.left, rc_src_old.top, RECTW(rc_src_old), RECTH(rc_src_old),
            sink_dc, ((POINT*)value)->x,((POINT*)value)->y, 0);

    cleareffector->prev_x = ((POINT*)value)->x;
    cleareffector->prev_y = ((POINT*)value)->y;
#endif
}

static MGEFF_EFFECTOR effcleareffector_init(MGEFF_EFFECTOR _effector)
{
    EffEffector *effector = (EffEffector *)_effector;
    EffClearCtxt* clear_context = (EffClearCtxt*)calloc(1, sizeof(EffClearCtxt));
    effector->context = clear_context;
    return _effector;
}

static void effcleareffector_finalize(MGEFF_EFFECTOR _effector)
{
    EffEffector *effector = (EffEffector *)_effector;
    free (effector->context);
}

MGEFF_EFFECTOROPS cleareffector = 
{
    MGEFF_MINOR_clear, 
    MGEFF_POINT, 
    effcleareffector_init,
    effcleareffector_finalize,
    effcleareffector_ondraw,
    effcleareffector_begindraw, 
    NULL,
    NULL,
    NULL,
};
#endif
