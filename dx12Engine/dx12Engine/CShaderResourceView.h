#ifndef _CShaderResourceView
#define _CShaderResourceView

#include "standard.h"

#include "CFloatBuffer.h"
#include "CGlobalObjects.h"
#include "CTexture.h"

class CShaderResourceView
{
public:

	enum
	{
		TEXTURE = 0,
		BUFFER,

		VIEW_COUNT = 2
	};

	BYTE type;

	UINT64 slot;

	UINT elements;

	CShaderResourceView();

	CShaderResourceView(CGlobalObjects* g, D3D12_CPU_DESCRIPTOR_HANDLE handle, UINT s, CTexture* r);

	CShaderResourceView(CGlobalObjects* g, D3D12_CPU_DESCRIPTOR_HANDLE handle, UINT s, CFloatBuffer* r);

	~CShaderResourceView();

private:

	HRESULT hr;

	CGlobalObjects* globalObjects;

	D3D12_SHADER_RESOURCE_VIEW_DESC desc;
};
#endif