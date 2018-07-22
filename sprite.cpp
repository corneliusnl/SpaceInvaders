// ######################################
// #<<<<<<<<<<<< Include Files >>>>>>>>>#
// ######################################
#include <Windows.h>
#include <Stdio.h>
#include <stdlib.h>
#include "Sprite.h"

// ######################################
// #<<<<<<<<<< Create Sprite DC >>>>>>>>#
// ######################################
SPRITEDC::SPRITEDC(int Width, int Height)
{
   HDC 	Screen;
   int 	Counter;

   SprDCWidth 	= (WORD)Width + Width % 4;
   SprDCHeight	= (WORD)Height;

	// Set Bitmap Info Header Information
   SprDCInfo.bmiHeader.biSize 			= sizeof(BITMAPINFOHEADER);
   SprDCInfo.bmiHeader.biSizeImage 		= SprDCWidth * SprDCHeight;
   SprDCInfo.bmiHeader.biHeight			= -SprDCHeight;
   SprDCInfo.bmiHeader.biWidth 			= Width;
   SprDCInfo.bmiHeader.biPlanes			= 1;
   SprDCInfo.bmiHeader.biBitCount		= 8;
   SprDCInfo.bmiHeader.biCompression 	= BI_RGB;
   SprDCInfo.bmiHeader.biClrUsed			= 256;
   SprDCInfo.bmiHeader.biClrImportant	= 256;
   SprDCInfo.bmiHeader.biXPelsPerMeter	= 0;
   SprDCInfo.bmiHeader.biYPelsPerMeter	= 0;

	SpriteDC = CreateCompatibleDC(NULL);
   SprDCPal = CreateHalftonePalette(SpriteDC);
	hSprBmp = CreateDIBSection(SpriteDC, (BITMAPINFO *)&SprDCInfo,
   					DIB_PAL_COLORS, &SprDCBits, NULL, 0);

	GetPaletteEntries(SprDCPal, 0, 256, (PALETTEENTRY far *)SprDCInfo.bmiColors);
	for(int Counter = 0; Counter < 256; Counter++)
	{
		BYTE Temp = SprDCInfo.bmiColors[Counter].rgbBlue;
		SprDCInfo.bmiColors[Counter].rgbBlue = SprDCInfo.bmiColors[Counter].rgbRed;
		SprDCInfo.bmiColors[Counter].rgbRed = Temp;
	}
   hOldBmp = (HBITMAP)SelectObject(SpriteDC, hSprBmp);
   PatBlt(SpriteDC, 0, 0, SprDCWidth, SprDCHeight, WHITENESS);
};

// ######################################
// #<<<<<<<< Delete Allocations >>>>>>>>#
// ######################################
SPRITEDC::~SPRITEDC()
{
	SelectObject(SpriteDC, hOldBmp);
   DeleteObject(hSprBmp);
   DeleteDC(SpriteDC);
   DeleteObject(SprDCPal);
};

// ######################################
// #<<<<<<<<<< Clear Sprite DC >>>>>>>>>#
// ######################################
int SPRITEDC::Clear()
{
	PatBlt(SpriteDC, 0, 0, SprDCWidth, SprDCHeight, WHITENESS);
   return 0;
};

// ######################################
// #<<<<<<<<<< Clear Sprite DC >>>>>>>>>#
// ######################################
int SPRITEDC::Clear(COLORREF Color)
{
	HBRUSH	Brush, OldBrush;

   Brush = CreateSolidBrush(Color);
   OldBrush = SelectObject(SpriteDC, Brush);

	PatBlt(SpriteDC, 0, 0, SprDCWidth, SprDCHeight, PATCOPY);

   SelectObject(SpriteDC, OldBrush);
   DeleteObject(Brush);
   return 0;
};

// ######################################
// #<<<<<<<< Blt To Sprite DC >>>>>>>>>>#
// ######################################
int SPRITEDC::SpriteBlt(SPRDIB Sprite, int XDest, int YDest)
{
	char	*pDest, *pSrc;
	int	RealWidth;
	int	Width, Height;
	int	ScanX, ScanY;

	RealWidth = Sprite.BitmapInfo.biWidth + Sprite.BitmapInfo.biWidth % 4;
	Width		= Sprite.BitmapInfo.biWidth;
	Height	= Sprite.BitmapInfo.biHeight;
	ScanX = ScanY = 0;

   // Left Clipped
   if(XDest < 0)
   {
		ScanX 	=-	XDest;
		Width 	+= XDest;
      XDest		= 0;
   }
   // Rigth Clipped
   if((XDest + Width) > SprDCWidth)
	{
	  	Width = SprDCWidth - XDest;
   }
   // Top Clipped
   if(YDest < 0)
   {
		ScanY		=- YDest;
      Height 	+= YDest;
      YDest		= 0;
   }
	// Bottom Clipped
   if((YDest + Height) > SprDCHeight)
   {
		Height	-= YDest + Sprite.BitmapInfo.biHeight - SprDCHeight;
   }

   pDest = (char *)SprDCBits 		+ ((YDest * SprDCWidth) + XDest);
   pSrc	= (char *)Sprite.Handle + ((ScanY * RealWidth) + ScanX);

	if((Width > 0) && (Height > 0))
   {
	   for(int H = 0; H < Height; H++)
	   {
			memcpy(pDest, pSrc, sizeof(char) * Width);
	      pDest += SprDCWidth;
	      pSrc	+= RealWidth;
	   }
   }

	return 0;
};

// ######################################
// #<<<< Blt Transparent Sprite >>>>>>>>#
// ######################################
int SPRITEDC::TransBlt(SPRDIB Sprite, int XDest, int YDest, char TransColor)
{
	char	*pDest, *pSrc;
	int	RealWidth;
	int	Width, Height;
	int	ScanX, ScanY;

	RealWidth = Sprite.BitmapInfo.biWidth + Sprite.BitmapInfo.biWidth % 4;
	Width		= Sprite.BitmapInfo.biWidth;
	Height	= Sprite.BitmapInfo.biHeight;
	ScanX = ScanY = 0;

   // Left Clipped
   if(XDest < 0)
   {
		ScanX 	=-	XDest;
		Width 	+= XDest;
      XDest		= 0;
   }
   // Rigth Clipped
   if((XDest + Width) > SprDCWidth)
	{
	  	Width = SprDCWidth - XDest;
   }
   // Top Clipped
   if(YDest < 0)
   {
		ScanY		=- YDest;
      Height 	+= YDest;
      YDest		= 0;
   }
	// Bottom Clipped
   if((YDest + Height) > SprDCHeight)
   {
		Height	-= YDest + Sprite.BitmapInfo.biHeight - SprDCHeight;
   }

   pDest = (char *)SprDCBits 		+ ((YDest * SprDCWidth) + XDest);
   pSrc	= (char *)Sprite.Handle + ((ScanY * RealWidth) + ScanX);

	// Set Destination Bits
	if((Width > 0) && (Height > 0))
   {
		for(int i = 0; i < Height; i++)
		{
			for(int j = 0; j < Width; j++)
	      {
				if(*pSrc != TransColor)
	         	*pDest = *pSrc;

	         pDest++;
	         pSrc++;
			}
			pDest	+= SprDCWidth - Width;
	      pSrc 	+= RealWidth - Width;
   	}
   }
   return 0;
};

// ######################################
// #<<<< Stretch Sprite DC To Screen >>>#
// ######################################
int SPRITEDC::StretchToScreen(HDC Screen, int DestX, int DestY, int DestW, int DestH,
					int SrcX, int SrcY, int SrcW, int SrcH)
{
	StretchBlt(Screen, DestX, DestY, DestW, DestH, SpriteDC, SrcX, SrcY, SrcW, SrcH, SRCCOPY);
   return 0;
};

// ######################################
// #<<<<<< Blt Sprite DC To Screen >>>>>#
// ######################################
int SPRITEDC::SprToScreen(HDC Screen, int X, int Y)
{
	BitBlt(Screen, X, Y, SprDCWidth, SprDCHeight, SpriteDC, 0, 0, SRCCOPY);
   return 0;
};

// ######################################
// #<<<<<<<<< Fade Palette In >>>>>>>>>>#
// ######################################
int SPRITEDC::FadeIn(HDC Screen, int Loops)
{
	PALETTEENTRY	pe[256];
   HPALETTE		Pal, OldPal;
	RGBQUAD		rgbOriginal[256], rgb[256];

	// Get Original Sprite DC Color Table
   GetDIBColorTable(SpriteDC, 0, 256, rgbOriginal);

	//	Set Non-Static Intries to Black
	for(int i = 0; i < 256; i++)
   {
	     	rgb[i].rgbRed			= 0;
			rgb[i].rgbGreen		= 0;
			rgb[i].rgbBlue			= 0;
			rgb[i].rgbReserved 	= 0;
   }
   // Inforce New Color Table
	SetDIBColorTable(SpriteDC, 0, 256, rgb);

   // Create Palette With Colors
	Pal = CreateIdentityPalette(rgb, PAL_STATIC);
   OldPal = SelectPalette(Screen, Pal, FALSE);
   RealizePalette(Screen);

	// Get System Palette
   GetSystemPaletteEntries(Screen, 0, 256, pe);

	// Fade Bitmap In
	for(int i = 0; i < Loops; i++)
   {
   	for(int j = 10; j < 246; j++)
      {
      	pe[j].peRed		= (rgbOriginal[j].rgbRed * i) 	/ Loops;
			pe[j].peGreen	= (rgbOriginal[j].rgbGreen * i) 	/ Loops;
         pe[j].peBlue	= (rgbOriginal[j].rgbBlue * i) 	/ Loops;
         pe[j].peFlags	= PC_RESERVED;
		}
      AnimatePalette(Pal, 0, 256, pe);
   }

	// Delete Modified Palette
   SelectPalette(Screen, OldPal, FALSE);
   DeleteObject(Pal);

	// Set Sprite Color Table To Original Colors
   SetDIBColorTable(SpriteDC, 0, 256, rgbOriginal);

	// Create Original Palette
	Pal = CreateIdentityPalette(rgbOriginal, PAL_NOSTATIC);
   OldPal = SelectPalette(Screen, Pal, FALSE);
   RealizePalette(Screen);

	// Delele Palette
   SelectPalette(Screen, OldPal, FALSE);
   DeleteObject(Pal);
   return 0;
};

// ######################################
// #<<<<<<<<< Fade Palette Out >>>>>>>>>#
// ######################################
int SPRITEDC::FadeOut(HDC Screen, int Loops)
{
	PALETTEENTRY	pe[256];
   HPALETTE		Pal, OldPal;
	RGBQUAD		rgbOriginal[256], rgb[256];

	// Get Original Sprite DC Color Table
   GetDIBColorTable(SpriteDC, 0, 256, rgbOriginal);

   // Create Palette With Colors
	Pal = CreateIdentityPalette(rgb, PAL_STATIC);
   OldPal = SelectPalette(Screen, Pal, FALSE);
   RealizePalette(Screen);

	// Get System Palette
   GetSystemPaletteEntries(Screen, 0, 256, pe);

	// Fade Bitmap In
	for(int i = Loops; i > 0; i--)
   {
   	for(int j = 246; j > 10; j--)
      {
      	pe[j].peRed		= (rgbOriginal[j].rgbRed 	* i) 	/ Loops;
			pe[j].peGreen	= (rgbOriginal[j].rgbGreen * i) 	/ Loops;
         pe[j].peBlue	= (rgbOriginal[j].rgbBlue 	* i) 	/ Loops;
         pe[j].peFlags	= PC_RESERVED;
		}
      AnimatePalette(Pal, 0, 256, pe);
   }

	// Delete Modified Palette
   SelectPalette(Screen, OldPal, FALSE);
   DeleteObject(Pal);

	// Set Sprite Color Table To Original Colors
   SetDIBColorTable(SpriteDC, 0, 256, rgbOriginal);

   return 0;
};

// ######################################
// #<<<< Create Identity Palette >>>>>>>#
// ######################################
HPALETTE SPRITEDC::CreateIdentityPalette(RGBQUAD aRGB[], int Usage)
{
	PALETTE	Palette;
	HDC		hDC;
	int i;

	// Set Palette Information
	Palette.Version 			= 0x300;
   Palette.NumberOfEntries = 256;

	// Formal Purposes
	hDC = GetDC(NULL);

	// #############################################
	// System Palette Doesn't Includes Static Colors
   // #############################################
	if(GetSystemPaletteUse(hDC) == SYSPAL_NOSTATIC)
	{
		// Fill Palette With Given Values
		for(i = 0; i < 256; i++)
		{
			Palette.aEntries[i].peRed 		= aRGB[i].rgbRed;
			Palette.aEntries[i].peGreen 	= aRGB[i].rgbGreen;
			Palette.aEntries[i].peBlue 	= aRGB[i].rgbBlue;
			Palette.aEntries[i].peFlags 	= PC_NOCOLLAPSE;
      }
   }
	else
	// #####################################
   // System Palette Includes Static Colors
   // #####################################
   {
		GetSystemPaletteEntries(hDC, 0, 256, Palette.aEntries);
      for(i = 10; i < 246; i++)
      {
      	Palette.aEntries[i].peRed 		= aRGB[i].rgbRed;
         Palette.aEntries[i].peGreen	= aRGB[i].rgbGreen;
         Palette.aEntries[i].peBlue		= aRGB[i].rgbBlue;
         Palette.aEntries[i].peFlags	= PC_RESERVED;
      }
  		for(i = 0; i < 10; i++)
		{
			Palette.aEntries[i].peFlags 			= NULL;
			Palette.aEntries[i + 246].peFlags 	= NULL;
		}
	}

	// Release Device Context
	ReleaseDC(NULL, hDC);

	// Return Palette
	return CreatePalette((LOGPALETTE *)&Palette);
};

// ######################################
// #<<<<<< Realize Halftone Pal >>>>>>>>#
// ######################################
int SPRITEDC::HalftonePalette(HDC hDC)
{
	HPALETTE		Pal, OldPal;
   RGBQUAD		Colors[256];
   BYTE			TempColor;

   Pal 		= CreateHalftonePalette(hDC);
   OldPal	= SelectPalette(hDC, Pal, FALSE);
   RealizePalette(hDC);

   GetPaletteEntries(Pal, 0, 256, (PALETTEENTRY far *)Colors);
   for(int i = 0; i < 256; i++)
   {
   	TempColor 			= Colors[i].rgbBlue;
      Colors[i].rgbBlue = Colors[i].rgbRed;
      Colors[i].rgbRed 	= TempColor;
   }
   SetDIBColorTable(SpriteDC, 0, 256, Colors);
   DeleteObject(OldPal);

   return 0;
};

// ######################################
// #<<<<<<<<<<< Load Bitmap >>>>>>>>>>>>#
// ######################################
HPALETTE SPRITEDC::BitmapPalette(SPRDIB Sprite)
{
	HPALETTE			Pal;

	Pal = CreateIdentityPalette(Sprite.BitmapPal, PAL_NOSTATIC);
	SetDIBColorTable(SpriteDC, 0, 256, Sprite.BitmapPal);
	return Pal;
};

// ######################################
// #<<<<<<<< Set Font Settings >>>>>>>>>#
// ######################################
int SPRITEDC::SetFont(int Width, int Height, COLORREF Color)
{
	FontRec.lfWidth  = Width;
	FontRec.lfHeight = Height;
	FontRec.lfWeight = FW_BOLD;
	SelectObject(SpriteDC, CreateFontIndirect(&FontRec));
	SetTextColor(SpriteDC, Color);
	SetBkMode(SpriteDC, TRANSPARENT);
   return 0;
};

// ######################################
// #<<<<<<<<<< Display Text >>>>>>>>>>>>#
// ######################################
int SPRITEDC::DisplayText(int Number, int X, int Y)
{
	char Buffer[30];
	itoa(Number, Buffer, 10);
	TextOut(SpriteDC, X, Y, Buffer, strlen(Buffer));
	return 0;
};

// ######################################
// #<<<<<<<<<< Display Text >>>>>>>>>>>>#
// ######################################
int SPRITEDC::DisplayText(char *Text, int X, int Y)
{
	TextOut(SpriteDC, X, Y, Text, strlen(Text));
	return 0;
};

// ######################################
// #<<<<<< Delete Font Settings >>>>>>>>#
// ######################################
int SPRITEDC::DeleteFont()
{
	DeleteObject(SelectObject(SpriteDC, CreateFontIndirect(&FontRec)));
	DeleteObject(SelectObject(SpriteDC, GetStockObject(SYSTEM_FONT)));
	return 0;
};

// ######################################
// #<<<<<<<<<<< Load Bitmap >>>>>>>>>>>>#
// ######################################
SPRDIB SPRITEDC::LoadBitmap(char *File)
{
	BITMAPFILEHEADER	FileHeader;
	SPRDIB	TmpDib;
	BYTE		*TempData;
	FILE 		*fp;
	char 		Buff[200];
	char 		*Bits;
	int 		NumColors;
	int		RealWidth;

	// Load Bitmap
	fp = fopen(File, "rb");
   if(!fp)
   {
   	sprintf(Buff, "Could not open %s", File);
      MessageBox(GetFocus(), Buff, "Loading Bitmap...!", MB_OK | MB_ICONEXCLAMATION);
      fclose(fp);
   }

   // Read Bitmap File Header
   fread(&FileHeader, sizeof(BITMAPFILEHEADER), 1, fp);
   if(FileHeader.bfType != TYP_BITMAP)
   {
		sprintf(Buff, "Invalid Bitmap Format!");
      MessageBox(GetFocus(), Buff, "Bitmap File Header...", MB_OK | MB_ICONEXCLAMATION);
      fclose(fp);
   }

	// Read Bitmap Info Header
   fread(&TmpDib.BitmapInfo, sizeof(BITMAPINFOHEADER), 1, fp);

	// Get Real Bitmap Width Bits And Allocate Memory
	RealWidth = TmpDib.BitmapInfo.biWidth + TmpDib.BitmapInfo.biWidth % 4;
	TmpDib.BitmapInfo.biSizeImage = RealWidth * TmpDib.BitmapInfo.biHeight;
	TmpDib.Handle = GlobalAlloc(GPTR, TmpDib.BitmapInfo.biSizeImage);
	Bits = (char *)TmpDib.Handle;

	// Read Bitmap Color Data
	switch(TmpDib.BitmapInfo.biBitCount)
	{
		case 1:
      	NumColors = 2;
         break;
      case 4:
      	NumColors = 16;
         break;
      case 8:
      	NumColors = 256;
			break;
   }
   // Load Bitmap Palette
   fread(TmpDib.BitmapPal, sizeof(RGBQUAD), NumColors, fp);
   TmpDib.BitmapInfo.biClrUsed = NumColors;

	// Read Bitmap Bits
   TempData = new BYTE[TmpDib.BitmapInfo.biSizeImage];
   fread(TempData, sizeof(char), TmpDib.BitmapInfo.biSizeImage, fp);

	// Correct Bitmap Alignment
   for(long h = 0; h < TmpDib.BitmapInfo.biHeight; h++)
   	for(long w = 0; w < RealWidth; w++)
			*(Bits + (h * RealWidth) + w) = TempData[TmpDib.BitmapInfo.biSizeImage - ((h+1) * RealWidth) + w];

	delete[] TempData;
	fclose(fp);
   return TmpDib;
};

// ######################################
// #<<<<<<<<<<<<< Load JCM >>>>>>>>>>>>>#
// ######################################
SPRDIB SPRITEDC::LoadComponent(char *File, char *Component)
{
	JCMFILEHEADER			FileHeader;
	RGBTRIPLE				Palette[256];
	JCMCOMPONENTHEADER	ComponentHeader;
	SPRDIB	TmpDib;
   BYTE		*TempData;
   FILE		*fp;
   char		*Bits;
   int		RealWidth;
   int		i;

	// Open File
   fp = fopen(File, "rb");
   if(!fp)
   {
   	MessageBox(GetFocus(), "Could Not Open File!", "Error", MB_OK);
      fclose(fp);
   }

   // Read JCM File Header
   fread(&FileHeader, sizeof(JCMFILEHEADER), 1, fp);

   // Read Palette
   fread(Palette, sizeof(RGBTRIPLE), FileHeader.jcmNumPalEntries, fp);

   // Read Until Reach Requested Component
	while(!strstr(ComponentHeader.jcmTitle, Component))
	{
		// Read Component Header
		fread(&ComponentHeader, sizeof(JCMCOMPONENTHEADER), 1, fp);
		TmpDib.BitmapInfo.biSizeImage 	= ComponentHeader.jcmSizeImage;
		TmpDib.BitmapInfo.biWidth			= ComponentHeader.jcmWidth;
		TmpDib.BitmapInfo.biHeight		= ComponentHeader.jcmHeight;
		TmpDib.BitmapInfo.biClrUsed		= FileHeader.jcmNumPalEntries;

		switch(FileHeader.jcmNumPalEntries)
		{
			case 16:
           	TmpDib.BitmapInfo.biBitCount = 4;
         case 256:
           	TmpDib.BitmapInfo.biBitCount = 8;
      }

  		// DWord Align
		RealWidth = TmpDib.BitmapInfo.biWidth + TmpDib.BitmapInfo.biWidth % 4;

		// Set Palette
		for(i = 0; i < FileHeader.jcmNumPalEntries; i++)
      {
        	TmpDib.BitmapPal[i].rgbRed	= Palette[i].rgbtRed;
      	TmpDib.BitmapPal[i].rgbGreen	= Palette[i].rgbtGreen;
			TmpDib.BitmapPal[i].rgbBlue	= Palette[i].rgbtBlue;
      }

		// Read Component Bits
		TmpDib.Handle = GlobalAlloc(GPTR, TmpDib.BitmapInfo.biSizeImage);
      Bits = (char *)TmpDib.Handle;
		TempData = new BYTE[TmpDib.BitmapInfo.biSizeImage];
		fread(TempData, sizeof(char), TmpDib.BitmapInfo.biSizeImage, fp);

 		// Correct Bitmap Alignment
		for(long h = 0; h < TmpDib.BitmapInfo.biHeight; h++)
		 	for(long w = 0; w < RealWidth; w++)
				*(Bits + (h * RealWidth) + w) = TempData[TmpDib.BitmapInfo.biSizeImage - ((h+1) * RealWidth) + w];

		delete[] TempData;
   }
	fclose(fp);
   return TmpDib;
};

// ######################################
// #<<<<<<<< Load Components >>>>>>>>>>>#
// ######################################
SPRDIB *SPRITEDC::LoadComponents(char *File)
{
	JCMFILEHEADER			FileHeader;
	RGBTRIPLE				Palette[256];
	JCMCOMPONENTHEADER	ComponentHeader;
	SPRDIB	*TmpDib;
   BYTE		*TempData;
   FILE		*fp;
   char		*Bits;
   int		RealWidth;
   int		i;

	// Open File
   fp = fopen(File, "rb");
   if(!fp)
   {
   	MessageBox(GetFocus(), "Could Not Open File!", "Error", MB_OK);
      fclose(fp);
   }

   // Read JCM File Header
   fread(&FileHeader, sizeof(JCMFILEHEADER), 1, fp);

   // Read Palette
   fread(Palette, sizeof(RGBTRIPLE), FileHeader.jcmNumPalEntries, fp);

	// Allocate Storage
	TmpDib = new SPRDIB[FileHeader.jcmNumBitmaps];
	for(i = 0; i < FileHeader.jcmNumBitmaps; i++)
	{
		// Read Component Header
		fread(&ComponentHeader, sizeof(JCMCOMPONENTHEADER), 1, fp);
		TmpDib[i].BitmapInfo.biSizeImage = ComponentHeader.jcmSizeImage;
		TmpDib[i].BitmapInfo.biWidth		= ComponentHeader.jcmWidth;
		TmpDib[i].BitmapInfo.biHeight		= ComponentHeader.jcmHeight;
		TmpDib[i].BitmapInfo.biClrUsed	= FileHeader.jcmNumPalEntries;

		switch(FileHeader.jcmNumPalEntries)
		{
			case 16:
           	TmpDib[i].BitmapInfo.biBitCount = 4;
         case 256:
           	TmpDib[i].BitmapInfo.biBitCount = 8;
      }

  		// DWord Align
		RealWidth = TmpDib[i].BitmapInfo.biWidth + TmpDib[i].BitmapInfo.biWidth % 4;

		// Set Palette
		for(int Ent = 0; Ent < FileHeader.jcmNumPalEntries; Ent++)
		{
        	TmpDib[i].BitmapPal[Ent].rgbRed		= Palette[Ent].rgbtRed;
			TmpDib[i].BitmapPal[Ent].rgbGreen	= Palette[Ent].rgbtGreen;
			TmpDib[i].BitmapPal[Ent].rgbBlue		= Palette[Ent].rgbtBlue;
			TmpDib[i].BitmapPal[Ent].rgbReserved	= 0;
      }

		// Read Component Bits
		TmpDib[i].Handle = GlobalAlloc(GPTR, TmpDib[i].BitmapInfo.biSizeImage);
		Bits = (char *)TmpDib[i].Handle;
		TempData = new BYTE[TmpDib[i].BitmapInfo.biSizeImage];
		fread(TempData, sizeof(char), TmpDib[i].BitmapInfo.biSizeImage, fp);

		// Correct Bitmap Alignment
		for(long h = 0; h < TmpDib[i].BitmapInfo.biHeight; h++)
		  	for(long w = 0; w < RealWidth; w++)
				*(Bits + (h * RealWidth) + w) = TempData[TmpDib[i].BitmapInfo.biSizeImage - ((h+1) * RealWidth) + w];

		delete[] TempData;
   }
	fclose(fp);
	return TmpDib;
};

// ######################################
// #<<<<<<<<<<< Free Bitmap >>>>>>>>>>>>#
// ######################################
int SPRITEDC::FreeBitmap(SPRDIB Dib)
{
	GlobalFree(Dib.Handle);
   return 0;
};

// ######################################
// #<<<<<<<<< Free Components >>>>>>>>>>#
// ######################################
int SPRITEDC::FreeComponents(SPRDIB *Dib, int Start, int End)
{
	for(int i = Start; i < End; i++)
   	GlobalFree(Dib[i].Handle);

	return 0;
};
