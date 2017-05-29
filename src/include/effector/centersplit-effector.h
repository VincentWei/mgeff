#ifndef _CENTERSPLIT_EFFECTOR_H
#define _CENTERSPLIT_EFFECTOR_H
#include "effector.h"

MGEFF_EFFECTOR effcenterspliteffector_init(MGEFF_EFFECTOR _effector);
void effcenterspliteffector_finalize(MGEFF_EFFECTOR _effector);
void effcenterspliteffector_ondraw (MGEFF_ANIMATION animation, MGEFF_EFFECTOR _effector,
                   HDC sink_dc, int id, void *value);
void effcenterspliteffector_begindraw (MGEFF_ANIMATION handle, MGEFF_EFFECTOR _effector);
int effcenterspliteffector_setproperty (MGEFF_EFFECTOR _effector, int property_id, int value);
#endif
