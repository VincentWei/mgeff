///////////////////////////////////////////////////////////////////////////////
//
//                          IMPORTANT NOTICE
//
// The following open source license statement does not apply to any
// entity in the Exception List published by FMSoft.
//
// For more information, please visit:
//
// https://www.fmsoft.cn/exception-list
//
//////////////////////////////////////////////////////////////////////////////
/*
 *   This file is part of mGEff, a component for MiniGUI.
 * 
 *   Copyright (C) 2008~2018, Beijing FMSoft Technologies Co., Ltd.
 * 
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 * 
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 * 
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 *   Or,
 * 
 *   As this program is a library, any link to this program must follow
 *   GNU General Public License version 3 (GPLv3). If you cannot accept
 *   GPLv3, you need to be licensed from FMSoft.
 * 
 *   If you have got a commercial license of this program, please use it
 *   under the terms and conditions of the commercial license.
 * 
 *   For more information about the commercial license, please refer to
 *   <http://www.minigui.com/blog/minigui-licensing-policy/>.
 */
#include <assert.h>
#include "mgeff.h"
#include "common-effector.h"

static inline void DrawHLine_32(const unsigned char *src_pixels, int src_pitch,
        int src_x, int src_y, int src_len,
        unsigned char *dst_pixels, int dst_pitch,
        int dst_left, int dst_right,
        int dst_x, int dst_y, int dst_len)
{
    int i;
    unsigned char *dst;
    const unsigned char *src;
    const unsigned char *src0;

    dst = dst_pixels + dst_y * dst_pitch + dst_x * 4;
    src0 = src_pixels + src_y * src_pitch + src_x * 4;

    for (i = 0; i < dst_len; ++i, dst += 4) {
        if (dst_x + i < dst_left) {
            continue;
        }
        if (dst_x + i >= dst_right)
            break;
        src = src0 + i * src_len / dst_len * 4;
        *((unsigned int *)dst) = *((const unsigned int *)src);
    }
}

static inline void DrawHLine_16(const unsigned char *src_pixels, int src_pitch,
        int src_x, int src_y, int src_len,
        unsigned char *dst_pixels, int dst_pitch,
        int dst_left, int dst_right,
        int dst_x, int dst_y, int dst_len)
{
    int i;
    unsigned char *dst;
    const unsigned char *src;
    const unsigned char *src0;

    dst = dst_pixels + dst_y * dst_pitch + dst_x * 2;
    src0 = src_pixels + src_y * src_pitch + src_x * 2;

    for (i = 0; i < dst_len; ++i, dst += 2) {
        if (dst_x + i < dst_left) {
            continue;
        }
        if (dst_x + i >= dst_right)
            break;
        src = src0 + i * src_len / dst_len * 2;
        *((unsigned short int *)dst) = *((const unsigned short int *)src);
    }
}

static inline void DrawVLine_32(const unsigned char *src_pixels, int src_pitch,
        int src_x, int src_y, int src_len, 
        unsigned char *dst_pixels, int dst_pitch,
        int dst_top, int dst_bottom,
        int dst_x, int dst_y, int dst_len)
{
    int i;
    int yd;
    // int ys;
    unsigned char *dst;
    const unsigned char *src0;
    const unsigned char *src;

    assert(src_x >= 0);
    assert(src_y >= 0);

    dst = dst_pixels + dst_y * dst_pitch + dst_x * 4;
    src0 = src_pixels + src_y * src_pitch + src_x * 4;
    for (i=0; i<dst_len; i++) {
        yd = dst_y + i;
        if (yd < dst_top){
            dst += dst_pitch;
            continue;
        }
        if (yd >= dst_bottom){
            break;
        }
        /*
        ys = src_y + i * src_len / dst_len;
        dst = (unsigned int *)(dst_pixels + yd * dst_pitch + dst_x * 4);
        src = (const unsigned int *)(src_pixels + ys * src_pitch + src_x * 4);
        */
        src = src0 + (i*src_len/dst_len) * src_pitch;
        *((unsigned int *)dst) = *((const unsigned int *)src);
        dst += dst_pitch;
    }
}

static inline void DrawVLine_16(const unsigned char *src_pixels, int src_pitch,
        int src_x, int src_y, int src_len, 
        unsigned char *dst_pixels, int dst_pitch,
        int dst_top, int dst_bottom,
        int dst_x, int dst_y, int dst_len)
{
    int i;
    int yd;
    unsigned char *dst;
    const unsigned char *src0;
    const unsigned char *src;

    assert(src_x >= 0);
    assert(src_y >= 0);

    dst = dst_pixels + dst_y * dst_pitch + dst_x * 2;
    src0 = src_pixels + src_y * src_pitch + src_x * 2;
    for (i=0; i<dst_len; i++) {
        yd = dst_y + i;
        if (yd < dst_top){
            dst += dst_pitch;
            continue;
        }
        if (yd >= dst_bottom){
            break;
        }
        /*
        ys = src_y + i * src_len / dst_len;
        dst = (unsigned int *)(dst_pixels + yd * dst_pitch + dst_x * 4);
        src = (const unsigned int *)(src_pixels + ys * src_pitch + src_x * 4);
        */
        src = src0 + (i*src_len/dst_len) * src_pitch;
        *((unsigned short int *)dst) = *((const unsigned short int *)src);
        dst += dst_pitch;
    }
}

/*    3  |  0
 *   ----+----
 *    2  |  1
 */
void effcommon_v_fillladder(HDC src_dc, int src_left, int src_top, int src_right, int src_bottom,
        HDC dst_dc, int dst_left, int dst_top, int dst_right, int dst_bottom,
        int dst_x0, int dst_y0, 
        int dst_x1, int dst_y1, 
        int dst_x2, int dst_y2, 
        int dst_x3, int dst_y3)
{
    const unsigned char *src_pixels;
    unsigned char *dst_pixels;
    int src_pitch, dst_pitch;
    int i;
    int depth;

    assert(dst_x0 == dst_x1);
    assert(dst_x3 == dst_x2);
    assert(dst_x0 >= dst_x3);

    depth = GetGDCapability(dst_dc, GDCAP_DEPTH);
    if (GetGDCapability(src_dc, GDCAP_DEPTH) != depth) {
        return;
    }
    src_pixels = LockDC(src_dc, NULL, NULL, NULL, &src_pitch);
    UnlockDC(src_dc);
    dst_pixels = LockDC(dst_dc, NULL, NULL, NULL, &dst_pitch);
    UnlockDC(dst_dc);
#if 0
    int len = dst_x0 - dst_x3;
    for (i=0; i<len; i++) {
        int y0 = dst_y3 - i * (dst_y0 - dst_y3) / len;
        int y1 = dst_y2 + i * (dst_y2 - dst_y1) / len;
        int src_x0 = i * (src_right - src_left) / len + src_left;
#else
    for (i=0; i<(dst_x0 - dst_x3); i++) {
        int y0 = dst_y3 - i * (dst_y3 - dst_y0) / (dst_x0 - dst_x3);
        int y1 = dst_y2 - i * (dst_y2 - dst_y1) / (dst_x1 - dst_x2);
        int src_x0 = i * (src_right - src_left) / (dst_x0 - dst_x3) + src_left;
#endif
        if (dst_x3+i < dst_left) {
            continue;
        }
        if (dst_x3+i >= dst_right) {
            break;
        }
        if (depth == 32) {
            DrawVLine_32(src_pixels, src_pitch, src_x0, src_top, (src_bottom - src_top),
                    dst_pixels, dst_pitch, dst_top, dst_bottom,
                    dst_x3+i, y0, y1-y0);
        }else if (depth == 16) {
            DrawVLine_16(src_pixels, src_pitch, src_x0, src_top, (src_bottom - src_top),
                    dst_pixels, dst_pitch, dst_top, dst_bottom,
                    dst_x3+i, y0, y1-y0);
        }else{
            fprintf(stderr, "cubic-renderer: depth=%d not supported\n", depth);
        }
    }
}

void effcommon_h_fillladder(HDC src_dc, int src_left, int src_top, int src_right, int src_bottom,
        HDC dst_dc, int dst_left, int dst_top, int dst_right, int dst_bottom,
        int dst_x0, int dst_y0,
        int dst_x1, int dst_y1,
        int dst_x2, int dst_y2,
        int dst_x3, int dst_y3)
{
    const unsigned char *src_pixels;
    unsigned char *dst_pixels;
    int src_pitch;
    int dst_pitch;
    int i;
    int depth;
	int len;

    assert(dst_y3 == dst_y0);
    assert(dst_y2 == dst_y1);
    assert(dst_y2 > dst_y3);

    depth = GetGDCapability(dst_dc, GDCAP_DEPTH);
    if (GetGDCapability(src_dc, GDCAP_DEPTH) != depth) {
        return;
    }

    src_pixels = LockDC(src_dc, NULL, NULL, NULL, &src_pitch);
    UnlockDC(src_dc);
    dst_pixels = LockDC(dst_dc, NULL, NULL, NULL, &dst_pitch);
    UnlockDC(dst_dc);

    len = dst_y2 - dst_y3;
    for (i = 0; i < (dst_y2 - dst_y3); ++i) {
        int x0 = dst_x3 - i * (dst_x3 - dst_x2) / len;
        int x1 = dst_x0 + i * (dst_x1 - dst_x0) / len;
        int src_y = src_top + i * (src_bottom - src_top) / len;
        if (dst_y3 + i < dst_top)
            continue;
        if (dst_y3 + i >= dst_bottom)
            break;
        if (depth == 32) {
            DrawHLine_32(src_pixels, src_pitch, src_left, src_y, (src_right - src_left),
                    dst_pixels, dst_pitch, dst_left, dst_right, x0, dst_y3 + i, x1 - x0);
        } else if (depth == 16) {
            DrawHLine_16(src_pixels, src_pitch, src_left, src_y, (src_right - src_left),
                    dst_pixels, dst_pitch, dst_left, dst_right, x0, dst_y3 + i, x1 - x0);
        } else {
            fprintf(stderr, "cubic-renderer: depth=%d not supported\n", depth);
        }
    }
}

