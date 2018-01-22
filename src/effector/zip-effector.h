#ifndef _ZIP_EFFECTOR_H
#define _ZIP_EFFECTOR_H
#include "effector.h"

void effzipeffector_ondraw (MGEFF_ANIMATION animation, MGEFF_EFFECTOR _effector,
                   HDC sink_dc, int id, void *value);
void effzipeffector_begindraw (MGEFF_ANIMATION handle, MGEFF_EFFECTOR _effector);
int effzipeffector_setproperty (MGEFF_EFFECTOR _effector, int property_id, int value);

#endif
