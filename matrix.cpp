// ############################################
// #<<<<<<<<<<< Math 3-D: Matrix Procs >>>>>>>#
// ############################################
// #<<<<<<<<<<<<<< Include Files >>>>>>>>>>>>>#
// ############################################
#include <stdio.h>
#include <math.h>
#include <mem.h>
#include "Matrix.h"
#include "types.h"

// ############################################
// #<<<<<<<<<<<< Initiate Matrix's >>>>>>>>>>>#
// ############################################
MATRIX::MATRIX()
{
	MainMatrix.ElemPtr 	= *MainMatrix.Element;
	TempMatrix.ElemPtr 	= *TempMatrix.Element;
	UsageMatrix.ElemPtr 	= *UsageMatrix.Element;

	Identity(MainMatrix.ElemPtr);
};

// ############################################
// #<<<<<<<<< Create Identity Matrix >>>>>>>>>#
// ############################################
void MATRIX::Identity(float *MatPtr)
{
	memset(MatPtr, 0, sizeof(float) * 15);
	*(MatPtr)       = *(MatPtr + 5)   =
	*(MatPtr + 10)  = *(MatPtr + 15)  = 1;
};

// ############################################
// #<<<<<<<<<<< Multiply Matrix's >>>>>>>>>>>>#
// ############################################
void MATRIX::Multiply(float *Dest, float *ptr1, float *ptr2)
{
	int 	t;

	for(int i = 0; i < 4; i++)
   {
		t = i * 4;
		*(Dest + t) =
     		( (*(ptr1 + t))     * (*(ptr2))     	)
		+	( (*(ptr1 + t + 1)) * (*(ptr2 + 4)) 	)
		+	( (*(ptr1 + t + 2)) * (*(ptr2 + 8)) 	)
		+	( (*(ptr1 + t + 3)) * (*(ptr2 + 12))	);

		t = i * 4;
		*(Dest + t + 1) =
     		( (*(ptr1 + t))     * (*(ptr2 + 1))   	)
		+	( (*(ptr1 + t + 1)) * (*(ptr2 + 5)) 	)
		+	( (*(ptr1 + t + 2)) * (*(ptr2 + 9)) 	)
		+	( (*(ptr1 + t + 3)) * (*(ptr2 + 13))	);

		t = i * 4;
		*(Dest + t + 2) =
     		( (*(ptr1 + t))     * (*(ptr2 + 2))   	)
		+	( (*(ptr1 + t + 1)) * (*(ptr2 + 6)) 	)
		+	( (*(ptr1 + t + 2)) * (*(ptr2 + 10)) 	)
		+	( (*(ptr1 + t + 3)) * (*(ptr2 + 14))	);

		t = i * 4;
		*(Dest + t + 3) =
     		( (*(ptr1 + t))     * (*(ptr2 + 3))   	)
		+	( (*(ptr1 + t + 1)) * (*(ptr2 + 7)) 	)
		+	( (*(ptr1 + t + 2)) * (*(ptr2 + 11)) 	)
		+	( (*(ptr1 + t + 3)) * (*(ptr2 + 15))	);
   }
};

// ############################################
// #<<<<<<<<<<<<<< Copy Matrix >>>>>>>>>>>>>>>#
// ############################################
void MATRIX::Copy(float *dest, float *src)
{
	memcpy(dest, src, sizeof(float) * 16);
};

// ############################################
// #<<<<<<<<<<<<<< Scale Matrix  >>>>>>>>>>>>>#
// ############################################
void MATRIX::Scale(float sx, float sy, float sz)
{
	Identity(UsageMatrix.ElemPtr);
	*(UsageMatrix.ElemPtr)      = (float)sx;
	*(UsageMatrix.ElemPtr + 5)  = (float)sy;
	*(UsageMatrix.ElemPtr + 10) = (float)sz;

	Multiply(TempMatrix.ElemPtr, UsageMatrix.ElemPtr, MainMatrix.ElemPtr);
	Copy(MainMatrix.ElemPtr, TempMatrix.ElemPtr);
};

// ############################################
// #<<<<<<<<<<<<<< Rotate Matrix >>>>>>>>>>>>>#
// ############################################
void MATRIX::Rotate(float NX, float NY, float NZ)
{
	float	vSin, vCos;

	// X-Axis Rotation
	Identity(UsageMatrix.ElemPtr);
	vSin = sin(NX);
   vCos = cos(NX);
	*(UsageMatrix.ElemPtr + 5)  = vCos;
	*(UsageMatrix.ElemPtr + 6)  = vSin;
	*(UsageMatrix.ElemPtr + 9)  = -vSin;
	*(UsageMatrix.ElemPtr + 10) = vCos;
	Multiply(TempMatrix.ElemPtr, MainMatrix.ElemPtr, UsageMatrix.ElemPtr);
	Copy(MainMatrix.ElemPtr, TempMatrix.ElemPtr);

	// Y-Axis Rotation
	Identity(UsageMatrix.ElemPtr);
	vSin = sin(NY);
   vCos = cos(NY);
	*(UsageMatrix.ElemPtr)      = vCos;
	*(UsageMatrix.ElemPtr + 2)  = -vSin;
	*(UsageMatrix.ElemPtr + 8)  = vSin;
	*(UsageMatrix.ElemPtr + 10) = vCos;
	Multiply(TempMatrix.ElemPtr, MainMatrix.ElemPtr, UsageMatrix.ElemPtr);
	Copy(MainMatrix.ElemPtr, TempMatrix.ElemPtr);

	// Z-Axis Rotation
	Identity(UsageMatrix.ElemPtr);
	vSin = sin(NZ);
   vCos = cos(NZ);
	*(UsageMatrix.ElemPtr)      = vCos;
	*(UsageMatrix.ElemPtr + 1)  = vSin;
	*(UsageMatrix.ElemPtr + 4)  = -vSin;
	*(UsageMatrix.ElemPtr + 5)  = vCos;
	Multiply(TempMatrix.ElemPtr, MainMatrix.ElemPtr, UsageMatrix.ElemPtr);
	Copy(MainMatrix.ElemPtr, TempMatrix.ElemPtr);
};

// ############################################
// #<<<<<<<<<<<< Translate Matrix >>>>>>>>>>>>#
// ############################################
void MATRIX::Translate(float xt, float yt, float zt)
{
  	Identity(UsageMatrix.ElemPtr);
	*(UsageMatrix.ElemPtr + 12) = (float)xt;
	*(UsageMatrix.ElemPtr + 13) = (float)yt;
	*(UsageMatrix.ElemPtr + 14) = (float)zt;

	Multiply(TempMatrix.ElemPtr, MainMatrix.ElemPtr, UsageMatrix.ElemPtr);
	Copy(MainMatrix.ElemPtr, TempMatrix.ElemPtr);
};

// ############################################
// #<<<<<<<<<<<<< Transform Object >>>>>>>>>>>#
// ############################################
void MATRIX::Transform(OBJECT *obj)
{
	UINT   v;
	VERTEX *vptr;

	for(v = 0; v < obj->NumVertices; v++)
	{
		vptr = &(*obj).Vertex[v];

		vptr->xWorld = (vptr->xObject * (*(MainMatrix.ElemPtr))		) +
					   	(vptr->yObject * (*(MainMatrix.ElemPtr + 4))	) +
					   	(vptr->zObject * (*(MainMatrix.ElemPtr + 8))	) +
					   	(*(MainMatrix.ElemPtr + 12));

		vptr->yWorld = (vptr->xObject * (*(MainMatrix.ElemPtr + 1))	) +
						   (vptr->yObject * (*(MainMatrix.ElemPtr + 5))	) +
						   (vptr->zObject * (*(MainMatrix.ElemPtr + 9))	) +
						   (*(MainMatrix.ElemPtr + 13));

		vptr->zWorld = (vptr->xObject * (*(MainMatrix.ElemPtr + 2))	) +
						   (vptr->yObject * (*(MainMatrix.ElemPtr + 6))	) +
						   (vptr->zObject * (*(MainMatrix.ElemPtr + 10))) +
						   (*(MainMatrix.ElemPtr + 14));
	}
};
