#ifndef _CDepthBuffer
#define _CDepthBuffer

#include "standard.h"

#include "CErrorlog.h"

class CDepthBuffer
{
public:

	ComPtr<ID3D12Resource> resource;

	D3D12_RESOURCE_DESC desc;

	D3D12_CLEAR_VALUE clearValue;

	D3D12_DESCRIPTOR_HEAP_DESC heapDesc;

	ComPtr<ID3D12DescriptorHeap> heap;

	D3D12_RESOURCE_BARRIER barrier;

	D3D12_CPU_DESCRIPTOR_HANDLE handle;

	CDepthBuffer();

	CDepthBuffer(CErrorLog* el, ComPtr<ID3D12Device> d, UINT width, UINT height);

	~CDepthBuffer();

	void SetPreDrawBarrier();

private:

	HRESULT hr;

	CErrorLog* errorLog;

	ComPtr<ID3D12Device> device;

	D3D12_HEAP_PROPERTIES heapProperties;
};
#endif