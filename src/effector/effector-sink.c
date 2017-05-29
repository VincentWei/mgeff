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
        if (sink->bufdc) 
            BitBlt(sink->bufdc, 0, 0, 0, 0, sink->hdc, 0, 0, 0);
        return;
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
