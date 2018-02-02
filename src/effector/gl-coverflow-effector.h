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
#ifndef  GL_COVERFLOW_EFFECTOR_INC
#define  GL_COVERFLOW_EFFECTOR_INC

#include "effector.h"

#if defined(HAVE_MESA_MINIGUI)
#include <GL/gl.h>
#include <GL/mgmesa.h>
#elif defined(HAVE_EGL_SUPPORT)
#include <EGL/egl.h>
#endif

#include "opengl-manager.h"

enum {
    MAX_FLOW = 7,
    OBVERSE_DEGREE = 90,
    DEXTRAL_ANGLE = 70,
    LEFTWARD_DEGREE = 360 - DEXTRAL_ANGLE,
};
enum SLIP_DIRECTION
{
    LEFTWARD,
    RIGHTWARD
};

typedef struct _EffCoverFlowCtxt
{
    int     m_direction;
    int     m_offsetByTurn;
    OpenGLManager *m_mesaManager;
//    GLuint  m_textureID[MAX_FLOW];
    GLuint  m_backgroundTexture;
    GLfloat m_rotateDegreeUnit;
    GLfloat m_moveDegreeUnit;
    GLfloat m_verticalElevationUnit;
    GLfloat m_verticalElevation;
    GLfloat m_moveDistance;
    GLfloat m_mainRotateAngle;
    GLfloat m_subRotateAngle;
    GLfloat m_gapUnit;
    GLfloat m_gapDistance;
    HDC     m_background;
} EffCoverFlowCtxt;

/* static EffCoverFlowEffector * create(int direction, int ms_total_time,
 *         int total_frames);
 * virtual ~EffCoverFlowEffector();
 */
int beginDraw();
int drawOneFrame();
int endDraw();
int setBackground(HDC hdc);
void slipDirection(SLIP_DIRECTION);
//void index(int);
//int index() const;

/* EffCoverFlowEffector(int direction, int ms_total_time, int total_frames);
 */
void renderScene();
void turnNext();


MGEFF_EFFECTOR effcoverfloweffector_init(MGEFF_EFFECTOR _effector);
void effcoverfloweffector_finalize(MGEFF_EFFECTOR _effector);
void effcoverfloweffector_begindraw(MGEFF_ANIMATION animation, MGEFF_EFFECTOR _effector);
void effcoverfloweffector_enddraw(MGEFF_ANIMATION animation, MGEFF_EFFECTOR _effector);
void effcoverfloweffector_ondraw(MGEFF_ANIMATION animation, MGEFF_EFFECTOR _effector, 
        HDC sink_dc, int id, void* value);

#endif   /* ----- #ifndef GL_COVERFLOW_EFFECTOR_INC  ----- */
