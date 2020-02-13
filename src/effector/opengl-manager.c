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
#include <string.h>
#include <minigui/common.h>

#include "mgeff.h"

#if defined(HAVE_MESA_MINIGUI) || defined(HAVE_EGL_SUPPORT)

#include "opengl-manager.h"
#include "effector.h"

static GLuint s_textureCounter = 0;

#if HAVE_EGL_SUPPORT
BOOL mgEGLinitialize(OpenGLManager* gl_man)
{
    EGLint majorVersion;
    EGLint minorVersion;
    EGLConfig config;
    EGLint numConfigs;
    const EGLint configAttribs[] =
    {
        EGL_RED_SIZE,       8,
        EGL_GREEN_SIZE,     8,
        EGL_BLUE_SIZE,      8,
        EGL_ALPHA_SIZE,     8,
        EGL_DEPTH_SIZE,     32,
        EGL_STENCIL_SIZE,   EGL_DONT_CARE,
        EGL_SURFACE_TYPE,   EGL_PIXMAP_BIT,
        EGL_NONE
    };

    puts("EGL initialize.");
    gl_man->m_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (EGL_NO_DISPLAY != gl_man->m_display) {
        if (EGL_TRUE == eglInitialize(gl_man->m_display, &majorVersion, &minorVersion)) {
            if (EGL_TRUE == eglGetConfigs(gl_man->m_display, NULL, 0, &numConfigs)) {
                if (EGL_TRUE == eglChooseConfig(gl_man->m_display, configAttribs, &config, 1, &numConfigs)) {
                    gl_man->m_context = eglCreateContext(gl_man->m_display, config, NULL, NULL);
                    if (EGL_NO_CONTEXT != gl_man->m_context) {
                        gl_man->m_surface = mgEGLCreateWindowSurface(gl_man->m_display, config, gl_man->m_hwnd, configAttribs);
                        if (EGL_NO_SURFACE != gl_man->m_surface) {
                            return mgEGLMakeCurrent(gl_man->m_display, gl_man->m_surface, gl_man->m_surface, gl_man->m_context);
                        }else fprintf(stderr, "mgEGLCreateWindowSurface failed.");
                    }else fprintf(stderr, "eglCreateContext failed.");
                }else fprintf(stderr, "eglChooseConfig failed.");
            }else fprintf(stderr, "eglGetConfigs failed.");
        } else fprintf(stderr, "eglInitialize failed.");
    } else fprintf(stderr, "eglGetDisplay failed.");
    assert(0);
    return TRUE;
}

void mgEGLfinalize(OpenGLManager* gl_man)
{
    if (EGL_NO_DISPLAY != gl_man->m_display) {
        eglMakeCurrent(gl_man->m_display , NULL, NULL, NULL);
        mgEGLDestroySurface(gl_man->m_display, gl_man->m_surface);
        eglDestroyContext(gl_man->m_display, gl_man->m_context);
        eglTerminate(gl_man->m_display);
    }
}

#endif

BOOL Mesa3DInitialize(OpenGLManager* gl_man)
{
    return FALSE;
}

OpenGLManager* CreateOpenGLManager(HWND hwnd)
{
    size_t width = 0, height = 0;
    if (HWND_INVALID != hwnd)
    {
        HDC hdc = GetDC(hwnd);
        RECT wnd_rc;
        GetClientRect(hwnd, &wnd_rc);
        width  = RECTW(wnd_rc);
        height = RECTH(wnd_rc);
        if (0 != width && 0 != height)
        {
            OpenGLManager *mesaman = (OpenGLManager*)malloc(sizeof(OpenGLManager));
            if (NULL != mesaman)
            {
                mesaman->m_hdc = hdc;
                mesaman->m_hwnd = hwnd;
                mesaman->m_attrModifier = NULL;
#if HAVE_MESA_MINIGUI
                mesaman->m_context = mgMesaCreateContext(mesaman->m_hdc, NULL, GL_TRUE, GL_TRUE, GL_FALSE);
#endif

#if defined(HAVE_EGL_SUPPORT)
                if (!mgEGLinitialize(mesaman)) {
                    free(mesaman);
                    return NULL;
                }
#endif

                if (NULL != mesaman->m_context)
                {
#if HAVE_MESA_MINIGUI
                    mgMesaMakeCurrent(mesaman->m_context, mesaman->m_hdc);
#endif
                    glViewport(0, 0, width, height); 
                    memset(mesaman->m_textureID, 0, sizeof(mesaman->m_textureID));
                    memset(mesaman->m_textures, 0, sizeof(mesaman->m_textures));
                    glClearColor(0.0, 0.0, 0.0, 0.0);
                    glClearDepthf(1.0f);
                    glGenTextures(MAX_TEXTURES, mesaman->m_textureID);
                    return (OpenGLManager*)mesaman;
                }
                else {
                    free(mesaman);
                    return NULL;
                }
            }
        }
    }

    return NULL;
}

void DestroyOpenGLManager(OpenGLManager* mesaman)
{
    int index;
    texturesUnload(mesaman);
    for (index = 0; index < MAX_TEXTURES; ++index)
    {    
        if (NULL != mesaman->m_textures[index])
        {
            free(mesaman->m_textures[index]->m_buffer);
            free(mesaman->m_textures[index]);
        }
    }
#if HAVE_MESA_MINIGUI
    mgMesaMakeCurrent(NULL, 0);
    mgMesaDestroyContext(mesaman->m_context);
#elif HAVE_EGL_SUPPORT
    mgEGLfinalize(mesaman);
#endif
    free(mesaman);
}

GLuint  textureLoadAsHDC(OpenGLManager* mesaman, HDC hdc, GLenum target)
{
    if (HDC_INVALID != hdc && NULL != mesaman)
    {
        RECT rect;
        int width = 0;
        int height = 0;
        size_t len = 0;
        unsigned char r, g, b, a;
        int x, y;
        effbaseeffector_rect(hdc, &rect);
        width = RECTW(rect);
        height = RECTH(rect);
        len = width * height * BYTE_PER_RGBA_PIXEL;
        if (0 != len && MAX_TEXTURES > s_textureCounter)
        {
            TextureInformation * tex_info = (TextureInformation*)malloc(sizeof(TextureInformation));
            if (NULL != tex_info)
            {
                tex_info->m_buffer = (unsigned char *)malloc(len);
                if (NULL != tex_info->m_buffer)
                {
                    for (y = 0; y < height; ++y)
                    {
                        for (x = 0; x < width;  ++x)
                        {
                            int pos = (width * y + x) * BYTE_PER_RGBA_PIXEL;
                            Pixel2RGBA(hdc, GetPixel(hdc, x, y), &r, &g, &b, &a);
                            tex_info->m_buffer[pos] = r;
                            tex_info->m_buffer[pos + 1] = g;
                            tex_info->m_buffer[pos + 2] = b;
                            tex_info->m_buffer[pos + 3] = a;
                        }
                    }
                    tex_info->m_width = width;
                    tex_info->m_height =height; 
                    tex_info->m_target = target;
                    printf("will create %d texture.\n", mesaman->m_textureID[s_textureCounter]);
                    glBindTexture(GL_TEXTURE_2D, mesaman->m_textureID[s_textureCounter]);
                    if (NULL != mesaman->m_attrModifier)
                    {
                        mesaman->m_attrModifier->setTextureParamter(tex_info);
                    }
                    else
                    {
                        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
                        switch (target)
                        {
#ifndef HAVE_EGL_SUPPORT
                            case GL_TEXTURE_1D:
                                break;
#endif

#ifdef HAVE_EGL_SUPPORT
                            case GL_TEXTURE_2D:
                                glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
                                glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
                                glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
                                glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
                                if (NULL != tex_info->m_buffer) {
                                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_info->m_width,
                                            tex_info->m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_info->m_buffer);
                                }
                                glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

                                glEnable(GL_TEXTURE_2D);
                                break;
#elif defined(HAVE_MESA_MINIGUI)
                            case GL_TEXTURE_2D:
                                glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
                                glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
                                glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
                                glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
                                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_info->m_width,
                                        tex_info->m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_info->m_buffer);
                                glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
                                glEnable(GL_TEXTURE_2D);
                                break;
#endif

#ifndef HAVE_EGL_SUPPORT
                            case GL_TEXTURE_3D:
                                break;
#endif
                            default:
                                assert(0);
                                break;
                        }
                    }
                    mesaman->m_textures[s_textureCounter] = tex_info;
                    GLuint name = mesaman->m_textureID[s_textureCounter++];
                    printf("now we have %d textures, last is %d..\n", s_textureCounter, name);
                    return name;
                }
                else
                {
                    free(tex_info);
                }
            }
        }
    }
    return 0;
}

GLuint textureLoad(OpenGLManager* mesaman, EffHDCSource *src, GLenum target/* = GL_TEXTURE_2D*/)
{
    if (NULL != mesaman && NULL != src) {
        return textureLoadAsHDC(mesaman, src->hdc, target);
    }
    return -1;
}

void texturesUnload(OpenGLManager* mesaman)
{
    glDeleteTextures(MAX_TEXTURES, mesaman->m_textureID);
}

void textureSelect(GLuint index, GLenum target)
{
    glBindTexture(target, index);
}

TextureAttribModifier * replaceAttrModifier(OpenGLManager* mesaman, TextureAttribModifier * ptr)
{
    TextureAttribModifier * prev = mesaman->m_attrModifier;
    mesaman->m_attrModifier = ptr;
    return prev;
}

#endif
