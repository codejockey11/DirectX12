#ifndef _CD12Font
#define _CD12Font

#include "standard.h"

#include "CColor.h"
#include "CErrorLog.h"
#include "CVideoDevice.h"
#include "CWindow.h"

class CD12Font
{
public:

	ComPtr<IDWriteTextFormat> textFormat;

	float height;

	CD12Font();

	CD12Font(CErrorLog* el, CVideoDevice* d, CWindow* w, const char* face, float pitch, DWRITE_FONT_WEIGHT weight);

	~CD12Font();

	void Draw(const char* text, float x, float y, CColor* color);

private:

	HRESULT hr;

	CErrorLog* errorLog;

	CVideoDevice* videoDevice;

	CWindow* window;

	ComPtr<ID2D1SolidColorBrush> textBrush;

	D2D1_RECT_F textRect;
};
#endif