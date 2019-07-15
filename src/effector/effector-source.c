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
 *   <http://www.minigui.com/en/about/licensing-policy/>.
 */
#include <string.h>
#include <stdio.h>
#include "mgeff.h"
#include "eff-common.h"
#include "effector-source.h"

MGEFF_SOURCE mGEffCreateSource(HDC hdc)
{
    return mGEffCreateSourceEx(hdc, NULL);
}

MGEFF_SOURCE mGEffCreateSourceEx(HDC hdc, MGEFF_DELETEHDC_CB cb)
{
    EffHDCSource* source = (EffHDCSource*)calloc(1, sizeof(EffHDCSource));
    source->hdc = hdc;
    //source->rect.right  = GetGDCapability(hdc, GDCAP_MAXX) + 1;
    //source->rect.bottom = GetGDCapability(hdc, GDCAP_MAXY) + 1;
    source->cb = cb;
    return (MGEFF_SOURCE)source;
}

HDC mGEffGetSourceDC(MGEFF_SOURCE handle)
{
    EffHDCSource* source = (EffHDCSource*)handle;
    CHECK_HANDLE_RET_INV(handle);

    return source->hdc;
}
