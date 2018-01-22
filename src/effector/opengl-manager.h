/*!============================================================================
 * \file mesa-manager.h 
 * \brief Mesa3D manager.
 * \author DongKai
 * \version 1.0
 * \date 03/04/2010 08:06:44 AM 
 *
 *  Company: Beijing Feynman Software Technology Co., Ltd.
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

