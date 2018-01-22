/*
 ** $Id: 3DCompute.h 10885 2009-06-24 12:01:39Z dengkexi$
 **
 ** 3DCompute.h: Implementation of 3D computation,
 **
 ** Copyright (C) 2003 ~ 2008 Feynman Software.
 **
 ** Create date: 2009/06/24
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

