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
#include <minigui/common.h>

#include "mgeff.h"

#if defined(HAVE_MESA_MINIGUI) || defined(HAVE_EGL_SUPPORT)

#include "gl-coverflow-effector.h"

const GLfloat MID_PANEL_PITCH = 2.4f;
const GLfloat PANEL_GAP = 2.0f; // gap between two sides panels
const GLfloat PANEL_SPACING = 1.0f;
const GLfloat Z_BASE_OFFSET = -16.0f;
static GLuint g_textureSequence[MAX_FLOW] = {
    2, 1, 0, 3, 4, 5, 6};

static MGEFF_EFFECTOR effcoverfloweffector_init(MGEFF_EFFECTOR _effector)
{
    EffEffector *effector = (EffEffector *)_effector;
    EffCoverFlowCtxt* coverflow_context = 
        (EffCoverFlowCtxt*)calloc(1, sizeof(EffCoverFlowCtxt));
    effector->context = coverflow_context;
    return _effector;
}

static void effcoverfloweffector_finalize(MGEFF_EFFECTOR _effector)
{
    EffEffector *effector = (EffEffector *)_effector;
    free (effector->context);
}

static void effcoverfloweffector_begindraw(MGEFF_ANIMATION animation, MGEFF_EFFECTOR _effector)
{
    EffEffector *effector = (EffEffector *)_effector;
    EffCoverFlowCtxt *coverflow_context = (EffCoverFlowCtxt *)effector->context;

    int i = 0;
    float start = 0.0f, end = 1.0f;
    HDC source = NULL;
    mGEffAnimationSetStartValue(animation, (const void*)(&start));
    mGEffAnimationSetEndValue(animation, (const void*)(&end));
    mGEffAnimationSetCurve(animation, InQuint);
    mGEffAnimationSetDuration(animation, 500);
    coverflow_context->m_mesaManager = 
        CreateOpenGLManager(effector->sink->hwnd);
    if (NULL == coverflow_context->m_mesaManager) return -1;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustumf(-1.0, 1.0, -1.0, 1.0, 4, 50.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClearColor(.6, .6, .6, 0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glViewport(0, 0, 640, 480);
    if (coverflow_context->m_background != HDC_INVALID &&
            coverflow_context->m_background != NULL)
    {
        coverflow_context->m_backgroundTexture = 
            textureLoadAsHDC(coverflow_context->m_mesaManager, 
                coverflow_context->m_background,
                GL_TEXTURE_2D);
    }
    for (i = 0; HDC_INVALID != source; ++i)
    {
        source = mGEffEffectorGetSourceDC(coverflow_context, i);
        if (HDC_INVALID != source && NULL != source) {
                textureLoadAsHDC(coverflow_context->m_mesaManager,
                        source, GL_TEXTURE_2D);
        }
    }
}

static void effcoverfloweffector_ondraw(MGEFF_ANIMATION animation, MGEFF_EFFECTOR _effector, 
        HDC sink_dc, int id, void* value)
{
    EffEffector *effector = (EffEffector *)_effector;
    EffCoverFlowCtxt *coverflow_context = (EffCoverFlowCtxt *)effector->context;

    GLfloat m_moveDistance = *(float*)value * PANEL_SPACING;
    GLfloat m_mainRotateAngle = DEXTRAL_ANGLE - (*(float*)value) * DEXTRAL_ANGLE;
    GLfloat m_subRotateAngle = (*(float*)value) * DEXTRAL_ANGLE;
    GLfloat m_verticalElevation = MID_PANEL_PITCH - (*(float*)value) * MID_PANEL_PITCH;
    GLfloat m_gapDistance = (*(float*)value) * PANEL_GAP;
    const int half = MAX_FLOW / 2;
    int i = 0;

    const GLfloat rect[] = 
    {
        (-2.0f), (-2.0),0,
        (2.0f), (-2.0), 0,
        (2.0f), (2.0), 0,

        (-2.0f), (-2.0),0,
        (2.0f), (2.0), 0,
        (-2.0f), (2.0f) , 0
    };
    const GLfloat rectuv[] = {
        (0.0f), (0.0f),
        (1.0f), (0.0f),
        (1.0f), (1.0f),

        (0.0f), (0.0f),
        (1.0f), (1.0f),
        (0.0f), (1.0f) };
    if (NULL == coverflow_context->m_mesaManager) return;
    if (HDC_INVALID != sink_dc)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glPushMatrix();
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnableClientState(GL_VERTEX_ARRAY);
        //glTexCoordPointer(2, GL_FLOAT, 5 * sizeof(GLfloat), plane);
        //glVertexPointer(3, GL_FLOAT, 5 * sizeof(GLfloat), plane + 2);
        glTexCoordPointer( 2, GL_FLOAT, 0, rectuv );
        glVertexPointer(3, GL_FLOAT, 0, rect);
        for (i = 0; i < MAX_FLOW; ++i)
        {
            glPushMatrix();
            textureSelect(coverflow_context->m_mesaManager->m_textureID[
                    g_textureSequence[i]], GL_TEXTURE_2D);
            glScalef(0.8, 0.9, 0.8);
            if (half == i)
            {
                if (coverflow_context->m_direction == MGEFF_DIRECTION_RIGHT2LEFT)
                {
                    glTranslatef(-m_gapDistance, 0.0, Z_BASE_OFFSET + m_verticalElevation);
                    glRotatef(m_subRotateAngle, 0.0, 1.0, 0.0);
                }
                else
                {
                    glTranslatef(m_gapDistance, 0.0, Z_BASE_OFFSET + m_verticalElevation);
                    glRotatef(-m_subRotateAngle, 0.0, 1.0, 0.0);
                }
                glDrawArrays(GL_TRIANGLES, 0, 6);
            }
            else if (i < half) // left
            {
                if (coverflow_context->m_direction == MGEFF_DIRECTION_RIGHT2LEFT)
                {
                    glTranslatef((-i * PANEL_SPACING - PANEL_GAP ) - m_moveDistance, 0.0, Z_BASE_OFFSET);
                    glRotatef(DEXTRAL_ANGLE, 0.0, 1.0, 0.0);
                }
                else
                {
                    if (0 == i)
                    {
                        glTranslatef((-i * PANEL_SPACING - PANEL_GAP ) + m_moveDistance * 2, 
                                        0.0, Z_BASE_OFFSET + MID_PANEL_PITCH - m_verticalElevation);
                        glRotatef(m_mainRotateAngle, 0.0, 1.0, 0.0);
                    }
                    else
                    {
                        glTranslatef((-i * PANEL_SPACING - PANEL_GAP ) + m_moveDistance, 0.0, Z_BASE_OFFSET);
                        glRotatef(DEXTRAL_ANGLE, 0.0, 1.0, 0.0);
                    }
                }
                glDrawArrays(GL_TRIANGLES, 0, 6);
            }
            else    // right
            {
                if (coverflow_context->m_direction == MGEFF_DIRECTION_RIGHT2LEFT)
                {
                    if (half + 1 == i)
                    {
                        glTranslatef(((i - half - 1) * PANEL_SPACING + PANEL_GAP) - m_moveDistance * 2,
                                       0.0, Z_BASE_OFFSET + MID_PANEL_PITCH - m_verticalElevation);
                        glRotatef(-m_mainRotateAngle, 0.0, 1.0, 0.0);
                    }
                    else
                    {
                        glTranslatef(((i - half - 1) * PANEL_SPACING + PANEL_GAP) - m_moveDistance, 0.0, Z_BASE_OFFSET);
                        glRotatef(-DEXTRAL_ANGLE, 0.0, 1.0, 0.0);
                    }
                }
                else
                {
                    glTranslatef(((i - half - 1) * PANEL_SPACING + PANEL_GAP) + m_moveDistance, 0.0, Z_BASE_OFFSET);
                    glRotatef(-DEXTRAL_ANGLE, 0.0, 1.0, 0.0);
                }
                glDrawArrays(GL_TRIANGLES, 0, 6);
            }
            glPopMatrix();
        }
        if (0 != coverflow_context->m_backgroundTexture)
        {
            glPushMatrix();
            textureSelect(coverflow_context->m_backgroundTexture, GL_TEXTURE_2D);
            glTranslatef(0.0, 0.0, Z_BASE_OFFSET - 5.0f);
            glScalef(2.63, 2.63, 2.63);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glPopMatrix();
        }
        glPopMatrix();
        if (EGL_FALSE == mgEGLSwapBuffers(coverflow_context->m_mesaManager->m_display, 
                    coverflow_context->m_mesaManager->m_surface)) {
            puts("mgEGLSwapBuffers failed.");
        }
    }
}

static void effcoverfloweffector_enddraw(MGEFF_ANIMATION animation, MGEFF_EFFECTOR _effector)
{
    EffEffector *effector = (EffEffector *)_effector;
    EffCoverFlowCtxt *coverflow_context = (EffCoverFlowCtxt *)effector->context;

    DestroyOpenGLManager(coverflow_context->m_mesaManager);
}

static int effcoverfloweffector_setproperty(MGEFF_EFFECTOR _effector,
        int property_id, int value)
{
    EffEffector *effector = (EffEffector *)_effector;
    EffCoverFlowCtxt *coverflow_context = (EffCoverFlowCtxt *)effector->context;

    switch (property_id)
    {
        case MGEFF_PROPERTY_DIRECTION:
            coverflow_context->m_direction = value;
            return 0;
        case MGEFF_PROPERTY_BACKGROUND:
            if ((HDC)value != HDC_INVALID && (HDC)value != NULL) {
                coverflow_context->m_backgroundTexture =
                    textureLoadAsHDC(coverflow_context->m_mesaManager, 
                            (HDC)value, GL_TEXTURE_2D);
            }
            break;
        default:
            break;
    }
    return -1;
}

MGEFF_EFFECTOROPS glcoverfloweffector = 
{
    MGEFF_MINOR_es_coverflow, 
    MGEFF_FLOAT, 
    effcoverfloweffector_init, 
    effcoverfloweffector_finalize,
    effcoverfloweffector_ondraw,
    effcoverfloweffector_begindraw,
    effcoverfloweffector_enddraw,
    effcoverfloweffector_setproperty,
    NULL
};

#endif

