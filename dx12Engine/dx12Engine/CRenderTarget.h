#ifndef _CRenderTarget
#define _CRenderTarget

#include "standard.h"

#include "CErrorLog.h"

class CRenderTarget
{
public:

	ComPtr<ID3D12DescriptorHeap> heap;
	
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc;
	
	D3D12_CPU_DESCRIPTOR_HANDLE	handle;

	D2D1_SIZE_F rtSize;

	ComPtr<ID3D12Resource> resource;

	ComPtr<ID2D1Bitmap1> d2dRenderTarget;

	ComPtr<ID3D11Resource> wrappedBackBuffer;

	D3D12_RESOURCE_BARRIER barrier;

	D3D12_CLEAR_VALUE clearValue;

	CRenderTarget();

	CRenderTarget(CErrorLog* el, ComPtr<ID3D12Device> d);

	~CRenderTarget();

	void SwapChainTarget(ComPtr<IDXGISwapChain3> swapChain, UINT bufferNbr);

	void AdditionalTarget(ComPtr<ID3D12Resource> r, UINT64 w, UINT h);

	void WrapResource(float dpi, ComPtr<ID3D11On12Device> d2, ComPtr<ID2D1DeviceContext1> dc);

	void SetPreDrawBarrier();

private:

	HRESULT hr;

	CErrorLog* errorLog;

	ComPtr<ID3D12Device> device;

	ComPtr<ID3D11On12Device> d3d11On12Device;

	ComPtr<ID2D1DeviceContext1> d2dDeviceContext;

	ComPtr<IDXGISurface> surface;

	D2D1_BITMAP_PROPERTIES1 bitmapProperties;
};
#endif