#ifndef _EFF_EFFECTORSINK_H
#define _EFF_EFFECTORSINK_H
#include "mgeff-effector.h"
#include "mgeff-effector-sink.h"

typedef struct _EffSink{
    enum MGEFF_SINK_TYPE type;
    int  status;
    HDC  bufdc;
    HDC  hdc;
    HWND hwnd;
    //RECT rect;
}EffSink;

HDC effsink_get(EffSink* sink);
void effsink_release(EffSink* sink, HDC hdc);

#endif
