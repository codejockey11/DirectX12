#ifndef _CConstantBuffer
#define _CConstantBuffer

#include "standard.h"

#include "CGlobalObjects.h"

class CConstantBuffer
{
public:

	ComPtr<ID3D12Resource> buffer;

	ComPtr<ID3D12GraphicsCommandList> commandList;

	D3D12_RESOURCE_BARRIER barrier;

	CConstantBuffer();

	CConstantBuffer(CGlobalObjects* g, ComPtr<ID3D12GraphicsCommandList> cl, UINT size);

	~CConstantBuffer();

	void UpdateBuffer(void* values);

private:

	HRESULT hr;

	CGlobalObjects* globalObjects;

	
	D3D12_RESOURCE_DESC bufferDesc;

	D3D12_HEAP_PROPERTIES bufferHeapProperties;


	D3D12_RESOURCE_DESC heapDesc;

	D3D12_HEAP_PROPERTIES heapProperties;

	ComPtr<ID3D12Resource> uploadHeap;


	UINT64 uploadBufferSize;

	D3D12_RESOURCE_ALLOCATION_INFO allocInfo;

	D3D12_SUBRESOURCE_DATA uploadData;

	UINT bufferSize;

	void* pDest;
};
#endif