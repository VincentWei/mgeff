#ifndef _ZOOM_EFFECTOR_H
#define _ZOOM_EFFECTOR_h
#include "effector.h"

typedef struct _EffZoomCtxt{
    int zoom;
    RECT prev_rc;
}EffZoomCtxt;


MGEFF_EFFECTOR effzoomeffector_init (MGEFF_EFFECTOR _effector);

void effzoomeffector_finalize (MGEFF_EFFECTOR _effector);

void effzoomeffector_ondraw (MGEFF_ANIMATION animation,
        MGEFF_EFFECTOR _effector, HDC sink_dc, intptr_t id, void* value);

void effstarscaneffector_begindraw (MGEFF_ANIMATION animation,
        MGEFF_EFFECTOR _effector);

int effzoomeffector_setproperty (MGEFF_EFFECTOR _effector,
        int property_id, int value);

int effzoomeffector_getproperty (MGEFF_EFFECTOR _effector,
        int property_id, int* value);

#endif
