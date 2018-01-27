#ifndef _EFF_EFFECTORCLEAR_H
#define _EFF_EFFECTORCLEAR_H
#include "effector.h"

typedef struct _EffClearCtxt{
    int prev_x;
    int prev_y;
}EffClearCtxt;

MGEFF_EFFECTOR effcleareffector_init(MGEFF_EFFECTOR _effector);
void effcleareffector_finalize(MGEFF_EFFECTOR _effector);
int  effcleareffector_setproperty(MGEFF_EFFECTOR _effector, int property_id, int value);
void effcleareffector_begindraw(MGEFF_ANIMATION animation, MGEFF_EFFECTOR _effector);
void effcleareffector_enddraw(MGEFF_ANIMATION animation, MGEFF_EFFECTOR _effector);
void effcleareffector_ondraw(MGEFF_ANIMATION animation, MGEFF_EFFECTOR _effector, 
        HDC sink_dc, intptr_t id, void* value);
#endif
