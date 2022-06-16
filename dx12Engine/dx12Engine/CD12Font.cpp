#include "CD12Font.h"

CD12Font::CD12Font()
{
	memset(this, 0x00, sizeof(CD12Font));
}

CD12Font::CD12Font(CErrorLog* el, CVideoDevice* d, CWindow* w, const char* face, float pitch, DWRITE_FONT_WEIGHT weight)
{
	memset(this, 0x00, sizeof(CD12Font));

	errorLog = el;
	
	videoDevice = d;
	
	window = w;

	WCHAR localName[1024];

	GetSystemDefaultLocaleName(localName, 1024);

	hr = videoDevice->d3d11On12->dWriteFactory->CreateTextFormat(MBSToWide(face), nullptr, weight, DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL, pitch, localName, &textFormat);

	if (hr < 0)
	{
		errorLog->WriteError("CD12Font::CD12Font::CreateTextFormat:%s:%s\n", face, errorLog->GetDXGIErrorMessage(hr)->name->GetText());

		return;
	}

	height = textFormat->GetFontSize();

	hr = videoDevice->d3d11On12->d2dDeviceContext->CreateSolidColorBrush(D2D1::ColorF(White.RGBA[0], White.RGBA[1], White.RGBA[2], White.RGBA[3]), &textBrush);

	if (hr < 0)
	{
		errorLog->WriteError("CD12Font::CD12Font::CreateSolidColorBrush:%s:%s\n", face, errorLog->GetDXGIErrorMessage(hr)->name->GetText());
	}
}

CD12Font::~CD12Font()
{
}

void CD12Font::Draw(const char* text, float x, float y, CColor* color)
{
	textRect = D2D1::RectF(x, y, videoDevice->swapChainRenderTargets[0]->rtSize.width, videoDevice->swapChainRenderTargets[0]->rtSize.height);

	textBrush->SetColor(D2D1::ColorF(color->RGBA[0], color->RGBA[1], color->RGBA[2], color->RGBA[3]));

	if (textFormat && textBrush)
	{
		videoDevice->d3d11On12->d2dDeviceContext->DrawText(MBSToWide(text), (UINT32)strlen(text), textFormat.Get(), &textRect, textBrush.Get());
	}
}
