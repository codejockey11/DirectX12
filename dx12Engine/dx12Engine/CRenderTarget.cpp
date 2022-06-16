#include "CRenderTarget.h"

CRenderTarget::CRenderTarget()
{
	memset(this, 0x00, sizeof(CRenderTarget));
}

CRenderTarget::CRenderTarget(CErrorLog* el, ComPtr<ID3D12Device> d)
{
	memset(this, 0x00, sizeof(CRenderTarget));

	errorLog = el;
	
	device = d;

	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAGS::D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE::D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;

	heapDesc.NumDescriptors = 1;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	hr = device->CreateDescriptorHeap(&heapDesc, __uuidof(ID3D12DescriptorHeap), (void**)&heap);

	if (hr < 0)
	{
		errorLog->WriteError("CRenderTarget::CRenderTarget::CreateDescriptorHeap:%s\n", errorLog->GetDXGIErrorMessage(hr)->name->GetText());
	}

	handle = heap->GetCPUDescriptorHandleForHeapStart();

	heap->SetName(L"CRenderTarget::Heap");
}

CRenderTarget::~CRenderTarget()
{
}

void CRenderTarget::SwapChainTarget(ComPtr<IDXGISwapChain3> swapChain, UINT bufferNbr)
{
	hr = swapChain->GetBuffer(bufferNbr, IID_PPV_ARGS(&resource));

	if (hr < 0)
	{
		errorLog->WriteError("CRenderTarget::SwapChainTarget::GetBuffer:%s\n", errorLog->GetDXGIErrorMessage(hr)->name->GetText());
	}

	device->CreateRenderTargetView(resource.Get(), nullptr, handle);

	resource->SetName(L"CRenderTarget::SwapChainTarget");
}

void CRenderTarget::AdditionalTarget(ComPtr<ID3D12Resource> r, UINT64 w, UINT h)
{
	resource = r;

	D3D12_RESOURCE_DESC desc = {};

	desc.MipLevels = 1;
	desc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.Width = w;
	desc.Height = h;
	desc.Flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
	desc.DepthOrArraySize = 1;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Dimension = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_TEXTURE2D;

	D3D12_HEAP_PROPERTIES heapProperties = {};

	heapProperties.Type = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT;
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
	heapProperties.CreationNodeMask = 1;
	heapProperties.VisibleNodeMask = 1;


	clearValue.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;



	hr = device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET, &clearValue, IID_PPV_ARGS(&resource));

	if (hr < 0)
	{
		errorLog->WriteError("CRenderTarget::AdditionalTarget::CreateCommittedResource::resource:%s\n", errorLog->GetDXGIErrorMessage(hr)->name->GetText());

		return;
	}

	resource->SetName(L"CRenderTarget::AdditionalRenderTarget");

	device->CreateRenderTargetView(resource.Get(), nullptr, handle);
}

void CRenderTarget::WrapResource(float dpi, ComPtr<ID3D11On12Device> d2, ComPtr<ID2D1DeviceContext1> dc)
{
	d3d11On12Device = d2;

	d2dDeviceContext = dc;

	D3D11_RESOURCE_FLAGS d3d11Flags = { D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET };

	hr = d3d11On12Device->CreateWrappedResource(resource.Get(), &d3d11Flags, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT, IID_PPV_ARGS(&wrappedBackBuffer));

	// Create a render target for D2D to draw directly to this back buffer.
	hr = wrappedBackBuffer->QueryInterface(surface.GetAddressOf());

	bitmapProperties = D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS::D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS::D2D1_BITMAP_OPTIONS_CANNOT_DRAW, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED), dpi, dpi);

	if (surface)
	{
		hr = d2dDeviceContext->CreateBitmapFromDxgiSurface(surface.Get(), &bitmapProperties, d2dRenderTarget.GetAddressOf());

		if (d2dRenderTarget)
		{
			rtSize = d2dRenderTarget->GetSize();
		}
	}
}

void CRenderTarget::SetPreDrawBarrier()
{
	barrier.Transition.pResource = resource.Get();
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET;
}
