#ifndef _GARBAGE_COLLECTION_H
#define _GARBAGE_COLLECTION_H

#include "animation.h"
#include "schedular.h"

int EffGCInit(void);
void EffGCDeinit(void);
void EffGCCheckPoint(void);

void EffGCAddAnimation(EffAnimation *animation);
void EffGCRemoveAnimation(EffAnimation *animation);

void EffGCAddSchedular(EffSchedular *schedular);
void EffGCRemoveSchedular(EffSchedular *schedular);
        
#endif
