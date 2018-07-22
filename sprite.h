// ######################################
// #<<<<<<<<<< Include Files >>>>>>>>>>>#
// ######################################
#include <Windows.h>
#include "Colors.h"

// ######################################
// #<<<<<<<<<<< Bitmap Types >>>>>>>>>>>#
// ######################################
#define TYP_BITMAP	0x4D42
#define TYP_ICON		0x4349
#define TYP_CURSOR	0x5450

// ######################################
// #<<<<<<<<<< Palette Defines >>>>>>>>>#
// ######################################
#define PAL_STATIC		0
#define PAL_NOSTATIC		1

// #######################################
// #<<<<<<<<<< JCM File Info >>>>>>>>>>>>#
// #######################################
typedef struct
{
	LONG	jcmNumBitmaps;
	WORD	jcmNumPalEntries;

} JCMFILEHEADER;

// #######################################
// #<<<<<<< Individule JCM Info >>>>>>>>>#
// #######################################
typedef struct
{
	char	jcmTitle[20];
	LONG	jcmWidth;
	LONG	jcmHeight;
	DWORD	jcmSizeImage;

} JCMCOMPONENTHEADER;

// ######################################
// #<<<<<<<<<< DIB Structure >>>>>>>>>>>#
// ######################################
typedef struct
{
	BITMAPINFOHEADER	BitmapInfo;
   RGBQUAD				BitmapPal[256];
   HGLOBAL				Handle;

} SPRDIB;

// ######################################
// #<<<<<<<< Bitmap Info Struct >>>>>>>>#
// ######################################
typedef struct
{
	BITMAPINFOHEADER		bmiHeader;
	RGBQUAD					bmiColors[256];

} BMPINFO;

// ######################################
// #<<<<<<<<<< Palette Struct >>>>>>>>>>#
// ######################################
typedef struct
{
	WORD	Version;
   WORD	NumberOfEntries;
   PALETTEENTRY	aEntries[256];

} PALETTE;

// ######################################
// #<<<<<<<<<<<< Sprite Class >>>>>>>>>>#
// ######################################
class SPRITEDC
{
private:
   HPALETTE	SprDCPal;
	HBITMAP	hSprBmp;
	HBITMAP	hOldBmp;
	LOGFONT	FontRec;
	WORD 		SprDCWidth, SprDCHeight;

public:
	BMPINFO	SprDCInfo;
   void far *SprDCBits;
   HDC		SpriteDC;

	SPRITEDC(int Width, int Height);
   ~SPRITEDC();

	// Sprite Bltting
   int SpriteBlt(SPRDIB Sprite, int XDest, int YDest);
   int TransBlt(SPRDIB Sprite, int XDest, int YDest, char TransColor);

	// Screen Bltting
   int StretchToScreen(HDC Screen, int XDest, int YDest, int DestW, int DestH,
   		int SrcX, int SrcY, int SrcW, int SrcH);
   int SprToScreen(HDC Screen, int X, int Y);

   // Sprite Loading/UnLoading
	SPRDIB 	LoadBitmap(char *File);
	SPRDIB	LoadComponent(char *File, char *Component);
	SPRDIB	*LoadComponents(char *File);
   int		FreeBitmap(SPRDIB Dib);
   int 		FreeComponents(SPRDIB *Dib, int Start, int End);

	// PreBlt Sprite Routines
   int Clear();
	int Clear(COLORREF Color);
   
   // Palette Functions
	int 		FadeOut(HDC Screen, int Delay);
   int 		FadeIn(HDC Screen, int Delay);
   int 		HalftonePalette(HDC hDC);
   HPALETTE BitmapPalette(SPRDIB Dib);
	HPALETTE	CreateIdentityPalette(RGBQUAD aRGB[], int Usage);

   // Font Manipulation
	int SetFont(int Width, int Height, COLORREF Color);
	int DisplayText(int Number, int X, int Y);
	int DisplayText(char *Text, int X, int Y);
	int DeleteFont();

   // In-line Functions
   HPALETTE GetPalette() 	{ return SprDCPal; 		};
   int 		Width() 			{ return SprDCWidth; 	};
   int		Height()       { return SprDCHeight;	};
};
