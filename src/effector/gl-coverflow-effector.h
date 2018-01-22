/*!============================================================================
 * \file gl-coverflow-effector.h 
 * \brief  
 * \author DongKai
 * \version 1.0
 * \date 03/04/2010 07:31:32 PM 
 *
 *  Company: Beijing Feynman Software Technology Co., Ltd.
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
