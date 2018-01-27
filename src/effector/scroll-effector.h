#ifndef _EFF_EFFECTORSCROLL_H
#define _EFF_EFFECTORSCROLL_H
#include "effector.h"

typedef struct _EffScrollPageCtxt{
    HGRAPHICS graphics; 
    HPATH leftpath;
    HPATH middlepath;
    HPEN  pen;
    HBRUSH brush;
}EffScrollPageCtxt;

MGEFF_EFFECTOR effscrolleffector_init(MGEFF_EFFECTOR _effector);
void effscrolleffector_finalize(MGEFF_EFFECTOR _effector);
void effscrolleffector_ondraw(MGEFF_ANIMATION animation, MGEFF_EFFECTOR _effector, HDC sink_dc, intptr_t id, void* value);
void effscrolleffector_begindraw(MGEFF_ANIMATION animation, MGEFF_EFFECTOR _effector);
void effscrolleffector_enddraw(MGEFF_ANIMATION animation, MGEFF_EFFECTOR _effector);
#endif
