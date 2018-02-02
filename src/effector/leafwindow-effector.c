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
#include "mgeff.h"
#include "eff-common.h"
#include "leafwindow-effector.h"

#ifdef _MGEFF_LEAFWINDOWEFFECTOR
int  effleafceffector_setproperty(MGEFF_EFFECTOR _effector, int property_id, int value)
{
    EffEffector *effector = (EffEffector *)_effector;
    EffLeafCtxt* leaf_context = (EffLeafCtxt*)effector->context;

    switch (property_id)
    {
        case MGEFF_PROPERTY_LEAFVERTICAL:
            if (value > 0) {
                leaf_context->vertical = TRUE;
            }
            else {
                leaf_context->vertical = FALSE;
            }
            return 0;
        case MGEFF_PROPERTY_DIRECTION:
            effector->direction = value;
            return 0;
        case MGEFF_PROPERTY_LEAFROWS:
            //assert(0 != value);
            if (0 == value) {
                leaf_context->nrows = 5;
                return -1;
            }
            leaf_context->nrows= value;
            return 0;
        default:
            return effbaseeffector_setproperty(_effector, property_id, value);
    }
    return -1;
}

int  effleafceffector_getproperty(MGEFF_EFFECTOR _effector, int property_id, int* pValue)
{
    EffEffector *effector = (EffEffector *)_effector;
    EffLeafCtxt* leaf_context = (EffLeafCtxt*)effector->context;

    switch (property_id)
    {
        case MGEFF_PROPERTY_LEAFVERTICAL:
            *pValue = leaf_context->vertical;
            return 0;
        case MGEFF_PROPERTY_DIRECTION:
            *pValue = effector->direction;
            return 0;
        case MGEFF_PROPERTY_LEAFROWS:
            *pValue = leaf_context->nrows;
            return 0;
        default:
            return effbaseeffector_getproperty(_effector, property_id, pValue);
    }
    return -1;
}

void effleafeffector_ondraw(MGEFF_ANIMATION animation, MGEFF_EFFECTOR _effector, 
        HDC sink_dc, intptr_t id, void* value)
{
    EffEffector *effector = (EffEffector *)_effector;
    EffLeafCtxt* leaf_context = (EffLeafCtxt*)effector->context;

    EffHDCSource *source = (EffHDCSource *) (effector->source_list.next);
    RECT rc1;
    RECT rc_sink;

    source = (EffHDCSource *) (source->list.next);
    
    effbaseeffector_rect(source->hdc, &rc1);
    effbaseeffector_rect(sink_dc, &rc_sink);

    _MGEFF_PRINTF("%s:value = %f\n", __FUNCTION__, *(float*)value);
    //printf("%s:value = %f\n", __FUNCTION__, *(float*)value);

    if (sink_dc != HDC_INVALID) {
        int i = 0;
#if 0
        int cur_height = 0; 
        int cur_width  = 0; 
        int width  =  RECTW(rc1);
        int height = RECTH(rc1);
        int m_heightPerLine = height / leaf_context->nrows;    
        int m_widthPerLine = width / leaf_context->nrows;    
#else
        int cur_v = 0;
        int m_valuePerRow = 0;
#endif
        int v = 0;
        int last = 0;
        int nrows = leaf_context->nrows;

        assert(NULL != source);
        if (leaf_context->vertical) {
            m_valuePerRow = RECTH(rc1)/ leaf_context->nrows;
            /*if (RECTH(rc1)% leaf_context->nrows) {
                nrows++;
            }*/
            last = RECTH(rc1) % leaf_context->nrows;
        }
        else {
            m_valuePerRow = RECTW(rc1)/ leaf_context->nrows;
            /*if (RECTW(rc1)% leaf_context->nrows) {
                nrows++;
            }*/
            last = RECTW(rc1) % leaf_context->nrows;
        }
        for (i = 0; i < nrows; i++){
            v = m_valuePerRow * (*(float*)value);
            if (leaf_context->vertical) {
                if (effector->direction == 1) {
                    cur_v = m_valuePerRow * (1 + i) - v;
                }
                else {
                    cur_v = m_valuePerRow * i;
                }
                if (v == 0) break;
                //printf("start pos is %d, height is %d\n", m_valuePerRow * i, v);
                BitBlt(source->hdc, rc1.left, rc1.top + 
                        m_valuePerRow * i, RECTW(rc1), v,
                        sink_dc, 0, cur_v, 0);
            }
            else {
                if (effector->direction == 1) {
                    cur_v = m_valuePerRow * (1 + i) - v;
                }
                else {
                    cur_v = m_valuePerRow * i;
                }
                if (v == 0) break;
                //printf("start pos is %d, width is %d\n", m_valuePerRow * i, v);
                BitBlt(source->hdc, rc1.left + m_valuePerRow * i, rc1.top,
                        v, RECTH(rc1),
                        sink_dc, cur_v, 0, 0);
            }
        }

        /* deal with the last one */
        if ( last > 0 && 0 != i ) {
            v = last * (*(float*)value);
            if (leaf_context->vertical) {
                if (effector->direction == 1) {
                    cur_v = m_valuePerRow * (1 + i) - v;
                }
                else {
                    cur_v = m_valuePerRow * i;
                }
                if ( 0 != v ) {
                    BitBlt(source->hdc, rc1.left, rc1.top + m_valuePerRow * i, RECTW(rc1), v,
                            sink_dc, 0, cur_v, 0);
                }
            }
            else {
                if (effector->direction == 1) {
                    cur_v = m_valuePerRow * (1 + i) - v;
                }
                else {
                    cur_v = m_valuePerRow * i;
                }
                if ( 0 != v ) {
                    BitBlt(source->hdc, rc1.left + m_valuePerRow * i, rc1.top,v, RECTH(rc1),
                            sink_dc, cur_v, 0, 0);
                }
            }
        } /* end if ( 0 != last) */
    } /* end if (sink_dc != HDC_INVALID) */
}


MGEFF_EFFECTOR effleafeffector_init(MGEFF_EFFECTOR _effector)
{
    EffEffector *effector = (EffEffector *)_effector;
    EffLeafCtxt* leaf_context =
        (EffLeafCtxt*)calloc(1, sizeof(EffLeafCtxt));
    leaf_context->nrows = 5;
    leaf_context->vertical = TRUE;
    //leaf->vertical = FALSE;
    effector->context = leaf_context;
    return _effector;
}

void effleafeffector_finalize(MGEFF_EFFECTOR _effector)
{
    EffEffector *effector = (EffEffector *)_effector;
    free (effector->context);
}

MGEFF_EFFECTOROPS leafwindoweffector = 
{ 
    MGEFF_MINOR_leafwindow, 
    MGEFF_FLOAT, 
    effleafeffector_init,
    effleafeffector_finalize,
    effleafeffector_ondraw,
    NULL,
    NULL,
    effleafceffector_setproperty,
    effleafceffector_getproperty
};
#endif
