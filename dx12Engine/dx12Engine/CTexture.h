#ifndef _CTexture
#define _CTexture

#include "standard.h"

#include "CErrorLog.h"
#include "CString.h"
#include "CTgaImage.h"
#include "CVideoDevice.h"

class CTexture
{
public:

	UINT64 width;

	UINT height;

	CTgaImage* image;

	BYTE isInitialized;

	ComPtr<ID3D12Resource> texture;

	CString* name;

	ComPtr<ID3D12CommandAllocator> commandAllocator[CVideoDevice::FRAME_COUNT];

	ComPtr<ID3D12GraphicsCommandList> commandList;

	CTexture();

	CTexture(CErrorLog* el, CVideoDevice* d, const char* fn);

	~CTexture();

	void CreateResource(BYTE* p);

private:

	HRESULT hr;

	CErrorLog* errorLog;

	CVideoDevice* videoDevice;

	D3D12_RESOURCE_DESC textureDesc;
	
	D3D12_HEAP_PROPERTIES textureHeapProperties;

	D3D12_RESOURCE_DESC heapDesc;

	D3D12_HEAP_PROPERTIES heapProperties;

	ComPtr<ID3D12Resource> textureUploadHeap;

	UINT64 uploadBufferSize;

	D3D12_RESOURCE_ALLOCATION_INFO allocInfo;

	D3D12_SUBRESOURCE_DATA uploadData;

	D3D12_RESOURCE_BARRIER barrier;
};

class CDynamicTexture : public CTexture
{
public:

	CDynamicTexture(CErrorLog* el, CVideoDevice* d, ComPtr<ID3D12GraphicsCommandList> cl, UINT w, UINT h, const char* n);

	~CDynamicTexture();

	void CreateResource();

	void UpdateTextureResource(void* p);


	D3D12_RESOURCE_BARRIER barrier;


private:

	HRESULT hr;

	CErrorLog* errorLog;

	CVideoDevice* videoDevice;

	D3D12_RESOURCE_DESC textureDesc;

	D3D12_HEAP_PROPERTIES textureHeapProperties;

	D3D12_RESOURCE_DESC heapDesc;

	D3D12_HEAP_PROPERTIES heapProperties;

	ComPtr<ID3D12Resource> textureUploadHeap;

	UINT64 uploadBufferSize;

	D3D12_RESOURCE_ALLOCATION_INFO allocInfo;

	D3D12_SUBRESOURCE_DATA uploadData;
};
#endif