#ifndef _EFF_EFFECTORCUBIC_H
#define _EFF_EFFECTORCUBIC_H
#include "effector.h"

typedef struct _EffCubicCtxt{
    int mode;
    HGRAPHICS graphics;
}EffCubicCtxt;

MGEFF_EFFECTOR effcubiceffector_init (MGEFF_EFFECTOR _effector);
MGEFF_EFFECTOR effcubiceffector_mgplus_init (MGEFF_EFFECTOR _effector);
void effcubiceffector_finalize (MGEFF_EFFECTOR _effector);
int  effcubiceffector_setproperty(MGEFF_EFFECTOR _effector, int property_id, int value);
void effcubiceffector_ondraw(MGEFF_ANIMATION animation, MGEFF_EFFECTOR _effector, 
        HDC sink_dc, intptr_t id, void* value);
void effcubiceffector_begindraw(MGEFF_ANIMATION animation, MGEFF_EFFECTOR _effector);
void effcubiceffector_enddraw(MGEFF_ANIMATION animation, MGEFF_EFFECTOR _effector);
#endif
