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
#include <string.h>
#include <stdio.h>
#include "mgeff.h"
#include "eff-common.h"
#include "effector-sink.h"

MGEFF_SINK mGEffCreateHDCSink(HDC hdc)
{
    EffSink* sink = (EffSink*)calloc(1, sizeof(EffSink));
    sink->type  = MGEFF_SINK_HDC;
    sink->hdc  = hdc;

    return (MGEFF_SINK) sink;
}

MGEFF_SINK mGEffCreateHwndSink(HWND hwnd)
{
    EffSink* sink = (EffSink*)calloc(1, sizeof(EffSink));
    sink->type = MGEFF_SINK_HWND;
    sink->hwnd = hwnd;
    return (MGEFF_SINK) sink;
}


void mGEffDeleteSink(MGEFF_SINK sink)
{
    EffSink* s = (EffSink*)sink;

    CHECK_HANDLE(sink);
    /* delete the sink of effector.*/
    if (s) {
        if (s->bufdc) {
            DeleteMemDC(s->bufdc);
        }
        free(s);
    }
}

void mGEffSetBufferSink(MGEFF_SINK handle, HDC hdc)
{
    EffSink* sink = (EffSink*)handle;
    CHECK_HANDLE(sink);
    if (sink->bufdc == 0) {
        if (sink->type == MGEFF_SINK_HWND) { 
            HDC hdc = GetSecondaryDC(sink->hwnd);
            sink->bufdc = CreateCompatibleDC(hdc);
            BitBlt(hdc, 0, 0, 0, 0, sink->bufdc, 0, 0, 0);
            ReleaseDC(hdc);
        }
        else {
            sink->bufdc = CreateCompatibleDC(hdc);
            BitBlt(hdc, 0, 0, 0, 0, sink->bufdc, 0, 0, 0);
        }
    }
}

HDC effsink_get(EffSink* sink)
{
    if (sink->type == MGEFF_SINK_HDC) {
        if (sink->bufdc){
            return sink->bufdc;
        }
        return sink->hdc;
    }
#ifndef _MG_MINIMALGDI
    else if (sink->type == MGEFF_SINK_HWND) { 
        if (IsWindow(sink->hwnd)) {
            if (sink->bufdc)
                return sink->bufdc;
            return GetDC(sink->hwnd);
        }
    }
#endif
    return HDC_INVALID;
}

void effsink_release(EffSink* sink, HDC hdc)
{
    if (sink->type == MGEFF_SINK_HDC) {
        if (sink->bufdc) {
            BitBlt (sink->bufdc, 0, 0, 0, 0, sink->hdc, 0, 0, 0);
#ifdef _MGUSE_UPDATE_REGION
            // Since 4.2.0
            SyncUpdateDC (sink->hdc);
#endif
        }
    }
    else if (sink->type == MGEFF_SINK_HWND) {
        if (sink->bufdc) {
            HDC hdc = GetDC(sink->hwnd);
            BitBlt(sink->bufdc, 0, 0, 0, 0, hdc, 0, 0, 0);
            ReleaseDC(hdc);
            return;
        }

        if (hdc != HDC_INVALID)
            ReleaseDC(hdc);
    }
}

