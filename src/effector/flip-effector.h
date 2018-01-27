#ifndef _EFF_EFFECTORFLIP_H
#define _EFF_EFFECTORFLIP_H
#include "effector.h"

typedef struct _EffFlipCtxt{
    int mode;
    HGRAPHICS graphics;
    HPATH path;
}EffFlipCtxt;

MGEFF_EFFECTOR effflipeffector_init(MGEFF_EFFECTOR _effector);
void effflipeffector_finalize(MGEFF_EFFECTOR _effector);
int  effflipeffector_setproperty(MGEFF_EFFECTOR _effector, int property_id, int value);
void effflipeffector_begindraw(MGEFF_ANIMATION animation, MGEFF_EFFECTOR _effector);
void effflipeffector_enddraw(MGEFF_ANIMATION animation, MGEFF_EFFECTOR _effector);
void effflipeffector_ondraw(MGEFF_ANIMATION animation, MGEFF_EFFECTOR _effector, 
        HDC sink_dc, intptr_t id, void* value);
#endif
