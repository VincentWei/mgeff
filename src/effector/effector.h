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
#ifndef _EFF_EFFECTOR_H
#define _EFF_EFFECTOR_H
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "linux-list.h"
#include "mgeff-effector.h"
#include "animation.h"
#include "effector-sink.h"
#include "effector-source.h"

#define FAST_MODE   0
#define MGPLUS_MODE 1

typedef struct _EffCBObjInfo{
    struct list_head list;
    void* obj;
}EffCBObjInfo;

struct _EffEffector;
typedef struct _EffEffector EffEffector;

struct _EffEffector{
    unsigned long key;
    MGEFF_BOOL initstate;
    int        direction;
    EffSink*   sink;
    void*      context;
    struct list_head source_list;
    MGEFF_EFFECTOROPS* ops;
};

int  effbaseeffector_setproperty(MGEFF_EFFECTOR effector, int property_id, int value);
int  effbaseeffector_getproperty(MGEFF_EFFECTOR effector, int property_id, int* pValue);
void effbaseeffector_rect(HDC hdc, RECT* rect);
int  effeffector_init(void);
void effeffector_deinit(void);
#endif
