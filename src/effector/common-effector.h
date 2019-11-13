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
