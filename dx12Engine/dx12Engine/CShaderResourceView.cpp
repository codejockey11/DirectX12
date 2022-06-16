#include "CShaderResourceView.h"

CShaderResourceView::CShaderResourceView()
{
	memset(this, 0x00, sizeof(CShaderResourceView));
}

CShaderResourceView::CShaderResourceView(CGlobalObjects* g, D3D12_CPU_DESCRIPTOR_HANDLE handle, UINT s, CTexture* r)
{
	memset(this, 0x00, sizeof(CShaderResourceView));

	globalObjects = g;

	slot = s;

	handle.ptr += (slot * globalObjects->videoDevice->device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));

	desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	desc.Format = r->texture->GetDesc().Format;
	desc.ViewDimension = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MipLevels = 1;

	globalObjects->videoDevice->device->CreateShaderResourceView(r->texture.Get(), &desc, handle);

	r->texture->SetName(MBSToWide(r->name->GetText()));
}

CShaderResourceView::CShaderResourceView(CGlobalObjects* g, D3D12_CPU_DESCRIPTOR_HANDLE handle, UINT s, CFloatBuffer* r)
{
	memset(this, 0x00, sizeof(CShaderResourceView));

	globalObjects = g;

	slot = s;

	handle.ptr += (slot * globalObjects->videoDevice->device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));

	desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	desc.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	desc.ViewDimension = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_BUFFER;
	desc.Buffer.FirstElement = 0;
	desc.Buffer.NumElements = r->count;
	desc.Buffer.StructureByteStride = r->stride;
	desc.Buffer.Flags = D3D12_BUFFER_SRV_FLAGS::D3D12_BUFFER_SRV_FLAG_NONE;

	globalObjects->videoDevice->device->CreateShaderResourceView(r->constantBuffer->buffer.Get(), &desc, handle);

	r->constantBuffer->buffer->SetName(L"CShaderResourceView::constantBuffer");
}

CShaderResourceView::~CShaderResourceView()
{
}