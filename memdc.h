#ifndef OFFSCRN_H
#define OFFSCRN_H
// žžžžžžžžžžžžžžžžžžžžžžžžžžžžž
// ž Include Files
// žžžžžžžžžžžžžžžžžžžžžžžžžžžžž
#include <Windows.h>
#include "Utils.h"

// žžžžžžžžžžžžžžžžžžžžžžžžžžžžž
// ž Off Screen DC Class
// žžžžžžžžžžžžžžžžžžžžžžžžžžžžž
class MEM_DC
{
private:
   HPALETTE	hPalette;
	HBITMAP	hBitmap;
	HBITMAP	hOldBmp;
	LOGFONT	FontRec;

public:
	BITMAPINFO	*Info;
   void far		*Surface;
   HDC			MemDC;

	// Class Initilization/Destruction
	MEM_DC(int Width, int Height);
   ~MEM_DC();

	// OffScreen Clearing
	int Clear(COLORREF Color);

	// Screen Bltting
	int BltToScreen(HDC Screen, int X, int Y);
   int StretchToScreen(HDC Screen, int XDest, int YDest, int DestW, int DestH,
   		int SrcX, int SrcY, int SrcW, int SrcH);

	// Palette Manipulation
	int		PrepareFade(HDC Screen, int X, int Y);
	int 		FadeOut(HDC Screen, int Delay);
   int 		FadeIn(HDC Screen, int Delay);
	int		EndFade(HDC Screen, int X, int Y);
   int 		HalftonePalette(HDC hDC);
   HPALETTE BitmapPalette(HDC Screen, DIB Dib);
	HPALETTE	CreateIdentityPalette(RGBQUAD *RgbValues, int NumColors);

   // Dib Loading
   DIB		LoadBitmap(char *File);
   DIB		LoadSprite(char *File, char *Title);
	LPDIB		LoadSprites(char *File);
   int		FreeDib(DIB Dib);
   int		FreeDibs(LPDIB Dib, int Start, int End);

   // Dib Bltting
   int 		DibBlt(DIB Dib, int X, int Y);
   int		TransBlt(DIB Dib, int X, int Y, char TransColor);

	// Font Manipulation
	int		SetFont(int Width, int Height, COLORREF Color);
	int		DisplayText(int Number, int X, int Y);
	int		DisplayText(char *Text, int X, int Y);
	int		DeleteFont();

	// Inline Functions
	inline int			GetWidth()		{ return Info->bmiHeader.biWidth; }
	inline int			GetHeight()		{ return -Info->bmiHeader.biHeight; }
	inline int			StorageWidth()	{ return (Info->bmiHeader.biWidth + 3)& ~3; }
   inline HPALETTE	GetPalette()	{ return hPalette; }
};
#endif
