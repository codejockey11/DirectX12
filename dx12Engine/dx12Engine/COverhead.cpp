#include "COverhead.h"

COverhead::COverhead()
{
	memset(this, 0x00, sizeof(COverhead));
}

COverhead::COverhead(CGlobalObjects* g, CVertex2* wh, CVertex2* p)
{
	memset(this, 0x00, sizeof(COverhead));

	globalObjects = g;

	for (UINT i = 0; i < CVideoDevice::FRAME_COUNT; i++)
	{
		commandAllocators[i] = globalObjects->videoDevice->CreateCommandAllocator();

		renderTargets[i] = new CRenderTarget(globalObjects->errorLog, globalObjects->videoDevice->device);

		renderTargets[i]->AdditionalTarget(resources[i], (UINT64)wh->p.x, (UINT)wh->p.y);

		depthBuffers[i] = new CDepthBuffer(globalObjects->errorLog, globalObjects->videoDevice->device, (UINT)wh->p.x, (UINT)wh->p.y);
	}

	commandList = globalObjects->videoDevice->CreateCommandList(commandAllocators[0]);

	commandList->SetName(L"COverhead::COverhead");



	texture = new CDynamicTexture(globalObjects->errorLog, globalObjects->videoDevice, commandList, 256, 256, "CDynamicTexture::Overhead");


	float xScale = (globalObjects->videoDevice->swapChainViewport->viewport.Width > 0) ? 2.0f / globalObjects->videoDevice->swapChainViewport->viewport.Width : 0.0f;
	float yScale = (globalObjects->videoDevice->swapChainViewport->viewport.Height > 0) ? 2.0f / globalObjects->videoDevice->swapChainViewport->viewport.Height : 0.0f;

	XMMATRIX wvpMat
	(
		xScale, 0, 0, 0,
		0, -yScale, 0, 0,
		0, 0, 1, 0,
		-1, 1, 0, 1
	);

	wvp = new CFloat4x4Buffer(globalObjects, commandList);

	XMStoreFloat4x4(&wvp->values, wvpMat);



	shaderHeap = new CShaderHeap(globalObjects, commandList);

	shaderHeap->SlotResource(0, wvp);

	shaderHeap->SlotResource(1, texture);


	range = new CRange();

	range->SetRange(0, D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1);

	range->SetRange(1, D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1);

	rootSignature = new CRootSignature(globalObjects, 2, range);


	vertexShader = globalObjects->shaderMgr->MakeShader("vertexSprite.hlsl", "VSMain", "vs_5_0");

	pixelShader = globalObjects->shaderMgr->MakeShader("pixelSprite.hlsl", "PSMain", "ps_5_0");


	pipelineState = new CPipelineState(globalObjects, CVertex::VT_VERTEXT);

	pipelineState->SetDesc(FALSE, TRUE, FALSE, D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, vertexShader, pixelShader, nullptr, nullptr, nullptr, rootSignature->signature);


	scaleSize = *wh;

	position = *p;

	vertices[0].p.x = 0.0F;
	vertices[0].p.y = 0.0F;

	vertices[0].uv.x = 0.0F;
	vertices[0].uv.y = 0.0F;

	vertices[1].p.x = scaleSize.p.x;
	vertices[1].p.y = 0.0F;

	vertices[1].uv.x = 1.0F;
	vertices[1].uv.y = 0.0F;

	vertices[2].p.x = scaleSize.p.x;
	vertices[2].p.y = scaleSize.p.y;

	vertices[2].uv.x = 1.0F;
	vertices[2].uv.y = 1.0F;

	vertices[3].p.x = 0.0F;
	vertices[3].p.y = scaleSize.p.y;

	vertices[3].uv.x = 0.0F;
	vertices[3].uv.y = 1.0F;

	vertexBuffer = new CVertexBuffer(globalObjects, commandList, CVertex::VT_VERTEXT, 4, (void*)vertices);

	index[0] = 0;
	index[1] = 1;
	index[2] = 2;
	index[3] = 0;
	index[4] = 2;
	index[5] = 3;

	indexBuffer = new CIndexBuffer(globalObjects, commandList, 6, index);

	COverhead::UpdatePosition();

	viewport = new CViewport(0.0F, 0.0F, scaleSize.p.x, scaleSize.p.y);

	camera = new CCamera(scaleSize.p.x, scaleSize.p.y);

	camera->position.y = 128.0F;
	camera->UpdateRotation(90.0F, 0.0F, 0.0F);
	camera->UpdateView();
}

COverhead::~COverhead()
{
	delete wvp;

	delete vertexBuffer;

	delete indexBuffer;

	delete shaderHeap;

	delete range;

	delete pipelineState;

	delete rootSignature;

	delete texture;

	for (UINT i = 0; i < CVideoDevice::FRAME_COUNT; i++)
	{
		delete renderTargets[i];

		delete depthBuffers[i];
	}

	delete viewport;

	delete camera;
}

void COverhead::UpdatePosition()
{
	vertices[0].p.x = position.p.x - (scaleSize.p.x / 2.0F);
	vertices[1].p.x = position.p.x + (scaleSize.p.x / 2.0F);
	vertices[2].p.x = position.p.x + (scaleSize.p.x / 2.0F);
	vertices[3].p.x = position.p.x - (scaleSize.p.x / 2.0F);

	vertices[0].p.y = position.p.y - (scaleSize.p.y / 2.0F);
	vertices[1].p.y = position.p.y - (scaleSize.p.y / 2.0F);
	vertices[2].p.y = position.p.y + (scaleSize.p.y / 2.0F);
	vertices[3].p.y = position.p.y + (scaleSize.p.y / 2.0F);

	if ((vertices[1].p.x < 0.0F) || (vertices[3].p.y < 0.0F) || (vertices[0].p.x > globalObjects->window->width) || (vertices[1].p.y > globalObjects->window->height))
	{
		return;
	}

	vertexBuffer->LoadBuffer((void*)vertices);
}

void COverhead::Draw()
{
	commandAllocators[globalObjects->videoDevice->frameIndex]->Reset();

	commandList->Reset(commandAllocators[globalObjects->videoDevice->frameIndex].Get(), nullptr);


	renderTargets[globalObjects->videoDevice->frameIndex]->barrier.Transition.pResource = renderTargets[globalObjects->videoDevice->frameIndex]->resource.Get();
	renderTargets[globalObjects->videoDevice->frameIndex]->barrier.Transition.StateBefore = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET;
	renderTargets[globalObjects->videoDevice->frameIndex]->barrier.Transition.StateAfter = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_SOURCE;

	commandList->ResourceBarrier(1, &renderTargets[globalObjects->videoDevice->frameIndex]->barrier);

	commandList->CopyResource(texture->texture.Get(), renderTargets[globalObjects->videoDevice->frameIndex]->resource.Get());

	texture->barrier.Transition.pResource = texture->texture.Get();
	texture->barrier.Transition.StateBefore = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST;
	texture->barrier.Transition.StateAfter = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

	commandList->ResourceBarrier(1, &texture->barrier);



	wvp->constantBuffer->UpdateBuffer((void*)&wvp->values);



	commandList->RSSetViewports(1, &globalObjects->videoDevice->swapChainViewport->viewport);

	commandList->RSSetScissorRects(1, &globalObjects->videoDevice->swapChainViewport->scissorRect);

	commandList->OMSetRenderTargets(1, &globalObjects->videoDevice->swapChainRenderTargets[globalObjects->videoDevice->frameIndex]->handle, FALSE, &globalObjects->videoDevice->swapChainDepthBuffers[globalObjects->videoDevice->frameIndex]->handle);



	commandList->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	commandList->SetGraphicsRootSignature(rootSignature->signature.Get());

	commandList->SetPipelineState(pipelineState->pipelineState.Get());



	commandList->SetDescriptorHeaps(1, shaderHeap->heap.GetAddressOf());

	commandList->SetGraphicsRootDescriptorTable(0, shaderHeap->heap->GetGPUDescriptorHandleForHeapStart());


	indexBuffer->Draw();

	vertexBuffer->DrawIndexed(indexBuffer->count);



	texture->barrier.Transition.pResource = texture->texture.Get();
	texture->barrier.Transition.StateBefore = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	texture->barrier.Transition.StateAfter = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST;

	commandList->ResourceBarrier(1, &texture->barrier);



	renderTargets[globalObjects->videoDevice->frameIndex]->barrier.Transition.pResource = renderTargets[globalObjects->videoDevice->frameIndex]->resource.Get();
	renderTargets[globalObjects->videoDevice->frameIndex]->barrier.Transition.StateBefore = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_SOURCE;
	renderTargets[globalObjects->videoDevice->frameIndex]->barrier.Transition.StateAfter = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET;

	commandList->ResourceBarrier(1, &renderTargets[globalObjects->videoDevice->frameIndex]->barrier);


	commandList->Close();
}
