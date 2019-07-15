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
#ifndef  MESA_MANAGER_INC
#define  MESA_MANAGER_INC

#if defined(HAVE_MESA_MINIGUI) || defined(HAVE_EGL_SUPPORT)

#include <minigui/gdi.h>

#if HAVE_MESA_MINIGUI
#include <GL/mgmesa.h>
#else
#include <mgegl/mgegl.h>
#include <GLES/gl.h>
#endif

enum {
    MAX_TEXTURES = 48,
    BYTE_PER_RGBA_PIXEL = 4,
};

typedef struct _TextureInformation
{
    GLsizei m_width;
    GLsizei m_height;
    GLenum  m_type;
    GLenum  m_target;
    GLint   m_border;
    unsigned char* m_buffer;
    GLint   m_internalFormat;
    GLenum  m_format;
} TextureInformation;

typedef struct _TextureAttribModifier
{
    void (*setTextureParamter)(TextureInformation * texinfo);
}TextureAttribModifier;

typedef struct _OpenGLManager
{
    HDC             m_hdc;
    HWND            m_hwnd;
#if HAVE_EGL_SUPPORT
    EGLContext m_context;
    EGLDisplay m_display;
    mgEGLSurface m_surface;
#elif defined(HAVE_MESA_MINIGUI)
    mgMesaContext   m_context;
#else
    void * m_context;
#endif
    GLuint     m_textureID[MAX_TEXTURES];
    TextureInformation *m_textures[MAX_TEXTURES];
    TextureAttribModifier          *m_attrModifier;
} OpenGLManager;

OpenGLManager* CreateOpenGLManager(HWND);
void DestroyOpenGLManager(OpenGLManager* mesa);
GLuint  textureLoad(OpenGLManager* mesaman, EffHDCSource* img, GLenum target/* = GL_TEXTURE_2D*/);
void    texturesUnload(OpenGLManager* mesa);
void    textureSelect(GLuint index, GLenum target/* = GL_TEXTURE_2D*/);

TextureAttribModifier * replaceAttrModifier(OpenGLManager* mesa, TextureAttribModifier * ptr);
#endif

#endif   /* ----- #ifndef MESA_MANAGER_INC  ----- */

