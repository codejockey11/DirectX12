#include "CTexture.h"

CTexture::CTexture()
{
	memset(this, 0x00, sizeof(CTexture));
}

CTexture::CTexture(CErrorLog* el, CVideoDevice* d, const char* fn)
{
	memset(this, 0x00, sizeof(CTexture));

	errorLog = el;

	videoDevice	= d;

	for (UINT i = 0; i < CVideoDevice::FRAME_COUNT; i++)
	{
		commandAllocator[i] = videoDevice->CreateCommandAllocator();
	}

	commandList = videoDevice->CreateCommandList(commandAllocator[0]);

	name = new CString(fn);

	image = new CTgaImage(fn);

	if (image->isInitialized)
	{
		width = image->header->width;
		height = image->header->height;

		CTexture::CreateResource(image->pixels32);

		isInitialized = TRUE;
	}
	else
	{
		errorLog->WriteError("CTexture::CTexture::NotCreated:%s\n", fn);
	}

	delete image;

	image = nullptr;
}

CTexture::~CTexture()
{
	delete name;
}

void CTexture::CreateResource(BYTE* p)
{
	textureDesc.MipLevels = 1;
	textureDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.Flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;
	textureDesc.DepthOrArraySize = 1;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Dimension = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_TEXTURE2D;

	textureHeapProperties.Type = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT;
	textureHeapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	textureHeapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
	textureHeapProperties.CreationNodeMask = 1;
	textureHeapProperties.VisibleNodeMask = 1;

	hr = videoDevice->device->CreateCommittedResource(&textureHeapProperties, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &textureDesc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&texture));

	if (hr < 0)
	{
		errorLog->WriteError("CTexture::CreateResource::CreateCommittedResource::texture:%s\n", errorLog->GetDXGIErrorMessage(hr)->name->GetText());

		return;
	}

	texture->SetName(L"CTexture::Resource");




	// Create the GPU upload buffer.

	uploadBufferSize = 0;

	videoDevice->device->GetCopyableFootprints(&textureDesc, 0, 1, 0, nullptr, nullptr, nullptr, &uploadBufferSize);

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

	hr = videoDevice->device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &heapDesc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&textureUploadHeap));

	if (hr < 0)
	{
		errorLog->WriteError("CTexture::CreateResource::CreateCommittedResource::textureUploadHeap:%s\n", errorLog->GetDXGIErrorMessage(hr)->name->GetText());

		return;
	}

	textureUploadHeap->SetName(L"CTexture::UploadHeap");




	uploadData.pData = p;
	uploadData.RowPitch = (LONG_PTR)width * 4;   // all images stored as 32 bit
	uploadData.SlicePitch = uploadData.RowPitch * height;

	BYTE* pData;

	hr = textureUploadHeap->Map(0, nullptr, (void**)&pData);

	if (hr < 0)
	{
		errorLog->WriteError("CTexture::CreateResource::textureUploadHeap->Map:%s\n", errorLog->GetDXGIErrorMessage(hr)->name->GetText());

		return;
	}

	memcpy(pData, uploadData.pData, uploadData.SlicePitch);

	textureUploadHeap->Unmap(0, nullptr);




	UINT64 RequiredSize = 0;

	UINT64 MemToAlloc = sizeof(D3D12_PLACED_SUBRESOURCE_FOOTPRINT) + sizeof(UINT) + sizeof(UINT64);

	if (MemToAlloc > SIZE_MAX)
	{
		errorLog->WriteError("CTexture::CreateResource::MemToAlloc:%s\n", errorLog->GetDXGIErrorMessage(hr)->name->GetText());

		return;
	}

	D3D12_PLACED_SUBRESOURCE_FOOTPRINT* pMem = (D3D12_PLACED_SUBRESOURCE_FOOTPRINT*)HeapAlloc(GetProcessHeap(), 0, MemToAlloc);

	if (pMem == nullptr)
	{
		errorLog->WriteError("CTexture::CreateResource::HeapAlloc:%s\n", errorLog->GetDXGIErrorMessage(hr)->name->GetText());

		return;
	}


	videoDevice->device->GetCopyableFootprints(&textureDesc, 0, 1, 0, pMem, &height, (UINT64*)&uploadData.RowPitch, &RequiredSize);





	D3D12_TEXTURE_COPY_LOCATION Src;

	Src.pResource = textureUploadHeap.Get();
	Src.Type = D3D12_TEXTURE_COPY_TYPE::D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
	Src.PlacedFootprint = *pMem;

	D3D12_TEXTURE_COPY_LOCATION Dst;

	Dst.pResource = texture.Get();
	Dst.Type = D3D12_TEXTURE_COPY_TYPE::D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
	Dst.SubresourceIndex = 0;

	commandList->CopyTextureRegion(&Dst, 0, 0, 0, &Src, nullptr);



	if (!HeapFree(GetProcessHeap(), 0, pMem))
	{
		errorLog->WriteError("CTexture::CreateResource::HeapFree:FALSE\n");

		return;
	}





	barrier.Transition.pResource = texture.Get();
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

	commandList->ResourceBarrier(1, &barrier);

	commandList->Close();


	ID3D12CommandList* commandLists[] = { commandList.Get() };

	videoDevice->commandQueue->ExecuteCommandLists(1, commandLists);
}

CDynamicTexture::CDynamicTexture(CErrorLog* el, CVideoDevice* d, ComPtr<ID3D12GraphicsCommandList> cl, UINT w, UINT h, const char* n)
{
	memset(this, 0x00, sizeof(CTexture));

	errorLog = el;

	videoDevice = d;

	commandList = cl;

	width = w;

	height = h;

	name = new CString(n);

	CDynamicTexture::CreateResource();
}

CDynamicTexture::~CDynamicTexture()
{
}

void CDynamicTexture::CreateResource()
{
	textureDesc.MipLevels = 1;
	textureDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.Flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;
	textureDesc.DepthOrArraySize = 1;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Dimension = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_TEXTURE2D;

	textureHeapProperties.Type = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT;
	textureHeapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	textureHeapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
	textureHeapProperties.CreationNodeMask = 1;
	textureHeapProperties.VisibleNodeMask = 1;

	hr = videoDevice->device->CreateCommittedResource(&textureHeapProperties, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &textureDesc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&texture));

	if (hr < 0)
	{
		errorLog->WriteError("CTexture::CreateResource::CreateCommittedResource::texture:%s\n", errorLog->GetDXGIErrorMessage(hr)->name->GetText());

		return;
	}

	texture->SetName(L"CTexture::Resource");




	// Create the GPU upload buffer.

	uploadBufferSize = 0;

	videoDevice->device->GetCopyableFootprints(&textureDesc, 0, 1, 0, nullptr, nullptr, nullptr, &uploadBufferSize);

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

	hr = videoDevice->device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &heapDesc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&textureUploadHeap));

	if (hr < 0)
	{
		errorLog->WriteError("CTexture::CreateDynamicResource::CreateCommittedResource::textureUploadHeap:%s\n", errorLog->GetDXGIErrorMessage(hr)->name->GetText());

		return;
	}

	textureUploadHeap->SetName(L"CTexture::UploadHeap");
}

void CDynamicTexture::UpdateTextureResource(void* p)
{
	uploadData.pData = p;
	uploadData.RowPitch = (LONG_PTR)width * 4;   // all images stored as 32 bit
	uploadData.SlicePitch = uploadData.RowPitch * height;

	BYTE* pData;

	hr = textureUploadHeap->Map(0, nullptr, (void**)&pData);

	if (hr < 0)
	{
		errorLog->WriteError("CTexture::UpdateDynamicResource::textureUploadHeap->Map:%s\n", errorLog->GetDXGIErrorMessage(hr)->name->GetText());

		return;
	}

	memcpy(pData, uploadData.pData, uploadData.SlicePitch);

	textureUploadHeap->Unmap(0, nullptr);





	UINT64 RequiredSize = 0;

	UINT64 MemToAlloc = sizeof(D3D12_PLACED_SUBRESOURCE_FOOTPRINT) + sizeof(UINT) + sizeof(UINT64);

	if (MemToAlloc > SIZE_MAX)
	{
		errorLog->WriteError("CTexture::UpdateDynamicResource::MemToAlloc:%s\n", errorLog->GetDXGIErrorMessage(hr)->name->GetText());

		return;
	}

	D3D12_PLACED_SUBRESOURCE_FOOTPRINT* pMem = (D3D12_PLACED_SUBRESOURCE_FOOTPRINT*)HeapAlloc(GetProcessHeap(), 0, MemToAlloc);

	if (pMem == nullptr)
	{
		errorLog->WriteError("CTexture::UpdateDynamicResource::HeapAlloc:%s\n", errorLog->GetDXGIErrorMessage(hr)->name->GetText());

		return;
	}


	videoDevice->device->GetCopyableFootprints(&textureDesc, 0, 1, 0, pMem, &height, (UINT64*)&uploadData.RowPitch, &RequiredSize);





	D3D12_TEXTURE_COPY_LOCATION Src;

	Src.pResource = textureUploadHeap.Get();
	Src.Type = D3D12_TEXTURE_COPY_TYPE::D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
	Src.PlacedFootprint = *pMem;

	D3D12_TEXTURE_COPY_LOCATION Dst;

	Dst.pResource = texture.Get();
	Dst.Type = D3D12_TEXTURE_COPY_TYPE::D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
	Dst.SubresourceIndex = 0;

	commandList->CopyTextureRegion(&Dst, 0, 0, 0, &Src, nullptr);



	if (!HeapFree(GetProcessHeap(), 0, pMem))
	{
		errorLog->WriteError("CTexture::UpdateDynamicResource::HeapFree:FALSE\n");

		return;
	}


	barrier.Transition.pResource = texture.Get();
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;



	commandList->ResourceBarrier(1, &barrier);
}