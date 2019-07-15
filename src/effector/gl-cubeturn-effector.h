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
#ifndef  _GL_CUBETURN_EFFECTOR__INC
#define  _GL_CUBETURN_EFFECTOR__INC

#include "effector.h"
#include "opengl-manager.h"

#define	MAX_TEXTURES_ID	2 /* Number of textures id. */
enum {
    LAST_KEY_FRAME = 0,
    FIRST_KEY_FRAME = 1,
    KEY_FRAMES_COUNT = 2,
};

typedef struct _EffCubeTurnCtxt{
    int m_axis;
    unsigned int m_texture_id[MAX_TEXTURES_ID];

    OpenGLManager *m_mesaManager;
    EffHDCSource *m_source1;
    EffHDCSource *m_source2;
} EffCubeTurnCtxt;

MGEFF_EFFECTOR effcubeturneffector_init (MGEFF_EFFECTOR _effector);
void effcubeturneffector_finalize (MGEFF_EFFECTOR _effector);
int  effcubeturneffector_setproperty(MGEFF_EFFECTOR _effector, int property_id, int value);
void effcubeturneffector_ondraw(MGEFF_ANIMATION animation, MGEFF_EFFECTOR _effector, 
        HDC sink_dc, int id, void* value);
void effcubeturneffector_begindraw(MGEFF_ANIMATION animation, MGEFF_EFFECTOR _effector);
void effcubeturneffector_enddraw(MGEFF_ANIMATION animation, MGEFF_EFFECTOR _effector);

#endif   /* ----- #ifndef _CUBETURN_EFFECTOR_H__INC  ----- */

