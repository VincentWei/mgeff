/*
 ** $Id: 3DCompute.c 10885 2009-06-24 12:01:39Z dengkexi$
 **
 ** 3DCompute.c: Implementation of 3D computation,
 **
 ** Copyright (C) 2003 ~ 2008 Feynman Software.
 **
 ** Create date: 2009/06/24
 */
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "3DCompute.h"

TransAffine3D* Init3DSenceTransAffine (void)
{
   TransAffine3D* pTransAffine = NULL;

   pTransAffine = (TransAffine3D*) malloc (sizeof (TransAffine3D));

   if (pTransAffine)
   {
       memset (pTransAffine, 0x0, sizeof (TransAffine3D));
   }

   pTransAffine->m[0] = 1;
   pTransAffine->m[1] = 0;
   pTransAffine->m[2] = 0;
   pTransAffine->m[3] = 0;

   pTransAffine->m[4] = 0;
   pTransAffine->m[5] = 1;
   pTransAffine->m[6] = 0;
   pTransAffine->m[7] = 0;

   pTransAffine->m[8] = 0;
   pTransAffine->m[9] = 0;
   pTransAffine->m[10] = 1;
   pTransAffine->m[11] = 0;

   return pTransAffine;
}

int MatrixMultiply (double* A, double* B, double* C)
{
    double tx, ty, tz;
	tx = A[3];
	ty = A[7];
	tz = A[11];
	
	C[0] = A[0] * B[0] + A[1] * B[4] + A[2] * B[8];
	C[1] = A[0] * B[1] + A[1] * B[5] + A[2] * B[9];
	C[2] = A[0] * B[2] + A[1] * B[6] + A[2] * B[10];
	C[3] = tx * B[0] + ty * B[4] + tz * B[8];
	
	C[4] = A[4] * B[0] + A[5] * B[4] + A[6] * B[8];
	C[5] = A[4] * B[1] + A[5] * B[5] + A[6] * B[9];
	C[6] = A[4] * B[2] + A[5] * B[6] + A[6] * B[10];
	C[7] = tx * B[1] + ty * B[5] + tz * B[9];
	
	C[8] = A[8] * B[0] + A[9] * B[4] + A[10] * B[8];
	C[9] = A[8] * B[1] + A[9] * B[5] + A[10] * B[9];
	C[10] = A[8] * B[2] + A[9] * B[6] + A[10] * B[10];
	C[11] = tx * B[2] + ty * B[6] + tz * B[10];

    return 1;
}



int Set3DPointOfView (TransAffine3D* pTrans, double dXMove, double dYMove, double dZMove)
{
    CHECK_SAFE_PAREMATER(pTrans);

    //init MoveMatrix;
    pTrans->m[0] = 1;
    pTrans->m[1] = 0;
    pTrans->m[2] = 0;
    pTrans->m[3] = dXMove;

    pTrans->m[4] = 0;
    pTrans->m[5] = 1;
    pTrans->m[6] = 0;
    pTrans->m[7] = dYMove;

    pTrans->m[8] = 0;
    pTrans->m[9] = 0;
    pTrans->m[10] = 1;
    pTrans->m[11] = dZMove;
	return 1;
}

int Scale3DSence (TransAffine3D* pTrans, double x, double y, double z)
{
    double tx, ty, tz;

    CHECK_SAFE_PAREMATER(pTrans);

	tx = pTrans->m[3];
	ty = pTrans->m[7];
	tz = pTrans->m[11];

    pTrans->m[0] *= x;
    pTrans->m[4] *= x;
    pTrans->m[8] *= x;
    pTrans->m[3] *= x;

    pTrans->m[1] *= y;
    pTrans->m[5] *= y;
    pTrans->m[9] *= y;
    pTrans->m[7] *= y;

    pTrans->m[2] *= z;
    pTrans->m[6] *= z;
    pTrans->m[10] *= z;
    pTrans->m[11] *= z;

    return 1;
}

int Move3DSence (TransAffine3D* pTrans, double x, double y, double z)
{
    CHECK_SAFE_PAREMATER(pTrans);

    pTrans->m[3] += x;
    pTrans->m[7] += y;
    pTrans->m[11] += z;

    return 1;
}

//Use right hand theory, the first fingure is pointing at U.
//If now U roll your right, it will roll around Z axis.
int Roll3DSence (TransAffine3D* pTrans, double dZAngle)
{
	double R[12];
	double W[12];
	double c,s;
    CHECK_SAFE_PAREMATER(pTrans);

    //Start to roll around Z axis
    dZAngle = dZAngle * PI / 180;
	
	c = (double)cos (dZAngle);
	s = (double)sin (dZAngle);
	
	R[0] = c;	 R[1] = -s;   R[2] = 0;   R[3] = 0;
	R[4] = s;	 R[5] = c;	  R[6] = 0;   R[7] = 0;
	R[8] = 0;	 R[9] = 0;	  R[10]= 1;   R[11]= 0;
	
    MatrixMultiply (pTrans->m, R, W);
	memcpy (pTrans->m, W, 12 * sizeof (double));
	
    return 1;
}


//Rotate horizon, the 3D will rotate around Y axis.
int RotateHoriz3DSence (TransAffine3D* pTrans, double dYAngle)
{
	double R[12];
	double W[12];
	double c,s;
    CHECK_SAFE_PAREMATER(pTrans);

    dYAngle = dYAngle * PI / 180;
	c = cos (dYAngle);
	s = sin (dYAngle);
	
	R[0] = c;	R[1] = 0;	R[2] = s;	R[3] = 0;
	R[4] = 0;	R[5] = 1;	R[6] = 0;	R[7] = 0;
	R[8] = -s;	R[9] = 0;	R[10]= c;	R[11]= 0;


    MatrixMultiply (pTrans->m, R, W);
	memcpy (pTrans->m, W, 12 * sizeof (double));

	return 1;

}

//Rotate vertical, the 3D will rotate around X axis.
int RotateVertical3DSence (TransAffine3D* pTrans, double dXAngle)
{
	double R[12];
	double W[12];
	double c,s;
    CHECK_SAFE_PAREMATER(pTrans);

    dXAngle = dXAngle * PI / 180;

	c = cos (dXAngle);
	s = sin (dXAngle);
	
	R[0] = 1;	R[1] = 0;	R[2] = 0;	R[3] = 0;
	R[4] = 0;	R[5] = c;	R[6] = -s;	R[7] = 0;
	R[8] = 0;	R[9] = s;	R[10]= c;	R[11]= 0;
	
    MatrixMultiply (pTrans->m, R, W);
	memcpy (pTrans->m, W, 12 * sizeof (double));

    return 1;
}


#include <stdio.h>
int Generate3DPointBy3DSence (TransAffine3D* pTrans, Vector3D* pVector)
{
    Vector3D TmpVector;
    CHECK_SAFE_PAREMATER(pTrans);
    CHECK_SAFE_PAREMATER(pVector);


    memcpy (&TmpVector, pVector, sizeof (Vector3D));
	pVector->x = (int)(TmpVector.x * pTrans->m[0] + \
		               TmpVector.y * pTrans->m[4] + \
		               TmpVector.z * pTrans->m[8] + \
		               pTrans->m[3]);

	pVector->y = (int)(TmpVector.x * pTrans->m[1] + \
		               TmpVector.y * pTrans->m[5] + \
		               TmpVector.z * pTrans->m[9] + \
		               pTrans->m[7]);

	pVector->z = (int)(TmpVector.x * pTrans->m[2] + \
		               TmpVector.y * pTrans->m[6] + \
		               TmpVector.z * pTrans->m[10] + \
		               pTrans->m[11]);
	
    return 1;
}


void Terminate3DSenceTransAffine (TransAffine3D* pTransAffine)
{
    if (pTransAffine)
        free (pTransAffine);
}



int Perspective3DTo2D (Vector3D* pVector3D, Vector2D* pVector2D, int nPerspectiveZ, int nXCenter, int nYCenter)
{
    CHECK_SAFE_PAREMATER (pVector3D);
    CHECK_SAFE_PAREMATER (pVector2D);
	pVector2D->x = pVector3D->x * nPerspectiveZ / (nPerspectiveZ - pVector3D->z) + nXCenter;
	pVector2D->y = pVector3D->y * nPerspectiveZ / (nPerspectiveZ - pVector3D->z) + nYCenter;

    return 1;
}


int Reset3DSenceTransAffine (TransAffine3D* pTransAffine)
{
    CHECK_SAFE_PAREMATER (pTransAffine);

	memset (pTransAffine, 0x0, sizeof (TransAffine3D));
    //init MoveMatrix;
    pTransAffine->m[0] = 1;
    pTransAffine->m[1] = 0;
    pTransAffine->m[2] = 0;
    pTransAffine->m[3] = 0;

    pTransAffine->m[4] = 0;
    pTransAffine->m[5] = 1;
    pTransAffine->m[6] = 0;
    pTransAffine->m[7] = 0;

    pTransAffine->m[8] = 0;
    pTransAffine->m[9] = 0;
    pTransAffine->m[10] = 1;
    pTransAffine->m[11] = 0;
    return 1;
}


