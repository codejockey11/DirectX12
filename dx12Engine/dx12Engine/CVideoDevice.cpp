#include "CVideoDevice.h"

CVideoDevice::CVideoDevice()
{
	memset(this, 0x00, sizeof(CVideoDevice));
}

CVideoDevice::CVideoDevice(CErrorLog* el, CWindow* w)
{
	memset(this, 0x00, sizeof(CVideoDevice));

	errorLog = el;
	
	window = w;

	vsyncEnabled = FALSE;

	swapChainViewport = new CViewport(0.0F, 0.0F, (float)window->width, (float)window->height);


#if defined(_DEBUG)
	hr = D3D12GetDebugInterface(IID_PPV_ARGS(&debugController));

	debugController->EnableDebugLayer();

	factoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif	

	CVideoDevice::DXGIFactory();

	CVideoDevice::GetHardwareAdapter();

	CVideoDevice::CreateDevice();

// following section from github.com/jpvanoosten/LearningDirectX12/blob/master/Tutorial2/src/Application.cpp
#ifdef _DEBUG

	device->QueryInterface(IID_PPV_ARGS(&infoQueue));


	// Suppress whole categories of messages
	//D3D12_MESSAGE_CATEGORY Categories[] = {};
	
	D3D12_MESSAGE_SEVERITY Severities[] =
	{
		D3D12_MESSAGE_SEVERITY_INFO
	};
	
	// Suppress individual messages by their ID
	D3D12_MESSAGE_ID DenyIds[] =
	{
		D3D12_MESSAGE_ID_CLEARRENDERTARGETVIEW_MISMATCHINGCLEARVALUE,   // I'm really not sure how to avoid this message.
	};
	
	D3D12_INFO_QUEUE_FILTER NewFilter = {};
	
	//NewFilter.DenyList.NumCategories = _countof(Categories);
	//NewFilter.DenyList.pCategoryList = Categories;
	
	NewFilter.DenyList.NumSeverities = _countof(Severities);
	NewFilter.DenyList.pSeverityList = Severities;
	
	NewFilter.DenyList.NumIDs = _countof(DenyIds);
	NewFilter.DenyList.pIDList = DenyIds;
	
	infoQueue->PushStorageFilter(&NewFilter);
#endif

	CVideoDevice::DeviceInformation();

	CVideoDevice::CreateCommandQueue();

	CVideoDevice::CreateSwapChain();

	CVideoDevice::Create11on12Device();

	CVideoDevice::CreateRenderTargets();

	CVideoDevice::CreateDepthBuffers();


	for (UINT i = 0; i < CVideoDevice::FRAME_COUNT; i++)
	{
		commandAllocators[i] = CVideoDevice::CreateCommandAllocator();
	}

	commandList = CVideoDevice::CreateCommandList(commandAllocators[0]);

	commandList->SetName(L"CVideoDevice::CommandList");

	commandList->Close();
}

CVideoDevice::~CVideoDevice()
{
	if (swapChain)
	{
		swapChain->SetFullscreenState(FALSE, nullptr);
	}

	CloseHandle(fenceEvent);
	
	fenceEvent = 0;

	delete d3d11On12;

	delete swapChainViewport;

	for (UINT i = 0; i < CVideoDevice::FRAME_COUNT;i++)
	{
		delete swapChainRenderTargets[i];

		delete swapChainDepthBuffers[i];
	}
}

void CVideoDevice::DXGIFactory()
{
	hr = CreateDXGIFactory2(factoryFlags, IID_IDXGIFactory4, (void**)&factory);

	if (hr < 0)
	{
		errorLog->WriteError("CVideoDevice::DXGIFactory::CreateDXGIFactory2:%s\n", errorLog->GetDXGIErrorMessage(hr)->name->GetText());
	}

	hr = factory->CheckFeatureSupport(DXGI_FEATURE::DXGI_FEATURE_PRESENT_ALLOW_TEARING, &tearingSupport, sizeof(BOOL));

	if (hr < 0)
	{
		errorLog->WriteError("CVideoDevice::DXGIFactory::CheckFeatureSupport:%s\n", errorLog->GetDXGIErrorMessage(hr)->name->GetText());
	}

	if (tearingSupport)
	{
		errorLog->WriteError("CVideoDevice::DXGIFactory::DXGI_FEATURE::DXGI_FEATURE_PRESENT_ALLOW_TEARING\n");
	}
}

void CVideoDevice::GetHardwareAdapter()
{
	for (UINT ai = 0; ; ai++)
	{
		if (DXGI_ERROR_NOT_FOUND == factory->EnumAdapters1(ai, &adapter))
		{
			break;
		}

		hr = D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), 0);

		if (hr == S_FALSE)
		{
			errorLog->WriteError("CVideoDevice::GetHardwareAdapter D3D_FEATURE_LEVEL_11_0 supported\n");

			return;
		}
	}

	if (adapter == 0)
	{
		errorLog->WriteError("CVideoDevice::GetHardwareAdapter:No DirectX 12 Device Adapter Found\n");
	}
}

void CVideoDevice::CreateDevice()
{
	hr = D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), (void**)&device);

	if (hr < 0)
	{
		errorLog->WriteError("CVideoDevice::CreateDevice::D3D12CreateDevice:%s\n", errorLog->GetDXGIErrorMessage(hr)->name->GetText());
	}
}

void CVideoDevice::DeviceInformation()
{
	hr = adapter->EnumOutputs(0, &adapterOutput);

	if (hr < 0)
	{
		errorLog->WriteError("CVideoDevice::DeviceInformation::EnumOutputs:%s\n", errorLog->GetDXGIErrorMessage(hr)->name->GetText());
	}

	hr = adapterOutput->GetDisplayModeList(DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, nullptr);

	if (hr < 0)
	{
		errorLog->WriteError("CVideoDevice::DeviceInformation::GetDisplayModeList for numModes:%s\n", errorLog->GetDXGIErrorMessage(hr)->name->GetText());
	}

	hr = adapterOutput->GetDisplayModeList(DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);

	if (hr < 0)
	{
		errorLog->WriteError("CVideoDevice::DeviceInformation::GetDisplayModeList:%s\n", errorLog->GetDXGIErrorMessage(hr)->name->GetText());
	}

	errorLog->WriteError("CVideoDevice::DeviceInformation::Display Modes for DXGI_FORMAT_R8G8B8A8_UNORM\n");

	UINT prevWidth = 0;
	UINT prevHeight = 0;

	for (UINT i = 0; i < numModes; i++)
	{
		if ((displayModeList[i].Width != prevWidth) && (displayModeList[i].Height != prevHeight))
		{
			errorLog->WriteError("CVideoDevice::DeviceInformation::w:%i h:%i h:%i\n", displayModeList[i].Width, displayModeList[i].Height, displayModeList[i].RefreshRate.Numerator / displayModeList[i].RefreshRate.Denominator);

			prevWidth = displayModeList[i].Width;
			prevHeight = displayModeList[i].Height;
		}
	}

	hr = adapter->GetDesc(&adapterDesc);

	if (hr < 0)
	{
		errorLog->WriteError("CVideoDevice::DeviceInformation::GetDesc:%s\n", errorLog->GetDXGIErrorMessage(hr)->name->GetText());
	}

	videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	errorLog->WriteError("CVideoDevice::DeviceInformation::Name:%s\n", WideToMBS(adapterDesc.Description));
	errorLog->WriteError("CVideoDevice::DeviceInformation::Memory:%dM\n", videoCardMemory);
}

void CVideoDevice::CreateCommandQueue()
{
	commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT;
	commandQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY::D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAGS::D3D12_COMMAND_QUEUE_FLAG_NONE;
	commandQueueDesc.NodeMask = 0;

	hr = device->CreateCommandQueue(&commandQueueDesc, __uuidof(ID3D12CommandQueue), (void**)&commandQueue);

	if (hr < 0)
	{
		errorLog->WriteError("CVideoDevice::CommandQueue::CreateCommandQueue:%s\n", errorLog->GetDXGIErrorMessage(hr)->name->GetText());
	}
}

void CVideoDevice::CreateSwapChain()
{
	swapChainDesc.BufferCount = CVideoDevice::FRAME_COUNT;

	swapChainDesc.BufferDesc.Height = window->height;
	swapChainDesc.BufferDesc.Width = window->width;

	swapChainDesc.BufferDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;

	swapChainDesc.OutputWindow = window->hWnd;

	swapChainDesc.Windowed = window->windowed;

	if (vsyncEnabled)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 59;// numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;// denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING::DXGI_MODE_SCALING_UNSPECIFIED;

	swapChainDesc.Flags = 0;


	hr = factory->CreateSwapChain(commandQueue.Get(), &swapChainDesc, (IDXGISwapChain**)swapChain.GetAddressOf());

	if (hr < 0)
	{
		errorLog->WriteError("CVideoDevice::CreateSwapChain::CreateSwapChain:%s\n", errorLog->GetDXGIErrorMessage(hr)->name->GetText());
	}


	if (tearingSupport)
	{
		factory->MakeWindowAssociation(window->hWnd, DXGI_MWA_NO_ALT_ENTER);
	}

	frameIndex = swapChain->GetCurrentBackBufferIndex();
}

void CVideoDevice::Create11on12Device()
{
	d3d11On12 = new CD3D11On12(errorLog, device, commandQueue);
}

void CVideoDevice::CreateRenderTargets()
{
	float dpi = window->GetDpi();

	for (UINT i = 0; i < CVideoDevice::FRAME_COUNT; i++)
	{
		swapChainRenderTargets[i] = new CRenderTarget(errorLog, device);

		swapChainRenderTargets[i]->SwapChainTarget(swapChain, i);

		swapChainRenderTargets[i]->WrapResource(dpi, d3d11On12->d3d11On12Device, d3d11On12->d2dDeviceContext);
	}
}

void CVideoDevice::CreateDepthBuffers()
{
	for (UINT i = 0; i < CVideoDevice::FRAME_COUNT; i++)
	{
		swapChainDepthBuffers[i] = new CDepthBuffer(errorLog, device, window->width, window->height);
	}
}

ComPtr<ID3D12CommandAllocator> CVideoDevice::CreateCommandAllocator()
{
	ComPtr<ID3D12CommandAllocator> ca;

	hr = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&ca));

	if (hr < 0)
	{
		errorLog->WriteError("CVideoDevice::CommandProcessor::CreateCommandAllocator:%s\n", errorLog->GetDXGIErrorMessage(hr)->name->GetText());

		return nullptr;
	}
	
	ca->SetName(L"CVideoDevice::CommandAllocator");

	return ca;
}

ComPtr<ID3D12GraphicsCommandList> CVideoDevice::CreateCommandList(ComPtr<ID3D12CommandAllocator> ca)
{
	ComPtr<ID3D12GraphicsCommandList> cl;

	hr = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT, ca.Get(), nullptr, IID_PPV_ARGS(&cl));

	if (hr < 0)
	{
		errorLog->WriteError("CVideoDevice::CreateCommandList::CreateCommandList:%s\n", errorLog->GetDXGIErrorMessage(hr)->name->GetText());

		return nullptr;
	}

	return cl;
}

void CVideoDevice::CreateFence()
{
	hr = device->CreateFence(fenceValues[frameIndex], D3D12_FENCE_FLAGS::D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));

	if (hr < 0)
	{
		errorLog->WriteError("CVideoDevice::CreateFence::CreateFence:%s\n", errorLog->GetDXGIErrorMessage(hr)->name->GetText());

		return;
	}

	fence->SetName(L"CVideoDevice::Fence");

	fenceValues[frameIndex]++;

	fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
}

void CVideoDevice::WaitForGpu()
{
	if (!fence)
	{
		return;
	}

	commandQueue->Signal(fence.Get(), fenceValues[frameIndex]);

	fence->SetEventOnCompletion(fenceValues[frameIndex], fenceEvent);

	WaitForSingleObjectEx(fenceEvent, INFINITE, FALSE);

	fenceValues[frameIndex]++;
}

void CVideoDevice::MoveToNextFrame()
{
	if (vsyncEnabled)
	{
		swapChain->Present(1, 0);
	}
	else
	{
		swapChain->Present(0, 0);
	}


	currentFenceValue = fenceValues[frameIndex];
	
	hr = commandQueue->Signal(fence.Get(), currentFenceValue);

	if (hr < 0)
	{
		errorLog->WriteError("MoveToNextFrame::commandQueue->Signal:%s\n", errorLog->GetDXGIErrorMessage(hr)->name->GetText());
	}


	frameIndex = swapChain->GetCurrentBackBufferIndex();

	if (fence->GetCompletedValue() < fenceValues[frameIndex])
	{
		hr = fence->SetEventOnCompletion(fenceValues[frameIndex], fenceEvent);

		if (hr < 0)
		{
			errorLog->WriteError("MoveToNextFrame::fence->SetEventOnCompletion:%s\n", errorLog->GetDXGIErrorMessage(hr)->name->GetText());
		}

		WaitForSingleObjectEx(fenceEvent, INFINITE, FALSE);
	}

	fenceValues[frameIndex] = currentFenceValue + 1;
}

void CVideoDevice::Init2DDraw()
{
	d3d11On12->d3d11On12Device->AcquireWrappedResources(swapChainRenderTargets[frameIndex]->wrappedBackBuffer.GetAddressOf(), 1);

	d3d11On12->d2dDeviceContext->SetTarget(swapChainRenderTargets[frameIndex]->d2dRenderTarget.Get());
	
	d3d11On12->d2dDeviceContext->BeginDraw();
	
	d3d11On12->d2dDeviceContext->SetTransform(D2D1::Matrix3x2F::Identity());
}

void CVideoDevice::End2DDraw()
{
	d3d11On12->d2dDeviceContext->EndDraw();

	d3d11On12->d3d11On12Device->ReleaseWrappedResources(swapChainRenderTargets[frameIndex]->wrappedBackBuffer.GetAddressOf(), 1);

	d3d11On12->d3d11DeviceContext->Flush();
}

void CVideoDevice::ToggleFullscreenWindow()
{
	if (window->windowed)
	{
		windowRect = window->GetWindowRectangle();

		SetWindowLong(window->hWnd, GWL_STYLE, WS_POPUP);

		if (swapChain)
		{
			IDXGIOutput* pOutput;
			
			swapChain->GetContainingOutput(&pOutput);

			DXGI_OUTPUT_DESC Desc;
			
			pOutput->GetDesc(&Desc);
			
			fullscreenWindowRect = Desc.DesktopCoordinates;

			pOutput->Release();
			pOutput = 0;
		}

		SetWindowPos(window->hWnd, HWND_TOPMOST, fullscreenWindowRect.left, fullscreenWindowRect.top, fullscreenWindowRect.right, fullscreenWindowRect.bottom, SWP_FRAMECHANGED | SWP_NOACTIVATE);

		ShowWindow(window->hWnd, SW_MAXIMIZE);
	}
	else
	{
		SetWindowLong(window->hWnd, GWL_STYLE, WS_TILEDWINDOW);

		SetWindowPos(window->hWnd, HWND_NOTOPMOST, windowRect.left, windowRect.top, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, SWP_FRAMECHANGED | SWP_NOACTIVATE);

		ShowWindow(window->hWnd, SW_NORMAL);
	}

	window->windowed = !window->windowed;
}
