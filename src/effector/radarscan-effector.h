/* 
 * $Id: radarscan-effector.h 1389 2010-07-01 07:29:08Z huzhaolin $
 *
 * radarscan.h: For radarscan-effector.c.
 *
 * Copyright (C) 2010 Feynman Software.
 *
 * Author: rlzheng
 *
 * License: GPL
 */

#ifndef _EFF_RADAR_SCAN_H
#define _EFF_RADAR_SCAN_H

#include "effector.h"

typedef struct _EffRadarScanCtxt{
    BLOCKHEAP cliprc_heap;
    CLIPRGN cliprgn;
    POINT pts[10];
    POINT special_points[4];
    int start_angle;
    int middle_angle;
    int direction;
    int npieces;
}EffRadarScanCtxt;

MGEFF_EFFECTOR effradarscaneffector_init(MGEFF_EFFECTOR _effector);

void effradarscaneffector_finalize (MGEFF_EFFECTOR _effector);

void effradarscaneffector_ondraw(MGEFF_ANIMATION animation,
        MGEFF_EFFECTOR _effector, HDC sink_dc, intptr_t id, void* value);
void effradarscaneffector_begindraw(MGEFF_ANIMATION animation,
        MGEFF_EFFECTOR _effector);
void effradarscaneffector_enddraw(MGEFF_ANIMATION animation,
        MGEFF_EFFECTOR _effector);
int  effradarscaneffector_setproperty(MGEFF_EFFECTOR _effector,
        int property_id, int value);

#endif /* end of _EFF_RADAR_SCAN_H */
