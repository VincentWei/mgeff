#ifndef _EFF_EFFECTORSOURCE_H
#define _EFF_EFFECTORSOURCE_H

#include "linux-list.h"
#include "mgeff-effector.h"
#include "mgeff-effector-source.h"

//typedef void (*MGEFF_DELETEHDC_CB)(HDC hdc);

typedef struct _EffHDCSource {
    struct list_head list;
    HDC  hdc;
    //RECT rect;
    MGEFF_DELETEHDC_CB cb;
}EffHDCSource;

#endif
