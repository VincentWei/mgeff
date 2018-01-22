#ifndef _EFF_EFFECTORCOMMON_H
#define _EFF_EFFECTORCOMMON_H

MGEFF_EXPORT void effcommon_v_fillladder(HDC src_dc, int src_left, int src_top, int src_right, int src_bottom,
        HDC dst_dc, int dst_left, int dst_top, int dst_right, int dst_bottom,
        int dst_x0, int dst_y0, 
        int dst_x1, int dst_y1, 
        int dst_x2, int dst_y2, 
        int dst_x3, int dst_y3);

MGEFF_EXPORT void effcommon_h_fillladder(HDC src_dc, int src_left, int src_top, int src_right, int src_bottom,
        HDC dst_dc, int dst_left, int dst_top, int dst_right, int dst_bottom,
        int dst_x0, int dst_y0,
        int dst_x1, int dst_y1,
        int dst_x2, int dst_y2,
        int dst_x3, int dst_y3);
#endif
