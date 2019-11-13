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

#ifndef COMPUTE_3D_H
#define COMPUTE_3D_H

#include "mgeff-common.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

typedef struct _TransAffine_3D
{
   double m[12]; //World TransForm;
}TransAffine3D;

typedef struct _Vector3D
{
    int x;
	int y;
	int z;
}Vector3D;

typedef struct _Vector2D
{
    int x;
	int y;
}Vector2D;


typedef TransAffine3D* PTransAffine3D;
typedef Vector3D* PVector3D;
typedef Vector2D* PVector2D;

#define PI 3.14159265358979323846


#define CHECK_SAFE_PAREMATER(parameter) if (!parameter) return 0;

//function
MGEFF_EXPORT TransAffine3D* Init3DSenceTransAffine (void);
MGEFF_EXPORT int MatrixMultiply (double* A, double* B, double* C);
MGEFF_EXPORT int Set3DPointOfView (TransAffine3D* pTrans, double dXMove, double dYMove, double dZMove);
MGEFF_EXPORT int Scale3DSence (TransAffine3D* pTrans, double x, double y, double z);
MGEFF_EXPORT int Move3DSence (TransAffine3D* pTrans, double x, double y, double z);
MGEFF_EXPORT int Roll3DSence (TransAffine3D* pTrans, double dZAngle);
MGEFF_EXPORT int RotateHoriz3DSence (TransAffine3D* pTrans, double dYAngle);
MGEFF_EXPORT int RotateVertical3DSence (TransAffine3D* pTrans, double dXAngle);
MGEFF_EXPORT int Generate3DPointBy3DSence (TransAffine3D* pTrans, Vector3D* pVector);
MGEFF_EXPORT void Terminate3DSenceTransAffine (TransAffine3D* pTransAffine);
MGEFF_EXPORT int Perspective3DTo2D (Vector3D* pVector3D, Vector2D* pVector2D, int nPerspectiveZ, int nXCenter, int nYCenter);
MGEFF_EXPORT int Reset3DSenceTransAffine (TransAffine3D* pTransAffine);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif

