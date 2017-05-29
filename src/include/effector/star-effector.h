/* 
 * $Id: star-effector.c 1252 2010-06-04 03:51:07Z humingming $
 *
 * star-effector.c: five star effector implement.
 *
 * Copyright (C) 2010 Feynman Software.
 *
 * Author: humingming
 *
 * License: GPL
 */

#ifndef _STAR_EFFECTOR_H
#define _STAR_EFFECTOR_H

#include "effector.h"
#include <mgplus/mgplus.h>


typedef struct _EffStarCtxt{
    int zoom;
    int current_angle;

    HPATH path;
    PCLIPRGN pRgn;
}EffStarCtxt;


MGEFF_EFFECTOR effstareffector_init (MGEFF_EFFECTOR _effector);
void effstareffector_finalize (MGEFF_EFFECTOR _effector);

void effstareffector_ondraw (MGEFF_ANIMATION animation,
        MGEFF_EFFECTOR _effector, HDC sink_dc, int id, void* value);

void effstarscaneffector_begindraw (MGEFF_ANIMATION animation,
        MGEFF_EFFECTOR _effector);

void effstareffector_enddraw (MGEFF_ANIMATION animation,
        MGEFF_EFFECTOR _effector);

int effstareffector_setproperty (MGEFF_EFFECTOR _effector,
        int property_id, int value);

int effstareffector_getproperty (MGEFF_EFFECTOR _effector,
        int property_id, int* value);

#endif /* end of _STAR_EFFECTOR_H */


