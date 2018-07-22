#ifndef MATRIX_H
#define MATRIX_H
// ############################################
// #<<<<<<<<<<< Math 3-D: Matrix Procs >>>>>>>#
// ############################################
// #<<<<<<<<<<<<<< Include Files >>>>>>>>>>>>>#
// ############################################
#include "types.h"

// ############################################
// #<<<<<<<<<<<<<< Matrix Struct >>>>>>>>>>>>>#
// ############################################
typedef struct
{
	float Element[4][4];
	float *ElemPtr;

} MATRIX4x4;

// ############################################
// #<<<<<<<<<<<<<< Matrix Class >>>>>>>>>>>>>>#
// ############################################
class MATRIX
{
public:
	MATRIX4x4		MainMatrix;
	MATRIX4x4		UsageMatrix;
	MATRIX4x4		TempMatrix;

	// Matrix Initilization
	MATRIX();

   // Matrix Manuiplation
	void 	Identity(float *ptr);
	void	Transform(OBJECT *object);
	void 	Scale(float sx, float sy, float sz);
	void 	Rotate(float rx, float ry, float rz);
	void 	Translate(float tx, float ty, float tz);
	void	Multiply(float *dest, float *mat1, float *mat2);
	void	Copy(float *dest, float *src);
};

#endif
