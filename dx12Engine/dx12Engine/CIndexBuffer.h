#ifndef _CIndexBuffer
#define _CIndexBuffer

#include "standard.h"

#include "CGlobalObjects.h"

class CIndexBuffer
{
public:

	D3D12_INDEX_BUFFER_VIEW	view;

	ComPtr<ID3D12Resource> indexBuffer;

	UINT count;

	ComPtr<ID3D12GraphicsCommandList> commandList;

	CIndexBuffer();
	
	CIndexBuffer(CGlobalObjects* g, ComPtr<ID3D12GraphicsCommandList> cl, UINT c, void* indicies);

	~CIndexBuffer();

	BYTE CreateResource();

	void LoadBuffer(void* indices);

	void Draw();

private:

	HRESULT hr;

	CGlobalObjects* globalObjects;

	D3D12_HEAP_PROPERTIES heapProperties;

	D3D12_RESOURCE_DESC resourceDesc;

	void* pData;
};
#endif