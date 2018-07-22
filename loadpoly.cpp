// ##################################
// #<<<<<<< Include Files >>>>>>>>>>#
// ##################################
#include <stdio.h>
#include <Stdlib.h>
#include <string.h>
#include "World.h"

// ##################################
// #<<<<<<<< Load Polygon >>>>>>>>>>#
// ##################################
OBJECT *WORLD::LoadObject(char *Filename)
{
	OBJECT   *Obj;
	FILE 		*fp;
   char 		Buff[300];
	char 		*P;
	int		PolyNum;

	// #########################################
	// Open File					                #
	// #########################################
   if((fp 	= fopen(Filename, "rt")) ==  NULL)
	{
		wsprintf(Buff, "Could Not Open File:  %s", Filename);
   	MessageBox(GetFocus(), Buff, "Error", MB_OK);
		return NULL;
   }
	Obj	= new OBJECT;

	// #########################################
	// Read File Information                   #
	// #########################################
	while(fgets(Buff, sizeof(Buff), fp))
	{
		// Put String in Upper Case
   	strupr(Buff);

		// #########################################
		// Comment		                            #
      // #########################################
      if(P = strchr(Buff, ';'))
      {
      	*P = '\0';
      }
		// #########################################
		// Polygon Type                            #
      // #########################################
		if(strstr(Buff, "UNDEFINED_OBJECT_TYPE"))
      {
			Obj->PolygonType = UNDEFINED;
      }
   	else if(strstr(Buff, "WIRE_OBJECT_TYPE"))
      {
			Obj->PolygonType = WIREOBJECT;
      }
		else if(strstr(Buff, "SOLID_OBJECT_TYPE"))
      {
			Obj->PolygonType = SOLIDOBJECT;
		}

		// #########################################
		// Object Description                      #
      // #########################################
		if(strchr(Buff, '{'))
      {
      	while(!strchr(Buff, '}'))
         {
				// Continue to read from file
				fgets(Buff, sizeof(Buff), fp);

				// #########################################
				// Comment		                            #
		      // #########################################
		      if(P = strchr(Buff, ';'))
		      {
		      	*P = '\0';
		      }
		      // #########################################
				// Number of Vertices                      #
		      // #########################################
				if((P = strstr(Buff, "VERTICES=")) != NULL)
				{
					P += 9;
					Obj->NumVertices = atoi(P);
					Obj->Vertex = new VERTEX[Obj->NumVertices];
				}

				// #########################################
				// Number of Vertices                      #
		      // #########################################
				if((P = strstr(Buff, "POLYGONS=")) != NULL)
				{
					P += 9;
					Obj->NumPolygons = atoi(P);
					Obj->Polygon = new POLYGON[Obj->NumPolygons];
				}

				// #########################################
				// Polygon Shading Method                  #
		      // #########################################
				if((P = strstr(Buff, "OBJECTSHADING=")) != NULL)
				{
					P += 14;
					if(strstr(P, "FLAT"))
		         {
						Obj->ShadeMethod = FLATSHADING;
					}
					else if(strstr(P, "LAMBERT"))
		         {
		         	Obj->ShadeMethod = LAMBERTSHADING;
		         }
		         else if(strstr(P, "GOURAUD"))
		         {
		         	Obj->ShadeMethod = GOURAUDSHADING;
		         }
				}
      	}
      }

		// #########################################
		// Read Object Structure Information       #
		// #########################################
      if(strstr(Buff, "BEGIN"))
      {
      	while(!strstr(Buff, "END"))
         {
				// Continue to read from file
				fgets(Buff, sizeof(Buff), fp);

				// #########################################
				// Comment		                            #
		      // #########################################
		      if(P = strchr(Buff, ';'))
		      {
		      	*P = '\0';
		      }
				// #########################################
				// Vertices                          		 #
		      // #########################################
				if((P = strstr(Buff, "VERT")) != NULL)
				{
					int VertNum;
					int X, Y, Z;

					if(sscanf(Buff, "%*s <%d,%d,%d>", &X, &Y, &Z) == 3)
		         {
						P += 4;
                  VertNum = atoi(P);
						Obj->Vertex[VertNum].xObject = X;
						Obj->Vertex[VertNum].yObject = Y;
						Obj->Vertex[VertNum].zObject = Z;
	   		  	}
				}

				// #########################################
				// Polygon Vertices                        #
		      // #########################################
				if((P = strstr(Buff, "POLY")) != NULL)
				{
		         char Tmp[250];
					int NumVert, V;

					P += 4;
		   		PolyNum = strtoul(P, &P, 0);
		         NumVert = strtoul(P, &P, 0);

					Obj->Polygon[PolyNum].Vertex 				= new VERTEX*[NumVert];
					Obj->Polygon[PolyNum].NumVertices 		= NumVert;

					for(int i = 0; i < NumVert; i++)
		         {
		            P++;  // Skip Brackets & Commas
		         	V = strtoul(P, &P, 0);
		      		Obj->Polygon[PolyNum].Vertex[i] = &Obj->Vertex[V];
					}
				}

				// #########################################
				// Polygon Information                     #
		      // #########################################
            if(strchr(Buff, '{'))
            {
            	while(!strchr(Buff, '}'))
               {
						fgets(Buff, sizeof(Buff), fp);

				      // #########################################
						// Polygon Color Data                      #
				      // #########################################
						if((P = strstr(Buff, "RGB")) != NULL)
				      {
							int Tmp;

							P += 4;
							// Get Red Value
                     Tmp = strtoul(P, &P, 0);
		               Obj->Polygon[PolyNum].Color.rgbRed		= Tmp;
                     P++;
							// Get Green Value
                     Tmp = strtoul(P, &P, 0);
  		               Obj->Polygon[PolyNum].Color.rgbGreen	= Tmp;
                     P++;
							// Get Blue Value
                     Tmp = strtoul(P, &P, 0);
  		               Obj->Polygon[PolyNum].Color.rgbBlue		= Tmp;
                  }
               }
		      }
         }
      }
	}

	fclose(fp);
   return Obj;
};
