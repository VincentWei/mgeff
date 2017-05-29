/*!============================================================================
 * \file mgeff-effector-sink.h
 * \brief Effecr sink headfile.
 * \author WanZheng
 * \version 1.0
 * \date 02/03/2010 05:22:39 PM
 *
 *  Company: Beijing Feynman Software Technology Co., Ltd.
 */
#ifndef _MGEFF_EFFECTORSINK_H_
#define _MGEFF_EFFECTORSINK_H_

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

/**
 * \brief The handle to the effector sink.
 */
MGEFF_DECLARE_HANDLE(MGEFF_SINK);

/**
 * \enum MGEFF_SINK_TYPE
 * \brief sink type
 */
enum MGEFF_SINK_TYPE{
    MGEFF_SINK_HDC  = 0,
    MGEFF_SINK_HWND = 1,
    MGEFF_SINK_MAX
};

/**
 * \fn MGEFF_SINK mGEffCreateHDCSink(HDC hdc)
 * \brief Create sink from a DC.
 *
 * This function creates sink ready to be used by a effector from a DC
 * specified by \a hdc.
 *
 * \param hdc the underlying DC of the sink to be created.
 *
 * \return the handle of the created sink.
 *
 * \note sink can be considered as a canvas on which the effector draws.
 *
 * \sa mGEffCreateHDCSink.
 */
MGEFF_EXPORT MGEFF_SINK mGEffCreateHDCSink(HDC hdc);

/**
 * \fn MGEFF_SINK mGEffCreateHwndSink(HWND hwnd)
 * \brief Create sink from a window handle.
 *
 * This function creates sink ready to be used by a effector from a window
 * handle specified by \a hwnd.
 *
 * \param hwnd the underlying window of the sink to be created.
 *
 * \return the handle of the created sink.
 *
 * \note sink can be considered as a canvas on which the effector draws.
 *
 * \sa mGEffCreateHDCSink, mGEffEffectorCreateEx.
 */
MGEFF_EXPORT MGEFF_SINK mGEffCreateHwndSink(HWND hwnd);

/**
 * \fn void mGEffDeleteSink(MGEFF_SINK sink)
 * \brief Deletes a sink.
 *
 * This function deletes a sink specified by \a sink.
 *
 * \param sink The handle of the sink to be deleted.
 *
 * \sa mGEffCreateHDCSink, mGEffCreateHwndSink.
 */
MGEFF_EXPORT void mGEffDeleteSink(MGEFF_SINK sink);

/**
 * \fn void mGEffSetBufferSink(MGEFF_SINK handle, HDC hdc)
 * \brief Sets the content of a sink according to a DC.
 *
 * This function sets content of a sink specified by \a handle according to
 * a DC specified by \a hdc.
 *
 * \param handle The handle of the sink to be set.
 * \param hdc The handle of the DC used to set the sink.
 *
 * \sa mGEffDeleteSink.
 */
MGEFF_EXPORT void mGEffSetBufferSink(MGEFF_SINK handle, HDC hdc);

#endif

