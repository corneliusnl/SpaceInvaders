#ifndef TYPES_H
#define TYPES_H
// ##################################
// #<<<<<<< Include Files >>>>>>>>>>#
// ##################################
#include <windows.h>

// ##################################
// #<<<<<<<< Object Types >>>>>>>>>>#
// ##################################
#define UNDEFINED			0
#define WIREOBJECT 		1
#define SOLIDOBJECT     2

// ##################################
// #<<<<<<< Include Files >>>>>>>>>>#
// ##################################
#define FLATSHADING		0
#define LAMBERTSHADING	1
#define GOURAUDSHADING	2

// ##################################
// #<<<<<<< Vertex Struct >>>>>>>>>>#
// ##################################
typedef struct
{
	float	xObject, yObject, zObject;
	float	xWorld, yWorld, zWorld;

	long	xScreen, yScreen;

} VERTEX;

typedef struct
{
	float xComp, yComp, zComp;
   float Length;

} VECTOR;

typedef struct
{
	VECTOR Vector;
   long 	Deviation;
   BYTE	RedLight;
   BYTE	GreenLight;
   BYTE	BlueLight;

} LIGHT;

// ##################################
// #<<<<<<< Polygon Struct >>>>>>>>>#
// ##################################
typedef struct
{
   RGBQUAD 	Color;
	VERTEX	**Vertex;
	VECTOR	Normal;
	int 		NumVertices;
	long		Distance; // Added McJay
   // BOOL	BackFace;

} POLYGON;

// ##################################
// #<<<<<<< Object Struct >>>>>>>>>>#
// ##################################
typedef struct
{
	float	xScale, yScale, zScale;
	float	xRotate, yRotate, zRotate;
	float	xTranslate, yTranslate, zTranslate;

	VERTEX	*Vertex;
	int		NumVertices;

	POLYGON	*Polygon;
	int		NumPolygons;

	int PolygonType;
	int ShadeMethod;

} OBJECT;

#endif


