// ##################################
// #<<<<<<< Include Files >>>>>>>>>>#
// ##################################
#include <Windows.h>
#include <Math.h>
#include <Stdlib.h>
#include "World.h"
#include "Types.h"

//void SortPolygons(OBJECT *obj);

// ##################################
// #<<<<<<<<<< Projection >>>>>>>>>>#
// ##################################
void WORLD::Project(MEM_DC *Mem, OBJECT *obj, int Dist)
{
	float Val;

	for(int i = 0; i < obj->NumVertices; i++)
	{
		Val = Dist / obj->Vertex[i].zWorld;
		obj->Vertex[i].xScreen = obj->Vertex[i].xWorld * Val + 250;
		obj->Vertex[i].yScreen = obj->Vertex[i].yWorld * Val + 250;
	}
};

// ##################################
// #<<<<<<<< Sort Polygons >>>>>>>>>#
// ##################################
void WORLD::SortPolygons(OBJECT *obj)
{
	POLYGON Temp;
	int Sort = 1;

   while(Sort)
   {
   	Sort = 0;
		for(int i = 0; i < obj->NumPolygons - 1; i++)
	   {
			obj->Polygon[i].Distance = obj->Polygon[i+1].Distance = 0;
			for(int j = 0; j < obj->Polygon[i].NumVertices; j++)
				obj->Polygon[i].Distance	+= obj->Polygon[i].Vertex[j]->zWorld;
			for(int j = 0; j < obj->Polygon[i+1].NumVertices; j++)
				obj->Polygon[i+1].Distance	+= obj->Polygon[i+1].Vertex[j]->zWorld;


			obj->Polygon[i].Distance	/= (int)obj->Polygon[i].NumVertices;
			obj->Polygon[i+1].Distance	/= (int)obj->Polygon[i+1].NumVertices;

			if(obj->Polygon[i].Distance < obj->Polygon[i+1].Distance)
	      {
	   		Temp					= obj->Polygon[i];
	         obj->Polygon[i]	= obj->Polygon[i+1];
	         obj->Polygon[i+1]	= Temp;
	         Sort = 1;
	      }
	   }
   }
};

// ##################################
// #<<<<<<<< Draw Polygon >>>>>>>>>>#
// ##################################
void WORLD::DrawObject(MEM_DC *Mem, OBJECT *obj)
{
	RGBQUAD	Color;
	LIGHT 	Light;
   float		Illum;

	SortPolygons(obj);

	for(int i = 0; i < obj->NumPolygons; i++)
	{
		if(BackfaceCull(&obj->Polygon[i]))
			continue; // Continue With Next "i" Loop

		CalcPolyNormal(&obj->Polygon[i]);
      Normalize(&obj->Polygon[i].Normal);

		Light.Vector.xComp	= -1;
		Light.Vector.yComp	= 1;
		Light.Vector.zComp	= -3;
      Normalize(&Light.Vector);

      Illum = DotProduct(&obj->Polygon[i].Normal, &Light.Vector);

      if(Illum < 0.0)
      	Illum = 0.2;
      if(Illum > 1.0)
      	Illum = 1.0;

      Color.rgbRed	= obj->Polygon[i].Color.rgbRed	* Illum;
		Color.rgbGreen = obj->Polygon[i].Color.rgbGreen	* Illum;
		Color.rgbBlue	= obj->Polygon[i].Color.rgbBlue	* Illum;

		// Draw Polygon
		DrawPolygon(Mem, &obj->Polygon[i], Color);
	}
};

// ##################################
// #<<<<<<<<< Draw Polygon >>>>>>>>>#
// ##################################
int WORLD::DrawPolygon(MEM_DC *Mem, POLYGON *Poly, RGBQUAD Color)
{
	int 	*StartX;
	int 	*EndX;
	int 	tmp, y;
	long 	x, m, val;
	int 	i, j;
	char 	*c;
	int 	x1, y1, x2, y2;
	BYTE 	Color1, Color2;
	BYTE 	tab;

	StartX	= new int[Mem->GetWidth()];
   EndX		= new int[Mem->GetHeight()];

   Color1 = (BYTE)GetNearestPaletteIndex(Mem->GetPalette(), RGB(Color.rgbRed, Color.rgbGreen, Color.rgbBlue));
	val = (long)(Color1 << 24) + (long)(Color1 << 16) + (long)(Color1 << 8) + (long)(Color1);

	for(i = 0; i < Mem->GetHeight(); i++)
	{
		StartX[i] 	= -32000;
		EndX[i] 		= -32000;
	}

	j = Poly->NumVertices - 1;
	for(i = 0; i < Poly->NumVertices; i++)
	{
		x1 = Poly->Vertex[j]->xScreen;
		y1 = Poly->Vertex[j]->yScreen;
		x2 = Poly->Vertex[i]->xScreen;
		y2 = Poly->Vertex[i]->yScreen;

		if(y2 != y1)
		{
			if(y2 < y1)
			{
				tmp 	= y2;
				y2 	= y1;
				y1 	= tmp;

				tmp 	= x2;
				x2 	= x1;
				x1 	= tmp;
			}

			x = (long)x1 << 8;
			m = ((long)(x2 - x1) << 8) / ((long)(y2 - y1));
			x += m;
			y1++;

			for(y = y1; y <= y2; y++)
			{
				if((y >= 0) && (y < Mem->GetHeight()))
				{
					if(StartX[y] == -32000)
					{
						if((x >> 8) < 0)
							StartX[y] = 0;
						else if((x >> 8) >= Mem->GetWidth())
							StartX[y] = Mem->GetWidth()-1;
						else
							StartX[y] = x >> 8;
					}
					else
					{
						if((x >> 8) < 0)
							EndX[y] = 0;
						else if((x >> 8) >= Mem->GetWidth())
							EndX[y] = Mem->GetWidth()-1;
						else
							EndX[y] = x >> 8;
					}
				}
				x += m;
			}
		}
		j = i;
	}

	for(i = 0; i < Mem->GetHeight(); i++)
	{
		if(StartX[i] != -32000)
		{
			if(EndX[i] == -32000)
				EndX[i] = StartX[i];

			if(EndX[i] < StartX[i])
			{
				tmp 			= EndX[i];
				EndX[i] 		= StartX[i];
				StartX[i] 	= tmp;
			}

			c = (char *)Mem->Surface + i * Mem->GetWidth() + StartX[i];
			for(j = 0; j < ((EndX[i] - StartX[i]) >> 3); j++)
			{
				*(long *)c = (long)val;
				c += 4;
				*(long *)c = (long)val;
				c += 4;
			}

			if(((EndX[i] - StartX[i]) & 0x07))
			{
				for(j = 0; j < ((EndX[i] - StartX[i]) & 0x07); j++)
					*c++ = Color1;
			}
		}
	}
	delete StartX;
   delete EndX;
	return 0;
};

// ##################################
// #<<<<<<< BackFace Removal >>>>>>>#
// ##################################
BOOL WORLD::BackfaceCull(POLYGON *Poly)
{
	float X1, Y1, Z1;
	float X2, Y2, Z2;
	float X3, Y3, Z3;
	float D;

	if(Poly->NumVertices < 3)
		return 0;

	// X
	X1 = Poly->Vertex[0]->xWorld;
	X2 = Poly->Vertex[1]->xWorld;
	X3 = Poly->Vertex[2]->xWorld;
	// Y
	Y1 = Poly->Vertex[0]->yWorld;
	Y2 = Poly->Vertex[1]->yWorld;
	Y3 = Poly->Vertex[2]->yWorld;
   // Z
	Z1 = Poly->Vertex[0]->zWorld;
	Z2 = Poly->Vertex[1]->zWorld;
	Z3 = Poly->Vertex[2]->zWorld;

	D = (	X3 * ((Z1 * Y2) - (Y1 * Z2)) )
	  + ( Y3 * ((X1 * Z2) - (Z1 * X2)) )
	  + ( Z3 * ((Y1 * X2) - (X1 * Y2)) );

	if(D < 0)
		return FALSE;
	else
		return TRUE;
};

// ##################################
// #<<< Calculate Polygon Normal >>>#
// ##################################
void WORLD::CalcPolyNormal(POLYGON *Poly)
{
	VECTOR Vect1, Vect2;

   Vect1.xComp = (Poly->Vertex[0]->xWorld - Poly->Vertex[1]->xWorld);
   Vect1.yComp = (Poly->Vertex[0]->yWorld - Poly->Vertex[1]->yWorld);
   Vect1.zComp = (Poly->Vertex[0]->zWorld - Poly->Vertex[1]->zWorld);

   Vect2.xComp = (Poly->Vertex[2]->xWorld - Poly->Vertex[1]->xWorld);
   Vect2.yComp = (Poly->Vertex[2]->yWorld - Poly->Vertex[1]->yWorld);
   Vect2.zComp = (Poly->Vertex[2]->zWorld - Poly->Vertex[1]->zWorld);

   Poly->Normal = CrossProduct(&Vect1, &Vect2);
};

// ##################################
// #<<<<<< Normalize Vector >>>>>>>>#
// ##################################
void WORLD::Normalize(VECTOR *Vect)
{
	Vect->Length = sqrt(Vect->xComp * Vect->xComp
							+ Vect->yComp * Vect->yComp
							+ Vect->zComp * Vect->zComp);

	Vect->xComp = (Vect->xComp / Vect->Length);
	Vect->yComp = (Vect->yComp / Vect->Length);
	Vect->zComp = (Vect->zComp / Vect->Length);
};

// ##################################
// #<<<<<<<<< Dot Product >>>>>>>>>>#
// ##################################
float WORLD::DotProduct(VECTOR *Vect1, VECTOR *Vect2)
{
   return 	Vect1->xComp * Vect2->xComp +
   			Vect1->yComp * Vect2->yComp +
				Vect1->zComp * Vect2->zComp;
};

// ##################################
// #<<<<<<<< Cross Product >>>>>>>>>#
// ##################################
VECTOR WORLD::CrossProduct(VECTOR *Vect1, VECTOR *Vect2)
{
	VECTOR RetVect;

	RetVect.xComp = Vect1->yComp * Vect2->zComp - Vect1->zComp * Vect2->yComp;
	RetVect.yComp = Vect1->zComp * Vect2->xComp - Vect1->xComp * Vect2->zComp;
	RetVect.zComp = Vect1->xComp * Vect2->yComp - Vect1->yComp * Vect2->xComp;

   return RetVect;
};
