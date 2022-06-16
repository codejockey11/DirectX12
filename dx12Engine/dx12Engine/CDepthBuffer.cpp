#include "CDepthBuffer.h"

CDepthBuffer::CDepthBuffer()
{
	memset(this, 0x00, sizeof(CDepthBuffer));
}

CDepthBuffer::CDepthBuffer(CErrorLog* el, ComPtr<ID3D12Device> d, UINT width, UINT height)
{
	memset(this, 0x00, sizeof(CDepthBuffer));

	errorLog = el;

	device = d;

	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	heapDesc.NumDescriptors = 1;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	heapDesc.NodeMask = 0;

	hr = device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&heap));

	if (hr < 0)
	{
		errorLog->WriteError("CDepthBuffer::CDepthBuffer::CreateDescriptorHeap:%s\n", errorLog->GetDXGIErrorMessage(hr)->name->GetText());

		return;
	}

	heap->SetName(L"CDepthBuffer::DescriptorHeap");

	handle = heap->GetCPUDescriptorHandleForHeapStart();


	heapProperties.Type = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT;

	desc.Dimension = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	desc.Alignment = 0;
	desc.Width = width;
	desc.Height = height;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.Format = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Layout = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_UNKNOWN;
	desc.Flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL | D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE;

	clearValue.Format = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
	clearValue.DepthStencil.Depth = 1.0F;
	//clearValue.DepthStencil.Stencil = 0;


	hr = device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_DEPTH_WRITE, &clearValue, IID_PPV_ARGS(&resource));

	if (hr < 0)
	{
		errorLog->WriteError("CDepthBuffer::CDepthBuffer::CreateCommittedResource:%s\n", errorLog->GetDXGIErrorMessage(hr)->name->GetText());

		return;
	}

	resource->SetName(L"CDepthBuffer::Resource");



	device->CreateDepthStencilView(resource.Get(), nullptr, handle);

	barrier.Transition.pResource = resource.Get();
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_DEPTH_WRITE;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_DEPTH_READ;
}

CDepthBuffer::~CDepthBuffer()
{
}

void CDepthBuffer::SetPreDrawBarrier()
{
	barrier.Transition.pResource = resource.Get();
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_DEPTH_WRITE;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_DEPTH_READ;
}
