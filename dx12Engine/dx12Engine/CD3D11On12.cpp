#include "CD3D11On12.h"

CD3D11On12::CD3D11On12()
{
	memset(this, 0x00, sizeof(CD3D11On12));
}

CD3D11On12::CD3D11On12(CErrorLog* el, ComPtr<ID3D12Device> d, ComPtr<ID3D12CommandQueue> q)
{
	memset(this, 0x00, sizeof(CD3D11On12));

	errorLog = el;

	videoDevice = d;

	commandQueue = q;

	UINT d3d11DeviceFlags = D3D11_CREATE_DEVICE_FLAG::D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#ifdef _DEBUG
	d3d11DeviceFlags |= D3D11_CREATE_DEVICE_FLAG::D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D11On12CreateDevice(videoDevice.Get(), d3d11DeviceFlags,
		nullptr,
		0,
		(IUnknown**)commandQueue.GetAddressOf(),
		1,
		0,
		d3d11Device.GetAddressOf(),
		d3d11DeviceContext.GetAddressOf(),
		nullptr
	);

#ifdef _DEBUG
	
	hr = d3d11Device->QueryInterface(IID_PPV_ARGS(&d3d11Debug));
	
	d3d11Debug->ReportLiveDeviceObjects(D3D11_RLDO_FLAGS::D3D11_RLDO_DETAIL | D3D11_RLDO_FLAGS::D3D11_RLDO_IGNORE_INTERNAL);

#endif

	d3d11Device->QueryInterface(IID_PPV_ARGS(&d3d11On12Device));

	D2D1_DEVICE_CONTEXT_OPTIONS deviceOptions = D2D1_DEVICE_CONTEXT_OPTIONS::D2D1_DEVICE_CONTEXT_OPTIONS_NONE;

	D2D1_FACTORY_OPTIONS d2dFactoryOptions = {};

#ifdef _DEBUG
	d2dFactoryOptions.debugLevel = D2D1_DEBUG_LEVEL::D2D1_DEBUG_LEVEL_INFORMATION;
#endif
		
	D2D1CreateFactory(D2D1_FACTORY_TYPE::D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory3), &d2dFactoryOptions, (void**)&d2dFactory);

	d3d11On12Device->QueryInterface(IID_PPV_ARGS(&dxgiDevice));

	if (dxgiDevice)
	{
		d2dFactory->CreateDevice(dxgiDevice.Get(), d2dDevice.GetAddressOf());

		d2dDevice->CreateDeviceContext(deviceOptions, d2dDeviceContext.GetAddressOf());

		DWriteCreateFactory(DWRITE_FACTORY_TYPE::DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**)dWriteFactory.GetAddressOf());
	}
}

CD3D11On12::~CD3D11On12()
{
}