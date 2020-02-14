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

#include "mgeff.h"
#include "eff-common.h"
#include "common-effector.h"
#include "effector.h"

#ifdef _MGEFF_ROTATEEFFECTOR
#if defined(HAVE_MESA_MINIGUI) || defined(HAVE_EGL_SUPPORT)
#if HAVE_MESA_MINIGUI
#include <GL/gl.h>
#include <GL/glu.h>
#elif HAVE_EGL_SUPPORT
#include <EGL/egl.h>
#include <EGL/eglext.h>
#endif
#include <assert.h>
#include "gl-rectrotate-effector.h"

#define dbg() printf("debug:------------%s, %d---------\n", __FUNCTION__, __LINE__);

static MGEFF_EFFECTOR effglrectrotateeffector_init(MGEFF_EFFECTOR _effector)
{
    EffEffector *effector = (EffEffector *)_effector;
    EffGLRectRotateCtxt* rect_effector = 
        (EffGLRectRotateCtxt*)calloc(1, sizeof(EffGLRectRotateCtxt));
    effector->context = rect_effector;
    return _effector;
}

static void effglrectrotateeffector_finalize(MGEFF_EFFECTOR _effector)
{
    EffEffector *effector = (EffEffector *)_effector;
    free (effector->context);
}

static void effglrectrotateeffector_ondraw(MGEFF_ANIMATION animation,
        MGEFF_EFFECTOR _effector, HDC sink_dc, int id, void* value)
{
    EffEffector *effector = (EffEffector *)_effector;
    EffGLRectRotateCtxt *rect_effector = (EffGLRectRotateCtxt*)effector->context;

    static float current_angle = 0.0;
    float position_x = 0.0, position_y = 0.0;
    float size_x = 1.0, size_y = 1.0;
    RECT screen_rc;
    RECT *rc = (RECT *)value;

    /* Get rect size */
    __mgeffbaseeffector_rect(HDC_SCREEN, &screen_rc);
    size_x = (float)(RECTWP(rc))/(RECTW(screen_rc));
    size_y = (float)(RECTHP(rc))/(RECTH(screen_rc));
    position_x = (float)(rc->left)/(RECTW(screen_rc));
    position_y = (float)(rc->top)/(RECTH(screen_rc));

    /* Calculate the current angle */
    current_angle = (int)(current_angle +rect_effector->m_angle) %360;

    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    /* select last texture */
    textureSelect(rect_effector->m_texture_id[0], GL_TEXTURE_2D);

    /* gl rotate start */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    glTranslatef(position_x, position_y, 0.0);
    /* Rotate Y axis in default */
    glRotatef (current_angle, rect_effector->m_x, rect_effector->m_y, rect_effector->m_z);
    glScalef(size_x, size_y, 0.0);
    glTranslatef(-position_x, -position_y, 0.0);

#if HAVE_MESA_MINIGUI
    /* gl render texture */
    glBegin(GL_QUADS);
        glTexCoord2f(0.0, 1.0); glVertex3f(-6.0, -6.0, 0.0);
        glTexCoord2f(1.0, 1.0); glVertex3f(6.0, -6.0, 0.0);
        glTexCoord2f(1.0, 0.0); glVertex3f(6.0, 6.0, 0.0);
        glTexCoord2f(0.0, 0.0); glVertex3f(-6.0, 6.0, 0.0);
    glEnd();
#elif HAVE_EGL_SUPPORT
#endif
    glPopMatrix();

#if HAVE_MESA_MINIGUI
    mgMesaSwapBuffers(effector->sink->hdc);
#elif HAVE_EGL_SUPPORT
    {
        mgEGLSwapBuffers(rect_effector->mesa_manager->m_display, rect_effector->mesa_manager->m_surface);
    }
#endif

    /* gl rotate end */
    glDisable(GL_TEXTURE_2D);
}

static void effglrectrotateeffector_begindraw(MGEFF_ANIMATION animation, MGEFF_EFFECTOR _effector)
{
    EffEffector *effector = (EffEffector *)_effector;
    EffGLRectRotateCtxt *rect_effector = (EffGLRectRotateCtxt*)effector->context;

    EffHDCSource *source1 = (EffHDCSource *)(effector->source_list.next);
    EffHDCSource *source2 = (EffHDCSource *)(source1->list.next);
    OpenGLManager* mesa_manager = CreateOpenGLManager(effector->sink->hwnd);
    rect_effector->mesa_manager = mesa_manager;

    RECT screen_rc;
    __mgeffbaseeffector_rect(HDC_SCREEN, &screen_rc);
    
    RECT start_rc = {0, 0, RECTW(screen_rc), RECTH(screen_rc)};
    RECT end_rc = {0, 0, 0, 0};
    mGEffAnimationSetStartValue(animation, &start_rc);
    mGEffAnimationSetEndValue(animation, &end_rc);
    
    /* init opengl */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
#if HAVE_MESA_MINIGUI
    glFrustum( -1.0, 1.0, -1.0, 1.0, 2.0, 60.0 );
#endif
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(-9.0, -9.0, -30.0);

    glEnable(GL_ALPHA_TEST);
    glEnable(GL_BLEND);

    glClearColor(0, 0, 0, 0);
    
    /* load texture */
    rect_effector->m_texture_id[0] = textureLoad(mesa_manager, source1, GL_TEXTURE_2D);
    rect_effector->m_texture_id[1] = textureLoad(mesa_manager, source2, GL_TEXTURE_2D);
    
    rect_effector->m_angle = 59.0;
    rect_effector->m_x = 0.0;
    rect_effector->m_y = 1.0;
    rect_effector->m_z = 0.0;
}

static void effglrectrotateeffector_enddraw(MGEFF_ANIMATION animation, MGEFF_EFFECTOR _effector)
{
    /* Animation end */
    EffEffector *effector = (EffEffector *)_effector;
    EffGLRectRotateCtxt *rect_effector = (EffGLRectRotateCtxt*)effector->context;
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    textureSelect(rect_effector->m_texture_id[1], GL_TEXTURE_2D);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    glRotatef (0, rect_effector->m_x, rect_effector->m_y, rect_effector->m_z);

#if HAVE_MESA_MINIGUI
    glBegin(GL_QUADS);
        glTexCoord2f(0.0, 1.0); glVertex3f(-6.0, -6.0, 0.0);
        glTexCoord2f(1.0, 1.0); glVertex3f(6.0, -6.0, 0.0);
        glTexCoord2f(1.0, 0.0); glVertex3f(6.0, 6.0, 0.0);
        glTexCoord2f(0.0, 0.0); glVertex3f(-6.0, 6.0, 0.0);
    glEnd();
#elif HAVE_EGL_SUPPORT
#endif
    glPopMatrix();
#if HAVE_MESA_MINIGUI
    mgMesaSwapBuffers(effector->sink->hdc);
#elif HAVE_EGL_SUPPORT
    {
        mgEGLSwapBuffers(rect_effector->mesa_manager->m_display, rect_effector->mesa_manager->m_surface);
    }
#endif
    glDisable(GL_TEXTURE_2D);
    
    DestroyOpenGLManager(rect_effector->mesa_manager);
}

static void set_dirction(MGEFF_EFFECTOR _effector, float x, float y, float z)
{
    EffEffector *effector = (EffEffector *)_effector;
    EffGLRectRotateCtxt *rect_effector = (EffGLRectRotateCtxt*)effector->context;

    rect_effector->m_x = x;
    rect_effector->m_y = y;
    rect_effector->m_z = z;
}

static int effglrectrotateeffector_setproperty(MGEFF_EFFECTOR _effector,
        int property_id, int value)
{
    int axis = 1, direction = 1;

    switch (property_id)
    {
        case MGEFF_PROPERTY_AXIS:
            {
                switch (value)
                {
                    case MGEFF_AXIS_X:
                        axis = 1;
                        break;
                    case MGEFF_AXIS_Y:
                        axis = 2;
                        break;
                    case MGEFF_AXIS_Z:
                        axis = 3;
                        break;
                    default:
                        break;
                }
            }
            break;
        case MGEFF_PROPERTY_DIRECTION:
            {
                switch (value)
                {
                    case MGEFF_CLOCKWISE:
                        direction = 1;
                        break;
                    case MGEFF_CLOCKINVERSE:
                        direction = -1;
                        break;
                    default:
                        break;
                }
            }
            break;
        default:
            return __mgeffbaseeffector_setproperty(_effector, property_id, value);
    }
    if (axis == 1) {
        set_dirction(_effector, 1.0 *direction, 0.0, 0.0);
    } else if (axis == 2) {
        set_dirction(_effector, 0.0, 1.0 *direction, 0.0);
    } else {
        set_dirction(_effector, 0.0, 0.0, 1.0 *direction);
    }

    return -1;
}

MGEFF_EFFECTOROPS glrectrotateeffector = 
{
    MGEFF_MINOR_es_rectrotate,
    MGEFF_RECT, 
    effglrectrotateeffector_init, 
    effglrectrotateeffector_finalize,
    effglrectrotateeffector_ondraw,
    effglrectrotateeffector_begindraw,
    effglrectrotateeffector_enddraw,
    effglrectrotateeffector_setproperty,
    NULL
};
#endif
#endif /* _MGEFF_ROTATEEFFECTOR */
