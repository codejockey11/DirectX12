#include "CShaderHeap.h"

CShaderHeap::CShaderHeap()
{
	memset(this, 0x00, sizeof(CShaderHeap));
}

CShaderHeap::CShaderHeap(CGlobalObjects* g, ComPtr<ID3D12GraphicsCommandList> commandList)
{
	memset(this, 0x00, sizeof(CShaderHeap));

	globalObjects = g;

	resources = new CLinkList();

	desc.NumDescriptors = CShaderHeap::HEAP_COUNT;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	hr = globalObjects->videoDevice->device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&heap));

	if (hr < 0)
	{
		globalObjects->errorLog->WriteError("CVideoDevice::CreateRootSignature::CreateDescriptorHeap:%s\n", globalObjects->errorLog->GetDXGIErrorMessage(hr)->name->GetText());

		return;
	}

	heap->SetName(L"CShaderHeap::ShaderHeap");

	floats = new CFloatBuffer(globalObjects, commandList);

	ints = new CIntBuffer(globalObjects, commandList);
}

CShaderHeap::~CShaderHeap()
{
	delete wvpView;

	delete resources;

	delete floats;

	delete ints;
}

void CShaderHeap::SlotResource(UINT slot, CTexture* r)
{
	textureView = new CShaderResourceView(globalObjects, heap->GetCPUDescriptorHandleForHeapStart(), slot, r);

	resources->Add(textureView, 0);
}

void CShaderHeap::SlotResource(UINT slot, CFloatBuffer* r)
{
	wvpView = new CShaderResourceView(globalObjects, heap->GetCPUDescriptorHandleForHeapStart(), slot, r);
}

void CShaderHeap::UpdateResourceBuffers()
{
	floats->constantBuffer->UpdateBuffer((void*)floats->values);

	ints->constantBuffer->UpdateBuffer((void*)ints->values);
}
