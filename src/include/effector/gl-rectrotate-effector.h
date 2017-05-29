/* 
 * $Id: gl-rectrotate-effector.h 1389 2010-07-01 07:29:08Z huzhaolin $
 *
 * gl-rect-rotate.h: For gl-rect-rotate.c.
 *
 * Copyright (C) 2010 Feynman Software.
 *
 * Author: rlzheng
 *
 * License: GPL
 */

#ifndef _EFF_GL_RECT_ROTATE_H
#define _EFF_GL_RECT_ROTATE_H
#include "effector.h"
#include "opengl-manager.h"
typedef struct _EffGLRectRotateCtxt{
    unsigned int m_texture_id[2];
    float m_angle;
    float m_x;
    float m_y;
    float m_z;
    OpenGLManager* mesa_manager;
}EffGLRectRotateCtxt;

MGEFF_EFFECTOR effglrectrotateeffector_init (MGEFF_EFFECTOR _effector);
void effglrectrotateeffector_finalize (MGEFF_EFFECTOR _effector);
int  effglrectrotateeffector_setproperty(MGEFF_EFFECTOR _effector,
        int property_id, int value);
void effglrectrotateeffector_ondraw(MGEFF_ANIMATION animation,
        MGEFF_EFFECTOR _effector, HDC sink_dc, int id, void* value);
void effglrectrotateeffector_begindraw(MGEFF_ANIMATION animation,
        MGEFF_EFFECTOR _effector);
void effglrectrotateeffector_enddraw(MGEFF_ANIMATION animation,
        MGEFF_EFFECTOR _effector);
#endif
