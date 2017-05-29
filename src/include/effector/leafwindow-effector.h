#ifndef _EFF_EFFECTORLEAFWIN_H
#define _EFF_EFFECTORLEAFWIN_H
#include "effector.h"

typedef struct _EffLeafCtxt{
    BOOL vertical;
    int  nrows;
}EffLeafCtxt;

MGEFF_EFFECTOR effleafceffector_init(MGEFF_EFFECTOR _effector);
void effleafceffector_finalize(MGEFF_EFFECTOR _effector);
int  effleafceffector_setproperty(MGEFF_EFFECTOR _effector, int property_id, int value);
void effleafeffector_ondraw(MGEFF_ANIMATION animation, MGEFF_EFFECTOR _effector, 
        HDC sink_dc, int id, void* value);
//void effleafeffector_begindraw(MGEFF_ANIMATION animation, MGEFF_EFFECTOR effector);
//void effleafeffector_enddraw(MGEFF_ANIMATION animation, MGEFF_EFFECTOR effector);
#endif

