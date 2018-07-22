#ifndef UTILS_H
#define UTILS_H
// �����������������������������
// � Dib Defines
// �����������������������������
#define	DALIGN(a)		((a + 3)& ~3)

// �����������������������������
// � Dib Structure
// �����������������������������
typedef struct
{
	BITMAPINFOHEADER	Info;
	RGBQUAD	*Palette;
	HGLOBAL	hBits;

} DIB, *LPDIB;

// �����������������������������
// � Component File Header
// �����������������������������
typedef struct
{
	LONG	jcmNumBitmaps;
	WORD	jcmNumPalEntries;
   WORD	jcmVersion;

} JCMFILEHEADER;

// �����������������������������
// � Component Info Header
// �����������������������������
typedef struct
{
	char	jcmTitle[15];
	LONG	jcmWidth;
	LONG	jcmHeight;
	DWORD	jcmSizeImage;

} JCMCOMPONENTHEADER;

#endif
