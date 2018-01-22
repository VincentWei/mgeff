/*! ============================================================================
 * \file gl-cubeturn-effector.h 
 * \brief  
 * \author DongKai
 * \version 1.0
 * \date 03/03/2010 08:41:06 AM 
 *
 *  Company: Beijing Feynman Software Technology Co., Ltd.
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

