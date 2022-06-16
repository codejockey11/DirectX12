#ifndef _CViewport
#define _CViewport

#include "standard.h"

class CViewport
{
public:

	D3D12_VIEWPORT viewport;

	D3D12_RECT scissorRect;

	CViewport();

	CViewport(float x, float y, float width, float height);

	~CViewport();
};
#endif