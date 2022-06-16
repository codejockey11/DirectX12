#ifndef _CD3D11On12
#define _CD3D11On12

#include "standard.h"

#include "CErrorLog.h"

class CD3D11On12
{
public:

	ComPtr<ID2D1DeviceContext1> d2dDeviceContext;

	ComPtr<IDWriteFactory> dWriteFactory;

	ComPtr<ID3D11On12Device> d3d11On12Device;

	ComPtr<ID3D11DeviceContext> d3d11DeviceContext;

	CD3D11On12();

	CD3D11On12(CErrorLog* el, ComPtr<ID3D12Device> d, ComPtr<ID3D12CommandQueue> q);

	~CD3D11On12();

private:

	HRESULT hr;

	CErrorLog* errorLog;

	ComPtr<ID3D12Device> videoDevice;

	ComPtr<ID3D12CommandQueue> commandQueue;

	ComPtr<ID3D11Device> d3d11Device;

	ComPtr<ID3D11Debug> d3d11Debug;

	ComPtr<IDXGIDevice> dxgiDevice;
	
	ComPtr<ID2D1Factory3> d2dFactory;
	
	ComPtr<ID2D1Device2> d2dDevice;
};
#endif