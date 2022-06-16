#include "CViewport.h"

CViewport::CViewport()
{
	memset(this, 0x00, sizeof(CViewport));
}

CViewport::CViewport(float x, float y, float width, float height)
{
	memset(this, 0x00, sizeof(CViewport));

	viewport.TopLeftX = x;
	viewport.TopLeftY = y;
	viewport.Width = width;
	viewport.Height = height;
	viewport.MinDepth = D3D12_MIN_DEPTH;
	viewport.MaxDepth = D3D12_MAX_DEPTH;

	scissorRect.left = 0;
	scissorRect.top = 0;
	scissorRect.right = (LONG)width;
	scissorRect.bottom = (LONG)height;
}

CViewport::~CViewport()
{
}
