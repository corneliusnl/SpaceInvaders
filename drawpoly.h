#ifndef POLYDRAW_H
#define POLYDRAW_H
// ##################################
// #<<<<<<< Include Files >>>>>>>>>>#
// ##################################
#include "MemDC.h"
#include "types.h"

// ##################################
// #<<<<<<<<< Procedures >>>>>>>>>>>#
// ##################################
BOOL	BackfaceCull(POLYGON *);
void	Project(MEM_DC *Mem, OBJECT *, int);
int	DrawPolygon(MEM_DC *Mem, POLYGON *Poly, RGBQUAD Color);
void	DrawObject(MEM_DC *Mem, OBJECT *obj);
void	CalcPolyNormal(POLYGON *Poly);
void	Normalize(VECTOR *Vect);
VECTOR CrossProduct(VECTOR *Vect1, VECTOR *Vect2);
float DotProduct(VECTOR *Vect1, VECTOR *Vect2);
#endif
