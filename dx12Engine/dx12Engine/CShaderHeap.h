#ifndef _CShaderHeap
#define _CShaderHeap

#include "standard.h"

#include "CGlobalObjects.h"
#include "CLinkList.h"
#include "CShaderResourceView.h"

class CShaderHeap
{
public:

	enum
	{
		HEAP_COUNT = 12
	};

	ComPtr<ID3D12DescriptorHeap> heap;

	CLinkList* resources;

	CFloatBuffer* floats;

	CIntBuffer* ints;

	CShaderHeap();

	CShaderHeap(CGlobalObjects* g, ComPtr<ID3D12GraphicsCommandList> commandList);

	void SlotResource(UINT slot, CTexture* r);

	void SlotResource(UINT slot, CFloatBuffer* r);

	void UpdateResourceBuffers();

	~CShaderHeap();

private:

	HRESULT hr;

	CGlobalObjects* globalObjects;

	D3D12_DESCRIPTOR_HEAP_DESC desc;

	CShaderResourceView* textureView;

	CShaderResourceView* wvpView;
};
#endif