#include "CBmpImage.h"

CBmpImage::CBmpImage()
{
	memset(this, 0x00, sizeof(CBmpImage));
}

CBmpImage::CBmpImage(const CHAR* fn)
{
	memset(this, 0x00, sizeof(CBmpImage));

	err = fopen_s(&fp, fn, "rb");

	if (err)
	{
		return;
	}

	bytesRead = fread_s(&bmapHeader.bfType, sizeof(WORD), sizeof(WORD), 1, fp);
	bytesRead = fread_s(&bmapHeader.bfSize, sizeof(DWORD), sizeof(DWORD), 1, fp);
	bytesRead = fread_s(&bmapHeader.bfReserved1, sizeof(WORD), sizeof(WORD), 1, fp);
	bytesRead = fread_s(&bmapHeader.bfReserved2, sizeof(WORD), sizeof(WORD), 1, fp);
	bytesRead = fread_s(&bmapHeader.bfOffBits, sizeof(DWORD), sizeof(DWORD), 1, fp);

	bytesRead = fread_s(&bmapInfo.bmiHeader.biSize, sizeof(DWORD), sizeof(DWORD), 1, fp);
	bytesRead = fread_s(&bmapInfo.bmiHeader.biWidth, sizeof(LONG), sizeof(LONG), 1, fp);
	bytesRead = fread_s(&bmapInfo.bmiHeader.biHeight, sizeof(LONG), sizeof(LONG), 1, fp);
	bytesRead = fread_s(&bmapInfo.bmiHeader.biPlanes, sizeof(WORD), sizeof(WORD), 1, fp);
	bytesRead = fread_s(&bmapInfo.bmiHeader.biBitCount, sizeof(WORD), sizeof(WORD), 1, fp);

	bytesPerPixel = bmapInfo.bmiHeader.biBitCount / 8;

	bytesRead = fread_s(&bmapInfo.bmiHeader.biCompression, sizeof(DWORD), sizeof(DWORD), 1, fp);

	// only uncompressed images
	if (bmapInfo.bmiHeader.biCompression != BI_RGB)
	{
		fclose(fp);

		return;
	}

	bytesRead = fread_s(&bmapInfo.bmiHeader.biSizeImage, sizeof(DWORD), sizeof(DWORD), 1, fp);
	bytesRead = fread_s(&bmapInfo.bmiHeader.biXPelsPerMeter, sizeof(LONG), sizeof(LONG), 1, fp);
	bytesRead = fread_s(&bmapInfo.bmiHeader.biYPelsPerMeter, sizeof(LONG), sizeof(LONG), 1, fp);
	bytesRead = fread_s(&bmapInfo.bmiHeader.biClrUsed, sizeof(DWORD), sizeof(DWORD), 1, fp);
	bytesRead = fread_s(&bmapInfo.bmiHeader.biClrImportant, sizeof(DWORD), sizeof(DWORD), 1, fp);

	if (bmapInfo.bmiHeader.biBitCount == 8)
	{
		bytesRead = fread_s(&paletteentry, 256 * sizeof(PALETTEENTRY), sizeof(PALETTEENTRY), 256, fp);
	}

	size = (SIZE_T)bmapInfo.bmiHeader.biWidth * bmapInfo.bmiHeader.biHeight * bytesPerPixel;

	pixels = (BYTE*)malloc(size);

	if (pixels)
	{
		memset(pixels, 0x00, size);

		pixels += (size - ((SIZE_T)bmapInfo.bmiHeader.biWidth * bytesPerPixel));

		fseek(fp, 0, SEEK_SET);
		fseek(fp, bmapHeader.bfOffBits, SEEK_SET);

		count = 0;

		while (count < size)
		{
			bytesRead = fread_s(pixels, size, ((SIZE_T)bmapInfo.bmiHeader.biWidth * bytesPerPixel), 1, fp);

			pixels -= ((SIZE_T)bmapInfo.bmiHeader.biWidth * bytesPerPixel);

			count += bmapInfo.bmiHeader.biWidth * bytesPerPixel;
		}

		pixels += ((SIZE_T)bmapInfo.bmiHeader.biWidth * bytesPerPixel);
	}

	fclose(fp);
}

CBmpImage::~CBmpImage()
{
	if (pixels)
	{
		free(pixels);
	}
}

VOID CBmpImage::WriteBitmapHeader(UINT width, UINT height, FILE* fp)
{
	BITMAPFILEHEADER	bmapHeader;
	BITMAPINFO		bmapInfo;

	bmapHeader.bfType = 19778;
	fwrite(&bmapHeader.bfType, sizeof(WORD), 1, fp);

	bmapHeader.bfSize = 0;
	fwrite(&bmapHeader.bfSize, sizeof(DWORD), 1, fp);

	bmapHeader.bfReserved1 = 0;
	fwrite(&bmapHeader.bfReserved1, sizeof(WORD), 1, fp);

	bmapHeader.bfReserved2 = 0;
	fwrite(&bmapHeader.bfReserved2, sizeof(WORD), 1, fp);

	bmapHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + (256 * sizeof(PALETTEENTRY));
	fwrite(&bmapHeader.bfOffBits, sizeof(DWORD), 1, fp);

	bmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	fwrite(&bmapInfo.bmiHeader.biSize, sizeof(DWORD), 1, fp);

	bmapInfo.bmiHeader.biWidth = width;
	fwrite(&bmapInfo.bmiHeader.biWidth, sizeof(LONG), 1, fp);

	bmapInfo.bmiHeader.biHeight = height;
	fwrite(&bmapInfo.bmiHeader.biHeight, sizeof(LONG), 1, fp);

	bmapInfo.bmiHeader.biPlanes = 1;
	fwrite(&bmapInfo.bmiHeader.biPlanes, sizeof(WORD), 1, fp);

	bmapInfo.bmiHeader.biBitCount = 8;
	fwrite(&bmapInfo.bmiHeader.biBitCount, sizeof(WORD), 1, fp);

	bmapInfo.bmiHeader.biCompression = BI_RGB;
	fwrite(&bmapInfo.bmiHeader.biCompression, sizeof(DWORD), 1, fp);

	bmapInfo.bmiHeader.biSizeImage = bmapInfo.bmiHeader.biWidth * bmapInfo.bmiHeader.biHeight;
	fwrite(&bmapInfo.bmiHeader.biSizeImage, sizeof(DWORD), 1, fp);

	bmapInfo.bmiHeader.biXPelsPerMeter = 0;
	fwrite(&bmapInfo.bmiHeader.biXPelsPerMeter, sizeof(LONG), 1, fp);

	bmapInfo.bmiHeader.biYPelsPerMeter = 0;
	fwrite(&bmapInfo.bmiHeader.biYPelsPerMeter, sizeof(LONG), 1, fp);

	bmapInfo.bmiHeader.biClrUsed = 0;
	fwrite(&bmapInfo.bmiHeader.biClrUsed, sizeof(DWORD), 1, fp);

	bmapInfo.bmiHeader.biClrImportant = 0;
	fwrite(&bmapInfo.bmiHeader.biClrImportant, sizeof(DWORD), 1, fp);
}

VOID CBmpImage::WriteGreyscalePalette(FILE* fp)
{
	PALETTEENTRY paletteentry;

	for (UINT i = 0; i < 256; i++)
	{
		paletteentry.peRed = i;
		paletteentry.peGreen = i;
		paletteentry.peBlue = i;
		paletteentry.peFlags = 0;

		fwrite(&paletteentry, sizeof(PALETTEENTRY), 1, fp);
	}
}