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

#include <assert.h>
#include <minigui/common.h>

#include "mgeff.h"

#if HAVE_MESA_MINIGUI
#include "gl-cubeturn-effector.h"
#include <GL/gl.h>

const int TURN_ANGLE = 90;

int  gl_initialize(MGEFF_EFFECTOR _effector)
{
    EffEffector * effector = (EffEffector *)_effector;
    EffCubeTurnCtxt *cubeturn_context = (EffCubeTurnCtxt *)effector->context;

    cubeturn_context->m_mesaManager = CreateOpenGLManager(effector->sink->hwnd);
    if (NULL != cubeturn_context->m_mesaManager)
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();   // init current projection matrix.
        glFrustum( -1.0, 1.0, -1.0, 1.0, 2.0, 60.0 );
        glMatrixMode(GL_MODELVIEW); // set projection matrix.
        glLoadIdentity();
        glTranslatef(0, 0, -30.0);    // move object.
        glClearColor(0.01, 0.5, .3, 0);
        cubeturn_context->m_mesaManager->m_textureID[FIRST_KEY_FRAME] = textureLoad(
                cubeturn_context->m_mesaManager, cubeturn_context->m_source1, GL_TEXTURE_2D);
        cubeturn_context->m_mesaManager->m_textureID[LAST_KEY_FRAME] = textureLoad(
                cubeturn_context->m_mesaManager,cubeturn_context->m_source2, GL_TEXTURE_2D);
        return 0;    
    }
    return -1;
}

int  effcubeturneffector_setproperty(MGEFF_EFFECTOR _effector, int property_id, int value)
{
    EffEffector * effector = (EffEffector *)_effector;
    EffCubeTurnCtxt *cubeturn_context = (EffCubeTurnCtxt *)effector->context;

    switch (property_id)
    {
        case MGEFF_PROPERTY_DIRECTION:
            if (value >= MGEFF_CLOCKWISE && 
                    value <= MGEFF_CLOCKINVERSE)
            effector->direction = value;
            return 0;
            break;
        case MGEFF_PROPERTY_AXIS:
            if (value >= MGEFF_AXIS_X &&
                    value <= MGEFF_AXIS_Z)
            cubeturn_context->m_axis = value;
            break;
        default:
            return effbaseeffector_setproperty(_effector, property_id, value);
    }
    return -1;
}

void effcubeturneffector_ondraw(MGEFF_ANIMATION animation, MGEFF_EFFECTOR _effector, 
        HDC sink_dc, int id, void* value)
{
    assert(0 != sink_dc);

    EffEffector * effector = (EffEffector *)_effector;
    EffCubeTurnCtxt *cubeturn_context = (EffCubeTurnCtxt *)effector->context;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
	glPushMatrix();
	if (cubeturn_context->m_axis == MGEFF_AXIS_X) {
        glRotatef(*((int*)value) * TURN_ANGLE, 0.0f, 1.0f, 0.0f);
    }
    else if (cubeturn_context->m_axis == MGEFF_AXIS_Y) {
        glRotatef(*((int*)value) * TURN_ANGLE, 1.0f, 0.0f, 0.0f);
    }
    textureSelect(cubeturn_context->m_mesaManager->m_textureID[FIRST_KEY_FRAME], GL_TEXTURE_2D);
	glBegin(GL_QUADS);   
    
	glColor3f(1.0,0.0,0.0);   
	glTexCoord2f(1.0,0.0);glVertex3f( 10.0f, 10.0f,10.0f);   //上面
	glTexCoord2f(0.0,0.0);glVertex3f(-10.0f, 10.0f,10.0f);         
	glTexCoord2f(0.0,1.0);glVertex3f(-10.0f,-10.0f,10.0f);   
	glTexCoord2f(1.0,1.0);glVertex3f( 10.0f,-10.0f,10.0f);   
    glEnd();

    textureSelect(cubeturn_context->m_mesaManager->m_textureID[LAST_KEY_FRAME], GL_TEXTURE_2D);
	glBegin(GL_QUADS);
    glColor3f(0.0,1.0,0.0);  
	glTexCoord2f(1.0,0.0);glVertex3f(10.0, 10.0,-10.0);		//右面
	glTexCoord2f(0.0,0.0);glVertex3f(10.0, 10.0, 10.0);   
	glTexCoord2f(0.0,1.0);glVertex3f(10.0,-10.0, 10.0);   
	glTexCoord2f(1.0,1.0);glVertex3f(10.0,-10.0,-10.0);   
    
	glColor3f(0.0,0.0,1.0);//蓝色   
	glTexCoord2f(1.0,0.0);glVertex3f( 10.0,-10.0,-10.0);   //下面
	glTexCoord2f(0.0,0.0);glVertex3f(-10.0,-10.0,-10.0);   
	glTexCoord2f(0.0,1.0);glVertex3f(-10.0, 10.0,-10.0);   
	glTexCoord2f(1.0,1.0);glVertex3f( 10.0, 10.0,-10.0);                  
	
    glColor3f(1.0,1.0,0.0);//   黄色
	glTexCoord2f(1.0,0.0);glVertex3f(-10.0, 10.0, 10.0);   //左面
	glTexCoord2f(0.0,0.0);glVertex3f(-10.0, 10.0,-10.0);          
	glTexCoord2f(0.0,1.0);glVertex3f(-10.0,-10.0,-10.0);   
	glTexCoord2f(1.0,1.0);glVertex3f(-10.0,-10.0, 10.0);   
    
	glColor3f(0.0,1.0,1.0);   
	glTexCoord2f(1.0,0.0);glVertex3f( 10.0,10.0,-10.0);		//后面
	glTexCoord2f(0.0,0.0);glVertex3f(-10.0,10.0,-10.0);           
	glTexCoord2f(0.0,1.0);glVertex3f(-10.0,10.0, 10.0);   
	glTexCoord2f(1.0,1.0);glVertex3f( 10.0,10.0, 10.0);   
    
	glColor3f(1.0,0.0,1.0);  
	glTexCoord2f(1.0,0.0);glVertex3f( 10.0,-10.0, 10.0);     //前面
	glTexCoord2f(0.0,0.0);glVertex3f(-10.0,-10.0, 10.0);   
	glTexCoord2f(0.0,1.0);glVertex3f(-10.0,-10.0,-10.0);   
	glTexCoord2f(1.0,1.0);glVertex3f( 10.0,-10.0,-10.0);   
	glEnd(); 

	glPopMatrix();
    mgMesaSwapBuffers(sink_dc);
}

MGEFF_EFFECTOR effcubeturneffector_init(MGEFF_EFFECTOR _effector)
{
    EffEffector * effector = (EffEffector *)_effector;

    EffCubeTurnCtxt* cubeturn_context = 
        (EffCubeTurnCtxt*)calloc(1, sizeof(EffCubeTurnCtxt));
    effector->context = cubeturn_context;
    return _effector;
}

void effcubeturneffector_finalize(MGEFF_EFFECTOR _effector)
{
    EffEffector * effector = (EffEffector *)_effector;
    free (effector->context);
}

void effcubeturneffector_begindraw(MGEFF_ANIMATION animation, MGEFF_EFFECTOR _effector)
{
    EffEffector * effector = (EffEffector *)_effector;
    EffCubeTurnCtxt *cubeturn_context = (EffCubeTurnCtxt *)effector->context;

    cubeturn_context->m_source1 = 
        (EffHDCSource *)(effector->source_list.next);
    cubeturn_context->m_source2 = 
        (EffHDCSource *)(cubeturn_context->m_source1->list.next);
    gl_initialize(_effector);
}

void effcubeturneffector_enddraw(MGEFF_ANIMATION animation, MGEFF_EFFECTOR _effector)
{
    EffEffector * effector = (EffEffector *)_effector;
    EffCubeTurnCtxt *cubeturn_context = (EffCubeTurnCtxt *)effector->context;

    DestroyOpenGLManager(((EffCubeTurnCtxt*)effector)->m_mesaManager);
}

#erro
MGEFF_EFFECTOROPS glcubeturneffector = 
{
    MGEFF_MINOR_es_cubicrotate, 
    MGEFF_FLOAT, 
    effcubeturneffector_init, 
    effcubeturneffector_finalize,
    effcubeturneffector_ondraw,
    effcubeturneffector_begindraw,
    effcubeturneffector_enddraw,
    effcubeturneffector_setproperty,
    NULL
};

#endif
