#ifndef _CVideoDevice
#define _CVideoDevice

#include "standard.h"

#include "CColor.h"
#include "CDepthBuffer.h"
#include "CD3D11On12.h"
#include "CErrorLog.h"
#include "CRenderTarget.h"
#include "CViewport.h"
#include "CWindow.h"

class CVideoDevice
{
public:

	enum
	{
		FRAME_COUNT = 3,

		DISPLAY_MODE_COUNT = 256
	};

	BOOL tearingSupport;

	CD3D11On12* d3d11On12;

	CDepthBuffer* swapChainDepthBuffers[CVideoDevice::FRAME_COUNT];

	CRenderTarget* swapChainRenderTargets[CVideoDevice::FRAME_COUNT];

	CViewport* swapChainViewport;

	ComPtr<ID3D12CommandAllocator> CreateCommandAllocator();

	ComPtr<ID3D12CommandAllocator> commandAllocators[CVideoDevice::FRAME_COUNT];

	ComPtr<ID3D12CommandQueue> commandQueue;

	ComPtr<ID3D12Device> device;

	ComPtr<ID3D12GraphicsCommandList> CreateCommandList(ComPtr<ID3D12CommandAllocator> ca);

	ComPtr<ID3D12GraphicsCommandList> commandList;

	ComPtr<ID3D12Resource> overheadResource[CVideoDevice::FRAME_COUNT];

	ComPtr<IDXGISwapChain3> swapChain;

	UINT frameIndex;

	CVideoDevice();

	CVideoDevice(CErrorLog* el, CWindow* w);

	~CVideoDevice();
	
	void MoveToNextFrame();
	void CreateFence();
	void WaitForGpu();
	void Init2DDraw();
	void End2DDraw();
	void ToggleFullscreenWindow();

private:

	HRESULT hr;

	CErrorLog* errorLog;
	
	CWindow* window;

#ifdef _DEBUG
	ComPtr<ID3D12Debug> debugController;
#endif

	bool vsyncEnabled;

	D3D12_COMMAND_QUEUE_DESC commandQueueDesc;

	DXGI_ADAPTER_DESC adapterDesc;

	DXGI_MODE_DESC displayModeList[CVideoDevice::DISPLAY_MODE_COUNT];

	DXGI_SWAP_CHAIN_DESC swapChainDesc;

	HANDLE fenceEvent;

	ComPtr<ID3D12Fence> fence;

	ComPtr<ID3DBlob> signature;

	ComPtr<IDXGIAdapter1> adapter;

	ComPtr<IDXGIFactory6> factory;

	ComPtr<IDXGIOutput> adapterOutput;

	int videoCardMemory;

	RECT fullscreenWindowRect;

	RECT windowRect;

	UINT factoryFlags;

	UINT numModes;

	UINT64 currentFenceValue;

	UINT64 fenceValues[CVideoDevice::FRAME_COUNT];

	ComPtr< ID3D12InfoQueue> infoQueue;

	void DXGIFactory();
	void GetHardwareAdapter();
	void CreateDevice();
	void CreateCommandQueue();
	void DeviceInformation();
	void CreateSwapChain();
	void Create11on12Device();
	void CreateRenderTargets();
	void CreateDepthBuffers();
};
#endif