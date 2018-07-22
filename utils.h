#ifndef UTILS_H
#define UTILS_H
// žžžžžžžžžžžžžžžžžžžžžžžžžžžžž
// ž Dib Defines
// žžžžžžžžžžžžžžžžžžžžžžžžžžžžž
#define	DALIGN(a)		((a + 3)& ~3)

// žžžžžžžžžžžžžžžžžžžžžžžžžžžžž
// ž Dib Structure
// žžžžžžžžžžžžžžžžžžžžžžžžžžžžž
typedef struct
{
	BITMAPINFOHEADER	Info;
	RGBQUAD	*Palette;
	HGLOBAL	hBits;

} DIB, *LPDIB;

// žžžžžžžžžžžžžžžžžžžžžžžžžžžžž
// ž Component File Header
// žžžžžžžžžžžžžžžžžžžžžžžžžžžžž
typedef struct
{
	LONG	jcmNumBitmaps;
	WORD	jcmNumPalEntries;
   WORD	jcmVersion;

} JCMFILEHEADER;

// žžžžžžžžžžžžžžžžžžžžžžžžžžžžž
// ž Component Info Header
// žžžžžžžžžžžžžžžžžžžžžžžžžžžžž
typedef struct
{
	char	jcmTitle[15];
	LONG	jcmWidth;
	LONG	jcmHeight;
	DWORD	jcmSizeImage;

} JCMCOMPONENTHEADER;

#endif
