#define dbg()   fprintf(stderr, "%s %d\n", __FUNCTION__, __LINE__)

#include "mgeff.h"
#include "effector.h"
#include "centersplit-effector.h"
#include "eff-common.h"

#ifdef _MGEFF_CENTERSPLITEFFECTOR
void effcenterspliteffector_ondraw (MGEFF_ANIMATION animation, MGEFF_EFFECTOR _effector,
        HDC sink_dc, int id, void *value)
{
    EffEffector *effector = (EffEffector *)_effector;
    EffHDCSource *source1 = (EffHDCSource *) (effector->source_list.next);
    EffHDCSource *source2 = (EffHDCSource *) (source1->list.next);
    RECT rc1, rc2, rc_sink;
	float reach = *(float *)value;
    int cur_w = 0, cur_h = 0;
    int x ,y, w, h;

    effbaseeffector_rect (source1->hdc, &rc1);
    effbaseeffector_rect (source2->hdc, &rc2);
    effbaseeffector_rect (sink_dc, &rc_sink);

    x = rc_sink.left;
    y = rc_sink.top;
    w = RECTW(rc_sink);
    h = RECTH(rc_sink);

    cur_w = (w * reach) / 2;
    cur_h = (h * reach) / 2;

    switch (effector->direction) {
        case MGEFF_DIRECTION_CENTER2OUTSIDE_4:
            {
                int crop_w = (RECTW(rc1) >> 1) - cur_w;
                int crop_h = (RECTH(rc1) >> 1) - cur_h;
                if (0 != cur_w || 0 != cur_h) {
                    BITBLT(source2->hdc, rc2.left, rc2.top, RECTW(rc_sink), RECTH(rc_sink),
                            sink_dc, x, y,0);
                }
                if (cur_h != 0 && cur_w != 0 && crop_w != 0 && crop_h != 0) {
                    BitBlt(source1->hdc, rc1.left + cur_w, rc1.top + cur_h, crop_w, crop_h,
                            sink_dc, x, y,
                            0);
                    BitBlt(source1->hdc, rc1.left + (RECTW(rc1) >> 1), rc1.top + cur_h, crop_w, crop_h,
                            sink_dc, x + w - ((RECTW(rc_sink) >> 1) - cur_w), y,
                            0);
                    BitBlt(source1->hdc, rc1.left + cur_w, rc1.top + (RECTH(rc1) >> 1), crop_w, crop_h,
                            sink_dc, x, y + (h >> 1) + cur_h,
                            0);
                    BitBlt(source1->hdc, rc1.left + (RECTW(rc1) >> 1), rc1.top + (RECTH(rc1) >> 1), crop_w, crop_h,
                            sink_dc, x + (w >> 1) + cur_w, y + (h >> 1) + cur_h,
                            0);
                }
            }
            break;
        case MGEFF_DIRECTION_OUTSIDE2CENTER_4:
            {
                int crop_w = RECTW(rc_sink) >> 1;
                int crop_h = RECTH(rc_sink) >> 1;
                if (mGEffAnimationGetProperty (animation, MGEFF_PROP_CURFRAME) == 0)
                    BITBLT(source1->hdc, rc1.left, rc2.top, RECTW(rc_sink), RECTH(rc_sink),
                            sink_dc, x, y,
                            0);
                if (cur_h != 0 && cur_w != 0) {
                    BitBlt(source2->hdc, crop_w - cur_w, crop_w - cur_h, cur_w, cur_h,
                            sink_dc, rc_sink.left, rc_sink.top,
                            0);
                    BitBlt(source2->hdc, crop_w, crop_h - cur_h, cur_w, cur_h,
                            sink_dc, rc_sink.right - cur_w, rc_sink.top,
                            0);
                    BitBlt(source2->hdc, crop_w - cur_w, crop_h, cur_w, cur_h,
                            sink_dc, rc_sink.left, rc_sink.bottom - cur_h,
                            0);
                    BitBlt(source2->hdc, crop_w, crop_h, cur_w, cur_h,
                            sink_dc, rc_sink.right - cur_w, rc_sink.bottom - cur_h,
                            0);
                }
            }
            break;
        case MGEFF_DIRECTION_CENTER2OUTSIDE_2:
            {
                int crop_w = RECTW(rc1);
                int crop_h = (RECTH(rc1) >> 1) - cur_h;
                if (0 != cur_h || 0 != cur_w) {
                    BITBLT(source2->hdc, rc2.left, rc2.top, RECTW(rc_sink), RECTH(rc_sink),
                            sink_dc, rc_sink.left, rc_sink.top, 0);
                }
                if (cur_h != 0 && cur_w != 0 && crop_h != 0 && crop_w != 0) {
                    BitBlt(source1->hdc, rc1.left, rc1.top + cur_h,
                            crop_w, crop_h,
                            sink_dc, x, y,
                            0);
                    BitBlt(source1->hdc, rc1.left, rc1.top + (RECTH(rc1) >> 1),
                            crop_w, crop_h,
                            sink_dc, x, y + (h >> 1) + cur_h,
                            0);
                }
            }
            break;
        case MGEFF_DIRECTION_OUTSIDE2CENTER_2:
            {
                if (mGEffAnimationGetProperty (animation, MGEFF_PROP_CURFRAME) == 0)
                    BITBLT(source1->hdc, rc1.left, rc1.top, RECTW(rc_sink), RECTH(rc_sink),
                            sink_dc, rc_sink.left, rc_sink.top,
                            0);
                if (cur_h != 0 && cur_w != 0) {
                    BitBlt(source2->hdc, x, (RECTH(rc_sink) >> 1) - cur_h, RECTW(rc_sink), cur_h,
                            sink_dc, rc_sink.left, rc_sink.top,
                            0);
                    BitBlt(source2->hdc, x, (RECTH(rc_sink) >> 1), RECTW(rc_sink), cur_h,
                            sink_dc, x, rc_sink.bottom - cur_h,
                            0);
                }
            }
            break;
        default:
            break;
    }
}

void effcenterspliteffector_begindraw (MGEFF_ANIMATION handle, MGEFF_EFFECTOR _effector)
{
    float value = 0.0f;
    mGEffAnimationSetStartValue (handle, &value);
    value = 1.0f;
    mGEffAnimationSetEndValue (handle, &value);
}

int effcenterspliteffector_setproperty (MGEFF_EFFECTOR _effector, int property_id, int value)
{
    EffEffector *effector = (EffEffector *)_effector;

    if (property_id == MGEFF_PROPERTY_DIRECTION) {
        if (value >= MGEFF_DIRECTION_CENTER2OUTSIDE_4 && value <= MGEFF_DIRECTION_OUTSIDE2CENTER_2) {
            effector->direction = value;
            return 0;
        }
        return -1;
    }
    return effbaseeffector_setproperty(_effector, property_id, value);
}

MGEFF_EFFECTOROPS centerspliteffector = 
{
    MGEFF_MINOR_centersplit, 
    MGEFF_FLOAT,
    NULL,
    NULL,
    effcenterspliteffector_ondraw,
    effcenterspliteffector_begindraw, 
    NULL,
    effcenterspliteffector_setproperty, 
    NULL
};
#endif
