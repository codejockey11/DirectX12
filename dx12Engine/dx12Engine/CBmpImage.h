#ifndef _CBMPIMAGE
#define _CBMPIMAGE

#include "standard.h"

class CBmpImage
{
public:
		
	BITMAPFILEHEADER bmapHeader;
	BITMAPINFO bmapInfo;
	PALETTEENTRY paletteentry[256];

	INT bytesPerPixel;

	BYTE* pixels;

	CBmpImage();
	
	CBmpImage(const CHAR* fn);

	~CBmpImage();

	VOID WriteBitmapHeader(UINT width, UINT height, FILE* fp);
	
	VOID WriteGreyscalePalette(FILE* fp);

private:

	FILE* fp;

	errno_t err;

	SIZE_T size;
	SIZE_T bytesRead;
	SIZE_T count;
};
#endif