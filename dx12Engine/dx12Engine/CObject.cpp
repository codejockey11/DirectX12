#include "CObject.h"

CObject::CObject()
{
	memset(this, 0x00, sizeof(CObject));
}

CObject::CObject(CGlobalObjects* g)
{
	memset(this, 0x00, sizeof(CObject));

	globalObjects = g;

	for (UINT i = 0; i < CVideoDevice::FRAME_COUNT; i++)
	{
		commandAllocators[i] = globalObjects->videoDevice->CreateCommandAllocator();
	}

	commandList = globalObjects->videoDevice->CreateCommandList(commandAllocators[0]);

	commandList->SetName(L"CObject::CObject");

	materialCount = 1;

	vertexBuffers[0] = new CLinkList();

	wvp = new CFloat4x4Buffer(globalObjects, commandList);

	shaderHeap[0] = new CShaderHeap(globalObjects, commandList);

	shaderHeap[0]->SlotResource(0, wvp);

	shaderHeap[0]->SlotResource(1, shaderHeap[0]->floats);

	shaderHeap[0]->SlotResource(2, shaderHeap[0]->ints);

	range = new CRange();
}

CObject::CObject(CGlobalObjects* g, COverhead* o, const char* fn, const char* vsn, const char* vep, const char* vv, const char* psn, const char* pep, const char* pv, BOOL AlphaToCoverageEnable, BOOL BlendEnable)
{
	memset(this, 0x00, sizeof(CObject));

	globalObjects = g;

	overhead = o;

	name = new CString(fn);

	for (UINT i = 0; i < CVideoDevice::FRAME_COUNT; i++)
	{
		commandAllocators[i] = globalObjects->videoDevice->CreateCommandAllocator();
	}

	commandList = globalObjects->videoDevice->CreateCommandList(commandAllocators[0]);

	commandList->SetName(L"CObject::CObject");


	wvp = new CFloat4x4Buffer(globalObjects, commandList);

	range = new CRange();


	if (overhead)
	{
		overheadWvp = new CFloat4x4Buffer(globalObjects, commandList);
	}

	
	fopen_s(&file, name->GetText(), "rb");

	fread(&materialCount, sizeof(SHORT), 1, file);

	for (SHORT mc = 0; mc < materialCount; mc++)
	{
		vertexBuffers[mc] = new CLinkList();

		material[mc] = new CMaterial();

		model = new CModel(globalObjects, file, commandList, vertexBuffers[mc], material[mc]);

		delete model;

		shaderHeap[mc] = new CShaderHeap(globalObjects, commandList);

		shaderHeap[mc]->SlotResource(0, wvp);

		shaderHeap[mc]->SlotResource(1, shaderHeap[mc]->floats);

		shaderHeap[mc]->SlotResource(2, shaderHeap[mc]->ints);

		if (material[mc])
		{
			shaderHeap[mc]->floats->values[0] = material[mc]->opacity;

			if (material[mc]->map_Kd)
			{
				shaderHeap[mc]->SlotResource(3, material[mc]->map_Kd);
				shaderHeap[mc]->ints->values[0] = 1;
			}

			if (material[mc]->map_Kb)
			{
				shaderHeap[mc]->SlotResource(4, material[mc]->map_Kb);
				shaderHeap[mc]->ints->values[1] = 1;
			}

			if (material[mc]->map_Ks)
			{
				shaderHeap[mc]->SlotResource(5, material[mc]->map_Ks);
				shaderHeap[mc]->ints->values[2] = 1;
			}

			if (material[mc]->map_Ka)
			{
				shaderHeap[mc]->SlotResource(6, material[mc]->map_Ka);
				shaderHeap[mc]->ints->values[3] = 1;
			}

			if (material[mc]->refl)
			{
				shaderHeap[mc]->SlotResource(7, material[mc]->refl);
				shaderHeap[mc]->ints->values[4] = 1;
			}
		}


		shaderHeap[mc]->floats->values[1] = 0.0F;
		shaderHeap[mc]->floats->values[2] = 1.0F;
		shaderHeap[mc]->floats->values[3] = 0.0F;
		shaderHeap[mc]->floats->values[4] = 1.0F;


		if (overhead)
		{
			overheadHeap[mc] = new CShaderHeap(globalObjects, commandList);

			overheadHeap[mc]->SlotResource(0, overheadWvp);

			overheadHeap[mc]->SlotResource(1, overheadHeap[mc]->floats);

			overheadHeap[mc]->SlotResource(2, overheadHeap[mc]->ints);

			if (material[mc])
			{
				overheadHeap[mc]->floats->values[0] = material[mc]->opacity;

				if (material[mc]->map_Kd)
				{
					overheadHeap[mc]->SlotResource(3, material[mc]->map_Kd);
					overheadHeap[mc]->ints->values[0] = 1;
				}

				if (material[mc]->map_Kb)
				{
					overheadHeap[mc]->SlotResource(4, material[mc]->map_Kb);
					overheadHeap[mc]->ints->values[1] = 1;
				}

				if (material[mc]->map_Ks)
				{
					overheadHeap[mc]->SlotResource(5, material[mc]->map_Ks);
					overheadHeap[mc]->ints->values[2] = 1;
				}

				if (material[mc]->map_Ka)
				{
					overheadHeap[mc]->SlotResource(6, material[mc]->map_Ka);
					overheadHeap[mc]->ints->values[3] = 1;
				}

				if (material[mc]->refl)
				{
					overheadHeap[mc]->SlotResource(7, material[mc]->refl);
					overheadHeap[mc]->ints->values[4] = 1;
				}
			}

			overheadHeap[mc]->floats->values[1] = 0.0F;
			overheadHeap[mc]->floats->values[2] = 1.0F;
			overheadHeap[mc]->floats->values[3] = 0.0F;
			overheadHeap[mc]->floats->values[4] = 1.0F;
			overheadHeap[mc]->floats->values[5] = 1.0F;
		}

	}

	fclose(file);

	vertexShader = globalObjects->shaderMgr->MakeShader(vsn, vep, vv);
	pixelShader = globalObjects->shaderMgr->MakeShader(psn, pep, pv);

	range->SetRange(0, D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 3);
	range->SetRange(1, D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 5);

	rootSignature = new CRootSignature(globalObjects, 2, range);

	pipelineState = new CPipelineState(globalObjects, CVertex::VT_VERTEXNT);
	
	pipelineState->SetDesc(AlphaToCoverageEnable, BlendEnable, TRUE, D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, vertexShader, pixelShader, nullptr, nullptr, nullptr, rootSignature->signature);
}

CObject::~CObject()
{
	delete name;

	delete wvp;

	delete range;

	delete pipelineState;

	delete rootSignature;

	for (UINT mc = 0; mc < CMaterial::MATERIAL_COUNT; mc++)
	{
		delete vertexBuffers[mc];

		delete material[mc];

		delete shaderHeap[mc];

		delete overheadHeap[mc];
	}

	delete overheadWvp;
}

void CObject::Draw()
{
	commandAllocators[globalObjects->videoDevice->frameIndex]->Reset();

	commandList->Reset(commandAllocators[globalObjects->videoDevice->frameIndex].Get(), nullptr);



	commandList->RSSetViewports(1, &globalObjects->videoDevice->swapChainViewport->viewport);

	commandList->RSSetScissorRects(1, &globalObjects->videoDevice->swapChainViewport->scissorRect);

	commandList->OMSetRenderTargets(1, &globalObjects->videoDevice->swapChainRenderTargets[globalObjects->videoDevice->frameIndex]->handle, FALSE, &globalObjects->videoDevice->swapChainDepthBuffers[globalObjects->videoDevice->frameIndex]->handle);



	wvp->constantBuffer->UpdateBuffer((void*)&wvp->values);



	commandList->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	commandList->SetGraphicsRootSignature(rootSignature->signature.Get());

	commandList->SetPipelineState(pipelineState->pipelineState.Get());


	for (SHORT mc = 0; mc < materialCount; mc++)
	{
		CLinkListNode* vbn = vertexBuffers[mc]->list;

		while (vbn->element)
		{
			CVertexBuffer* vb = (CVertexBuffer*)vbn->element;

			shaderHeap[mc]->UpdateResourceBuffers();

			commandList->SetDescriptorHeaps(1, shaderHeap[mc]->heap.GetAddressOf());

			commandList->SetGraphicsRootDescriptorTable(0, shaderHeap[mc]->heap->GetGPUDescriptorHandleForHeapStart());


			if (roll || pitch || yaw)
			{
				vb->UpdateRotation(pitch, yaw, roll);

				vb->Update();
			}


			vb->Draw();

			vbn = vbn->next;
		}
	}


	if (overhead)
	{
		commandList->RSSetViewports(1, &overhead->viewport->viewport);

		commandList->RSSetScissorRects(1, &overhead->viewport->scissorRect);

		commandList->OMSetRenderTargets(1, &overhead->renderTargets[globalObjects->videoDevice->frameIndex]->handle, FALSE, &overhead->depthBuffers[globalObjects->videoDevice->frameIndex]->handle);


		XMStoreFloat4x4(&overheadWvp->values, overhead->camera->wvpMat);

		overheadWvp->constantBuffer->UpdateBuffer((void*)&overheadWvp->values);

		
		for (SHORT mc = 0; mc < materialCount; mc++)
		{
			CLinkListNode* vbn = vertexBuffers[mc]->list;

			while (vbn->element)
			{
				CVertexBuffer* vb = (CVertexBuffer*)vbn->element;

				overheadHeap[mc]->UpdateResourceBuffers();

				commandList->SetDescriptorHeaps(1, overheadHeap[mc]->heap.GetAddressOf());

				commandList->SetGraphicsRootDescriptorTable(0, overheadHeap[mc]->heap->GetGPUDescriptorHandleForHeapStart());

				vb->Draw();

				vbn = vbn->next;
			}
		}

	}


	commandList->Close();
}
