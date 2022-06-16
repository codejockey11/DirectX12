#include "CVertexBuffer.h"

CVertexBuffer::CVertexBuffer()
{
	memset(this, 0x00, sizeof(CVertexBuffer));
}

CVertexBuffer::CVertexBuffer(CGlobalObjects* g, ComPtr<ID3D12GraphicsCommandList> cl, BYTE t, UINT c, void* vertices)
{
	memset(this, 0x00, sizeof(CVertexBuffer));

	globalObjects = g;

	type = t;
	
	count = c;

	commandList = cl;

	if (CVertexBuffer::CreateResource())
	{
		CVertexBuffer::LoadBuffer(vertices);
	}
}

CVertexBuffer::~CVertexBuffer()
{
}

BYTE CVertexBuffer::CreateResource()
{
	// We do not intend to read from this resource on the CPU.
	range.Begin	= 0;
	range.End = 0;

	heapProperties.Type = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD;
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProperties.MemoryPoolPreference	= D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
	heapProperties.CreationNodeMask = 1;
	heapProperties.VisibleNodeMask = 1;

	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Alignment = 0;
	resourceDesc.Width = 0;
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.SampleDesc.Quality = 0;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resourceDesc.Flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;

	switch (type)
	{
		case CVertex::VT_VERTEXRGBA:
		{
			resourceDesc.Width = count * sizeof(CVertexRGBA);

			hr = globalObjects->videoDevice->device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertexBuffer));

			if (hr < 0)
			{
				globalObjects->errorLog->WriteError("CVertexBuffer::CVertexBuffer::CreateCommittedResource::CVertex::VT_VERTEXRGBA:%s\n", globalObjects->errorLog->GetDXGIErrorMessage(hr)->name->GetText());

				return FALSE;
			}

			vertexBuffer->SetName(L"CVertex::VT_VERTEXRGBA");

			vertexBufferView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
			vertexBufferView.StrideInBytes = sizeof(CVertexRGBA);
			vertexBufferView.SizeInBytes = count * sizeof(CVertexRGBA);

			break;
		}

		case CVertex::VT_VERTEXT:
		{
			resourceDesc.Width = count * sizeof(CVertexT);

			hr = globalObjects->videoDevice->device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertexBuffer));

			if (hr < 0)
			{
				globalObjects->errorLog->WriteError("CVertexBuffer::CVertexBuffer::CreateCommittedResource::CVertex::VT_VERTEXT:%s\n", globalObjects->errorLog->GetDXGIErrorMessage(hr)->name->GetText());

				return FALSE;
			}

			vertexBuffer->SetName(L"CVertex::VT_VERTEXT");

			vertexBufferView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
			vertexBufferView.StrideInBytes = sizeof(CVertexT);
			vertexBufferView.SizeInBytes = count * sizeof(CVertexT);

			break;
		}

		case CVertex::VT_VERTEXNT:
		{
			resourceDesc.Width = count * sizeof(CVertexNT);

			hr = globalObjects->videoDevice->device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertexBuffer));

			if (hr < 0)
			{
				globalObjects->errorLog->WriteError("CVertexBuffer::CVertexBuffer::CreateCommittedResource::CVertex::VT_VERTEXNT:%s\n", globalObjects->errorLog->GetDXGIErrorMessage(hr)->name->GetText());

				return FALSE;
			}

			vertexBuffer->SetName(L"CVertex::VT_VERTEXNT");

			vertexBufferView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
			vertexBufferView.StrideInBytes = sizeof(CVertexNT);
			vertexBufferView.SizeInBytes = count * sizeof(CVertexNT);

			break;
		}

		case CVertex::VT_VERTEX2D:
		{
			resourceDesc.Width = count * sizeof(CVertex2d);

			hr = globalObjects->videoDevice->device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertexBuffer));

			if (hr < 0)
			{
				globalObjects->errorLog->WriteError("CVertexBuffer::CVertexBuffer::CreateCommittedResource::CVertex::VT_VERTEX2D:%s\n", globalObjects->errorLog->GetDXGIErrorMessage(hr)->name->GetText());

				return FALSE;
			}

			vertexBuffer->SetName(L"CVertex::VT_VERTEX2D");

			vertexBufferView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
			vertexBufferView.StrideInBytes = sizeof(CVertex2d);
			vertexBufferView.SizeInBytes = count * sizeof(CVertex2d);

			break;
		}

		default:
		{
			globalObjects->errorLog->WriteError("CVertexBuffer::LoadBuffer::Unknown CVertex::<type>\n");

			return FALSE;

			break;
		}
	}

	return TRUE;
}

BYTE CVertexBuffer::LoadBuffer(void* vertices)
{
	switch (type)
	{
		case CVertex::VT_VERTEXRGBA:
		{
			vertexBuffer->Map(0, nullptr, &pData);
			memcpy(pData, vertices, count * sizeof(CVertexRGBA));
			vertexBuffer->Unmap(0, nullptr);

			break;
		}

		case CVertex::VT_VERTEXT:
		{
			vertexBuffer->Map(0, nullptr, &pData);
			memcpy(pData, vertices, count * sizeof(CVertexT));
			vertexBuffer->Unmap(0, nullptr);

			break;
		}

		case CVertex::VT_VERTEXNT:
		{
			vertexBuffer->Map(0, nullptr, &pData);
			memcpy(pData, vertices, count * sizeof(CVertexNT));
			vertexBuffer->Unmap(0, nullptr);

			break;
		}

		case CVertex::VT_VERTEX2D:
		{
			vertexBuffer->Map(0, nullptr, &pData);
			memcpy(pData, vertices, count * sizeof(CVertex2d));
			vertexBuffer->Unmap(0, nullptr);

			break;
		}

		default:
		{
			globalObjects->errorLog->WriteError("CVertexBuffer::LoadBuffer::Unknown CVertex::<type>\n");

			return FALSE;

			break;
		}
	}

	return TRUE;
}

void CVertexBuffer::Draw()
{
	commandList->IASetVertexBuffers(0, 1, &vertexBufferView);

	commandList->DrawInstanced(count, 1, 0, 0);
}

void CVertexBuffer::DrawIndexed(UINT c)
{
	commandList->IASetVertexBuffers(0, 1, &vertexBufferView);

	commandList->DrawIndexedInstanced(c, 1, 0, 0, 0);
}

void CVertexBuffer::UpdateRotation(float x, float y, float z)
{
	rotation.x = x;

	rotation.y = y;

	rotation.z = z;
}

void CVertexBuffer::Update()
{
	yaw = rotation.y * PiDiv180;
	pitch = rotation.x * PiDiv180;
	roll = rotation.z * PiDiv180;

	XMMATRIX xmMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	switch (type)
	{
	case CVertex::VT_VERTEXRGBA:
	{
		CVertexRGBA* pData;

		vertexBuffer->Map(0, nullptr, (void**)&pData);

		for (UINT i = 0; i < count; i++)
		{
			FXMVECTOR vp = XMLoadFloat3(&pData->p);

			XMVECTOR v = XMVector3TransformCoord(vp, xmMatrix);

			pData->p = XMFLOAT3(XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v));

			pData++;
		}

		vertexBuffer->Unmap(0, nullptr);

		break;
	}
	case CVertex::VT_VERTEXT:
	{
		CVertexT* pData;

		vertexBuffer->Map(0, nullptr, (void**)&pData);

		for (UINT i = 0; i < count; i++)
		{
			FXMVECTOR vp = XMLoadFloat3(&pData->p);

			XMVECTOR v = XMVector3TransformCoord(vp, xmMatrix);

			pData->p = XMFLOAT3(XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v));

			pData++;
		}

		vertexBuffer->Unmap(0, nullptr);

		break;
	}
	case CVertex::VT_VERTEXNT:
	{
		CVertexNT* pData;

		hr = vertexBuffer->Map(0, nullptr, (void**)&pData);

		if (hr < 0)
		{
			return;
		}

		for (UINT i = 0; i < count; i++)
		{
			FXMVECTOR vp = XMLoadFloat3(&pData->p);

			XMVECTOR v = XMVector3TransformCoord(vp, xmMatrix);

			pData->p = XMFLOAT3(XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v));

			FXMVECTOR vn = XMLoadFloat3(&pData->n);

			v = XMVector3TransformCoord(vn, xmMatrix);

			pData->n = XMFLOAT3(XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v));

			pData++;
		}

		vertexBuffer->Unmap(0, nullptr);

		break;
	}
	}
}