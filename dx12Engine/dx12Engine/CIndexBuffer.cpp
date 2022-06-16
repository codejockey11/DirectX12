#include "CIndexBuffer.h"

CIndexBuffer::CIndexBuffer()
{
	memset(this, 0x00, sizeof(CIndexBuffer));
}

CIndexBuffer::CIndexBuffer(CGlobalObjects* g, ComPtr<ID3D12GraphicsCommandList> cl, UINT c, void* indicies)
{
	memset(this, 0x00, sizeof(CIndexBuffer));

	globalObjects = g;

	commandList = cl;

	count = c;

	if (CIndexBuffer::CreateResource())
	{
		CIndexBuffer::LoadBuffer(indicies);
	}
}

CIndexBuffer::~CIndexBuffer()
{
}

BYTE CIndexBuffer::CreateResource()
{
	heapProperties.Type = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD;
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
	heapProperties.CreationNodeMask = 1;
	heapProperties.VisibleNodeMask = 1;

	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Alignment = 0;
	resourceDesc.Width = count * sizeof(UINT);
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.SampleDesc.Quality = 0;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resourceDesc.Flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;

	hr = globalObjects->videoDevice->device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&indexBuffer));

	if (hr < 0)
	{
		globalObjects->errorLog->WriteError("CIndexBuffer::CreateResource::CreateCommittedResource:%s\n", globalObjects->errorLog->GetDXGIErrorMessage(hr)->name->GetText());

		return FALSE;
	}

	indexBuffer->SetName(L"CIndexBuffer::Resource");

	view.BufferLocation = indexBuffer->GetGPUVirtualAddress();
	view.Format = DXGI_FORMAT::DXGI_FORMAT_R32_UINT;
	view.SizeInBytes = count * sizeof(UINT);

	return TRUE;
}

void CIndexBuffer::LoadBuffer(void* indices)
{
	indexBuffer->Map(0, nullptr, &pData);
	
	memcpy(pData, indices, count * sizeof(UINT));
	
	indexBuffer->Unmap(0, nullptr);
}

void CIndexBuffer::Draw()
{
	commandList->IASetIndexBuffer(&view);
}