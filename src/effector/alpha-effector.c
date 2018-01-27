#include "mgeff.h"
#include "alpha-effector.h"
#include "eff-common.h"

#ifdef _MGEFF_ALPHAEFFECTOR
void effalphaeffector_ondraw(MGEFF_ANIMATION animation, MGEFF_EFFECTOR _effector, 
        HDC sink_dc, intptr_t id, void* value)
{
    EffEffector *effector = (EffEffector *)_effector;

    EffHDCSource *source1 = (EffHDCSource *) (effector->source_list.next);
    EffHDCSource *source2 = (EffHDCSource *) (source1->list.next);
    RECT rc1;
    RECT rc2;
    RECT rc_sink;

    effbaseeffector_rect(source1->hdc, &rc1);
    effbaseeffector_rect(source2->hdc, &rc2);
    effbaseeffector_rect(sink_dc, &rc_sink);

    _MGEFF_PRINTF("%s:value = %f\n", __FUNCTION__, *(float*)value);

    if (sink_dc != HDC_INVALID) {
        SetMemDCAlpha (source2->hdc, MEMDC_FLAG_SRCALPHA,  (*(float*)value) * 255);
        BitBlt (source1->hdc, rc1.left, rc1.top, RECTW(rc1),
                RECTH(rc1 ), sink_dc, rc_sink .left, rc_sink .top, 0);
        BitBlt (source2->hdc, rc2.left, rc2.top, RECTW(rc2 ),
                RECTH(rc2 ), sink_dc, rc_sink.left, rc_sink.top, 0);
    }
}

MGEFF_EFFECTOROPS alphaeffector = 
{ 
    MGEFF_MINOR_alpha,
    MGEFF_FLOAT, 
    NULL,
    NULL,
    effalphaeffector_ondraw,  
    NULL, 
    NULL,
    NULL
};
#endif
