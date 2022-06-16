#ifndef _CD12FontManager
#define _CD12FontManager

#include "standard.h"

#include "CD12Font.h"
#include "CLinkList.h"
#include "CVideoDevice.h"
#include "CWindow.h"

class CD12FontManager
{
public:

	CLinkList* fonts;

	CD12FontManager();

	CD12FontManager(CErrorLog* el, CVideoDevice* d, CWindow* w);

	~CD12FontManager();

	void DeleteFont(const char* fn);

	CD12Font* MakeFont(const char* face, float pitch, DWRITE_FONT_WEIGHT weight);

	CD12Font* GetFont(const char* face);

private:

	HRESULT hr;

	CErrorLog* errorLog;

	CVideoDevice* videoDevice;

	CWindow* window;

	CD12Font* font;
};
#endif