#include "CConstantBuffer.h"

CConstantBuffer::CConstantBuffer()
{
	memset(this, 0x00, sizeof(CConstantBuffer));
}

CConstantBuffer::CConstantBuffer(CGlobalObjects* g, ComPtr<ID3D12GraphicsCommandList> cl, UINT size)
{
	memset(this, 0x00, sizeof(CConstantBuffer));

	globalObjects = g;

	bufferSize = size;

	commandList = cl;

	bufferDesc.Dimension = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_BUFFER;
	bufferDesc.Alignment = 0;
	bufferDesc.Width = bufferSize;
	bufferDesc.Height = 1;
	bufferDesc.DepthOrArraySize = 1;
	bufferDesc.MipLevels = 1;
	bufferDesc.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	bufferDesc.SampleDesc.Count = 1;
	bufferDesc.SampleDesc.Quality = 0;
	bufferDesc.Layout = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	bufferDesc.Flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;

	bufferHeapProperties.Type = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT;
	bufferHeapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	bufferHeapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
	bufferHeapProperties.CreationNodeMask = 1;
	bufferHeapProperties.VisibleNodeMask = 1;

	hr = globalObjects->videoDevice->device->CreateCommittedResource(&bufferHeapProperties, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &bufferDesc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, nullptr, IID_PPV_ARGS(&buffer));

	if (hr < 0)
	{
		globalObjects->errorLog->WriteError("CConstantBuffer::CConstantBuffer::CreateCommittedResource::buffer:%s\n", globalObjects->errorLog->GetDXGIErrorMessage(hr)->name->GetText());

		return;
	}

	buffer->SetName(L"CConstantBuffer::constantBuffer");
	
	// Create the GPU upload buffer.	
	globalObjects->videoDevice->device->GetCopyableFootprints(&bufferDesc, 0, 1, 0, nullptr, nullptr, nullptr, &uploadBufferSize);

	allocInfo.SizeInBytes = uploadBufferSize;
	allocInfo.Alignment = 0;

	heapDesc.Dimension = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_BUFFER;
	heapDesc.Alignment = allocInfo.Alignment;
	heapDesc.Width = uploadBufferSize;
	heapDesc.Height = 1;
	heapDesc.DepthOrArraySize = 1;
	heapDesc.MipLevels = 1;
	heapDesc.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	heapDesc.SampleDesc.Count = 1;
	heapDesc.SampleDesc.Quality = 0;
	heapDesc.Layout = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	heapDesc.Flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;

	heapProperties.Type = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD;
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
	heapProperties.CreationNodeMask = 1;
	heapProperties.VisibleNodeMask = 1;

	hr = globalObjects->videoDevice->device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &heapDesc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&uploadHeap));

	if (hr < 0)
	{
		globalObjects->errorLog->WriteError("CConstantBuffer::CConstantBuffer::CreateCommittedResource::uploadHeap:%s\n", globalObjects->errorLog->GetDXGIErrorMessage(hr)->name->GetText());

		return;
	}

	uploadHeap->SetName(L"CConstantBuffer::UploadHeap");
}

CConstantBuffer::~CConstantBuffer()
{
}

void CConstantBuffer::UpdateBuffer(void* values)
{
	barrier.Transition.pResource = buffer.Get();
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST;

	commandList->ResourceBarrier(1, &barrier);


	hr = uploadHeap->Map(0, nullptr, &pDest);

	if (hr < 0)
	{
		globalObjects->errorLog->WriteError("CConstantBuffer::UpdateBuffer::uploadHeap->Map:%s\n", globalObjects->errorLog->GetDXGIErrorMessage(hr)->name->GetText());

		return;
	}

	memcpy(pDest, values, bufferSize);

	uploadHeap->Unmap(0, nullptr);


	commandList->CopyResource(buffer.Get(), uploadHeap.Get());

	
	
	barrier.Transition.pResource	= buffer.Get();
	barrier.Transition.StateBefore	= D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter	= D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;

	commandList->ResourceBarrier(1, &barrier);
}