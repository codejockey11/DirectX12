#include "CD12FontManager.h"

CD12FontManager::CD12FontManager()
{
	memset(this, 0x00, sizeof(CD12FontManager));
}

CD12FontManager::CD12FontManager(CErrorLog* el, CVideoDevice* d, CWindow* w)
{
	memset(this, 0x00, sizeof(CD12FontManager));

	errorLog = el;
	
	videoDevice = d;
	
	window = w;

	fonts = new CLinkList();
}

CD12FontManager::~CD12FontManager()
{
	CLinkListNode* lln = fonts->list;

	while (lln)
	{
		CD12Font* e = (CD12Font*)lln->element;
		
		delete e;

		lln = lln->next;
	}

	fonts = nullptr;
}

CD12Font* CD12FontManager::MakeFont(const char* face, float pitch, DWRITE_FONT_WEIGHT weight)
{
	CD12Font* fc = CD12FontManager::GetFont(face);

	if (fc)
	{
		return fc;
	}

	font = new CD12Font(errorLog, videoDevice, window, face, pitch, weight);

	if (font->textFormat)
	{
		fonts->Add(font, face);

		return font;
	}

	errorLog->WriteError("CD12FontManager::CD12Font:Failed:%s\n", face);

	delete font;

	return nullptr;
}

CD12Font* CD12FontManager::GetFont(const char* face)
{
	CLinkListNode* lln = fonts->Search(face);

	if (lln)
	{
		return (CD12Font*)lln->element;
	}

	return nullptr;
}

void CD12FontManager::DeleteFont(const char* face)
{
	CLinkListNode* lln = fonts->Search(face);

	if (lln)
	{
		fonts->Delete(lln);
	}
}