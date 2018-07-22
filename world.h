#ifndef MCJAY_WORLD_H
#define MCJAY_WORLD_H

#include "MemDC.h"
#include "types.h"

class WORLD
{
private:
public:
	OBJECT *LoadObject(char *);

	void	SortPolygons(OBJECT *obj);
	void	Project(MEM_DC *Mem, OBJECT *obj, int Dist);
   void	DrawObject(MEM_DC *Mem, OBJECT *obj);
   int	DrawPolygon(MEM_DC *Mem, POLYGON *Poly, RGBQUAD Color);
   BOOL	BackfaceCull(POLYGON *Poly);
	void	CalcPolyNormal(POLYGON *Poly);

   // Mathematics
   void		Normalize(VECTOR *Vect);
   float		DotProduct(VECTOR *Vect1, VECTOR *Vect2);
   VECTOR	CrossProduct(VECTOR *Vect1, VECTOR *Vect2);
};

#endif