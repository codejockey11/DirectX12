#include "standard.h"
#include "Resource.h"

#include "CCamera.h"
#include "CFloatBuffer.h"
#include "CGlobalObjects.h"
#include "CEventManager.h"
#include "CLinkList.h"
#include "CModel.h"
#include "CObject.h"
#include "COverhead.h"
#include "CPipelineState.h"
#include "CRange.h"
#include "CRootSignature.h"
#include "CShader.h"
#include "CShaderHeap.h"
#include "CShaderResourceView.h"
#include "CSprite.h"
#include "CTexture.h"
#include "CVertexBuffer.h"
#include "CWavefront.h"

CGlobalObjects* globalObjects;
CEventManager* eventManager;

CShader* vertexUVShader;
CShader* pixelUVShader;
CShader* vertexModelShader;
CShader* vertexColorShader;
CShader* pixelColorShader;
CShader* pixelModelShader;
CShader* vertexDepthShader;
CShader* pixelDepthShader;

CTexture* texture1;
CTexture* texture2;
CTexture* texture3;
CTexture* texture4;
CTexture* texture5;
CTexture* texture6;
CTexture* texture7;
CTexture* texture8;
CTexture* texture9;

CObject* vertex;
CObject* pixel;
CObject* modelObject;

CSprite* sprite;

COverhead* overhead;

float count = 0.0F;
float deg = 0.0F;

CCamera* camera;

WNDCLASSEX wcex;
HINSTANCE hInst;
MSG msg;

HRESULT hr;

CVertexT uvVertices[6];

CVertexRGBA colorVertices[6];

CD12Font* d12Font;

void InitializeObjects();
void LoadAssets();
void CleanupObjects();
void RunFrame();
void DrawFrame();
void RenderUI();

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		// Regular first person shooter mouse move
		/*
		case WM_MOUSEMOVE:
		{
			globalObjects->mouse->MouseMove();

			camera->UpdateRotation((globalObjects->mouse->lY * globalObjects->mouse->sensitivity), (globalObjects->mouse->lX * globalObjects->mouse->sensitivity), (globalObjects->mouse->lZ * globalObjects->mouse->sensitivity));

			globalObjects->mouse->FirstPersonReset();

			return 0;
		}
		*/
		// MMO style mouse move
		case WM_RBUTTONDOWN:
		{
			globalObjects->mouse->SaveLastPosition();

			globalObjects->mouse->FirstPersonReset();

			globalObjects->mouse->MouseClick(message, wParam);

			return 0;
		}
		case WM_RBUTTONUP:
		{
			globalObjects->mouse->SetLastPosition();
		
			globalObjects->mouse->MouseClick(message, wParam);

			return 0;
		}
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		{
			globalObjects->mouse->MouseClick(message, wParam);

			return 0;
		}
		case WM_MOUSEWHEEL:
		{
			globalObjects->mouse->MouseWheel(wParam);

			camera->UpdateRotation((globalObjects->mouse->lY * globalObjects->mouse->sensitivity), (globalObjects->mouse->lX * globalObjects->mouse->sensitivity), (globalObjects->mouse->lZ * globalObjects->mouse->sensitivity));

			return 0;
		}
		case WM_KEYDOWN:
		{
			if (eventManager->consoleVisible)
			{
				if ((wParam == VK_PRIOR) || (wParam == VK_NEXT))
				{
					CPacket* packet = new CPacket(0, 0, CPacket::PT_LOCAL, CPacket::PT_CONSOLEPAGING, (BYTE)wParam, sizeof(char));

					eventManager->SendEvent(packet);

					delete packet;

					return 0;
				}
			}

			globalObjects->keyboard->KeyDown(message, wParam);
		
			return 0;
		}
		case WM_KEYUP:
		{
			globalObjects->keyboard->KeyDown(message, wParam);

			return 0;
		}
		case WM_CHAR:
		{
			if (eventManager->consoleVisible)
			{
				CPacket* packet = new CPacket(0, 0, CPacket::PT_LOCAL, CPacket::PT_CONSOLEINPUT, (BYTE)wParam, sizeof(char));
			
				eventManager->SendEvent(packet);
			
				delete packet;
			}
		
			return 0;
		}
		case WM_DESTROY:
		{
			PostQuitMessage(0);
		
			return 0;
		}
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	memset(&wcex, 0x00, sizeof(WNDCLASSEX));
	
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DX12ENGINE));
	
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	//wcex.hCursor = LoadCursorFromFileA("dx12Engine.ico");
	
	//wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW + 1);
	//wcex.lpszMenuName		= MAKEINTRESOURCE(IDC_DX12ENGINE);
	wcex.lpszClassName = "dx12EngineClass";
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	RegisterClassEx(&wcex);

	globalObjects = new CGlobalObjects(hInstance, nCmdShow, "dx12EngineClass", "dx12Engine", "log.txt", "config.txt");

	if (!globalObjects->window->hWnd)
	{
		delete globalObjects;

		UnregisterClass("dx12EngineClass", hInstance);
		
		return 0;
	}

	InitializeObjects();

	LoadAssets();

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			
			DispatchMessage(&msg);
		}
		else
		{
			RunFrame();
		}
	}

	CleanupObjects();

	UnregisterClass("dx12EngineClass", hInstance);

	return (int)msg.wParam;
}

void InitializeObjects()
{
	eventManager = new CEventManager(globalObjects);

	camera = new CCamera((float)globalObjects->window->width, (float)globalObjects->window->height);
}

void CleanupObjects()
{
	globalObjects->videoDevice->WaitForGpu();

	delete vertex;
	delete pixel;
	delete modelObject;
	delete sprite;
	delete overhead;
	
	delete camera;
	delete eventManager;
	delete globalObjects;
}

void LoadAssets()
{
	colorVertices[0].p.x = -0.25F + 0.50F;
	colorVertices[0].p.y = -0.75F;
	colorVertices[0].p.z = 0.25F;
	colorVertices[0].c.x = WatermelonPink.RGBA[0];
	colorVertices[0].c.y = WatermelonPink.RGBA[1];
	colorVertices[0].c.z = WatermelonPink.RGBA[2];
	colorVertices[0].c.w = WatermelonPink.RGBA[3];

	colorVertices[1].p.x = -0.25F + 0.50F;
	colorVertices[1].p.y = 0.50F;
	colorVertices[1].p.z = 0.25F;
	colorVertices[1].c.x = ZombieGreen.RGBA[0];
	colorVertices[1].c.y = ZombieGreen.RGBA[1];
	colorVertices[1].c.z = ZombieGreen.RGBA[2];
	colorVertices[1].c.w = ZombieGreen.RGBA[3];

	colorVertices[2].p.x = 0.25F + 0.50F;
	colorVertices[2].p.y = -0.75F;
	colorVertices[2].p.z = 0.0F;
	colorVertices[2].c.x = Sandstone.RGBA[0];
	colorVertices[2].c.y = Sandstone.RGBA[1];
	colorVertices[2].c.z = Sandstone.RGBA[2];
	colorVertices[2].c.w = Sandstone.RGBA[3];

	colorVertices[3].p.x = 0.25F + 0.50F;
	colorVertices[3].p.y = -0.75F;
	colorVertices[3].p.z = 0.0F;
	colorVertices[3].c.x = LovelyPurple.RGBA[0];
	colorVertices[3].c.y = LovelyPurple.RGBA[1];
	colorVertices[3].c.z = LovelyPurple.RGBA[2];
	colorVertices[3].c.w = LovelyPurple.RGBA[3];

	colorVertices[4].p.x = -0.25F + 0.50F;
	colorVertices[4].p.y = 0.50F;
	colorVertices[4].p.z = 0.25F;
	colorVertices[4].c.x = MarbleBlue.RGBA[0];
	colorVertices[4].c.y = MarbleBlue.RGBA[1];
	colorVertices[4].c.z = MarbleBlue.RGBA[2];
	colorVertices[4].c.w = MarbleBlue.RGBA[3];

	colorVertices[5].p.x = 0.25F + 0.50F;
	colorVertices[5].p.y = 0.50F;
	colorVertices[5].p.z = 0.0F;
	colorVertices[5].c.x = Mahogany.RGBA[0];
	colorVertices[5].c.y = Mahogany.RGBA[1];
	colorVertices[5].c.z = Mahogany.RGBA[2];
	colorVertices[5].c.w = Mahogany.RGBA[3];


	uvVertices[0].p.x = -0.25F;
	uvVertices[0].p.y = -0.75F;
	uvVertices[0].p.z = 0.25F;
	uvVertices[0].uv.x = 0.0F;
	uvVertices[0].uv.y = 1.0F;

	uvVertices[1].p.x = -0.25F;
	uvVertices[1].p.y = 0.50F;
	uvVertices[1].p.z = 0.25F;
	uvVertices[1].uv.x = 0.0F;
	uvVertices[1].uv.y = 0.0F;

	uvVertices[2].p.x = 0.25F;
	uvVertices[2].p.y = -0.75F;
	uvVertices[2].p.z = 0.25F;
	uvVertices[2].uv.x = 1.0F;
	uvVertices[2].uv.y = 1.0F;

	uvVertices[3].p.x = 0.25F;
	uvVertices[3].p.y = -0.75F;
	uvVertices[3].p.z = 0.25F;
	uvVertices[3].uv.x = 1.0F;
	uvVertices[3].uv.y = 1.0F;

	uvVertices[4].p.x = -0.25F;
	uvVertices[4].p.y = 0.50F;
	uvVertices[4].p.z = 0.25F;
	uvVertices[4].uv.x = 0.0F;
	uvVertices[4].uv.y = 0.0F;

	uvVertices[5].p.x = 0.25F;
	uvVertices[5].p.y = 0.50F;
	uvVertices[5].p.z = 0.25F;
	uvVertices[5].uv.x = 1.0F;
	uvVertices[5].uv.y = 0.0F;



	texture1 = globalObjects->textureMgr->MakeTexture("32bitu.tga");
	texture2 = globalObjects->textureMgr->MakeTexture("starryAlpha.tga");
	texture3 = globalObjects->textureMgr->MakeTexture("mask2.tga");
	texture4 = globalObjects->textureMgr->MakeTexture("grass_03_d.tga");
	texture5 = globalObjects->textureMgr->MakeTexture("dirt_01_d.tga");

	CVertex2 v1(256.0F, 256.0F);
	CVertex2 v2(128.0F + 5.0F, 128.0F + 5.0F);

	overhead = new COverhead(globalObjects, &v1, &v2);


	vertex = new CObject(globalObjects);

	vertexColorShader = globalObjects->shaderMgr->MakeShader("vertexColor.hlsl", "VSMain", "vs_5_0");
	pixelColorShader = globalObjects->shaderMgr->MakeShader("pixelColor.hlsl", "PSMain", "ps_5_0");

	CVertexBuffer* colorVertexBuffer = new CVertexBuffer(globalObjects, vertex->commandList, CVertex::VT_VERTEXRGBA, 6, (void*)colorVertices);
	
	vertex->vertexBuffers[0]->Add(colorVertexBuffer, 0);

	vertex->range->SetRange(0, D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 3);

	vertex->rootSignature = new CRootSignature(globalObjects, 1, vertex->range);

	vertex->pipelineState = new CPipelineState(globalObjects, CVertex::VT_VERTEXRGBA);
	vertex->pipelineState->SetDesc(FALSE, TRUE, TRUE, D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, vertexColorShader, pixelColorShader, nullptr, nullptr, nullptr, vertex->rootSignature->signature);



	pixel = new CObject(globalObjects);

	vertexUVShader = globalObjects->shaderMgr->MakeShader("vertexUV.hlsl", "VSMain", "vs_5_0");
	pixelUVShader = globalObjects->shaderMgr->MakeShader("pixelUV.hlsl", "PSMain", "ps_5_0");

	CVertexBuffer* pixelVertexBuffer = new CVertexBuffer(globalObjects, pixel->commandList, CVertex::VT_VERTEXT, 6, (void*)uvVertices);
	
	pixel->vertexBuffers[0]->Add(pixelVertexBuffer, 0);

	pixel->range->SetRange(0, D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 3);
	pixel->range->SetRange(1, D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 3);
	
	pixel->rootSignature = new CRootSignature(globalObjects, 2, pixel->range);
	
	pixel->pipelineState = new CPipelineState(globalObjects, CVertex::VT_VERTEXT);
	pixel->pipelineState->SetDesc(FALSE, TRUE, TRUE, D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, vertexUVShader, pixelUVShader, nullptr, nullptr, nullptr, pixel->rootSignature->signature);

	pixel->shaderHeap[0]->SlotResource(3, texture2);
	pixel->shaderHeap[0]->SlotResource(4, texture2);
	pixel->shaderHeap[0]->SlotResource(5, texture3);

	pixel->shaderHeap[0]->floats->values[CFloatBuffer::AMBIENT] = 1.0f;


	//modelObject = new CObject(globalObjects, overhead, "cube.model", "vertexModel.hlsl", "VSMain", "vs_5_0", "pixelModel.hlsl", "PSMain", "ps_5_0", FALSE, TRUE);
	//modelObject = new CObject(globalObjects, overhead, "ResidentialBuildings001.model", "vertexModel.hlsl", "VSMain", "vs_5_0", "pixelModel.hlsl", "PSMain", "ps_5_0", FALSE, TRUE);
	//modelObject = new CObject(globalObjects, overhead, "ResidentialBuildings005.model", "vertexModel.hlsl", "VSMain", "vs_5_0", "pixelModel.hlsl", "PSMain", "ps_5_0", FALSE, TRUE);
	//modelObject = new CObject(globalObjects, overhead, "ResidentialBuildings010.model", "vertexModel.hlsl", "VSMain", "vs_5_0", "pixelModel.hlsl", "PSMain", "ps_5_0", FALSE, TRUE);
	//modelObject = new CObject(globalObjects, overhead, "woodenwatchtower2.model", "vertexModel.hlsl", "VSMain", "vs_5_0", "pixelModel.hlsl", "PSMain", "ps_5_0", FALSE, TRUE);
	modelObject = new CObject(globalObjects, overhead, "OldHouse.model", "vertexModel.hlsl", "VSMain", "vs_5_0", "pixelModel.hlsl", "PSMain", "ps_5_0", TRUE, FALSE);


	camera->position.y =   8.0F;
	camera->position.z = -32.0F;

	camera->position.x = -147.898819F;
	camera->position.y =  177.739365F;
	camera->position.z = -246.800385F;

	camera->UpdateRotation(7.0F, 25.0F, 0.0F);
	camera->UpdateView();


	d12Font = globalObjects->fontD12Mgr->MakeFont("Calibri", 20.0F, DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_BOLD);


	v1 = CVertex2(100.0F, 200.0F);
	v2 = CVertex2(globalObjects->window->width - 60.0F, 110.0F);

	sprite = new CSprite(globalObjects, texture2, &v1, &v2);



	vertex->commandList->Close();
	pixel->commandList->Close();
	modelObject->commandList->Close();
	sprite->commandList->Close();
	overhead->commandList->Close();

	ID3D12CommandList* commandLists2[] =
	{
		vertex->commandList.Get(),
		pixel->commandList.Get(),
		modelObject->commandList.Get(),
		sprite->commandList.Get(),
		overhead->commandList.Get()
	};

	globalObjects->videoDevice->commandQueue->ExecuteCommandLists(_countof(commandLists2), commandLists2);

	globalObjects->videoDevice->CreateFence();

	globalObjects->videoDevice->WaitForGpu();

	globalObjects->soundMgr->MakeSound("rifle.wav", 0);
}

void RunFrame()
{
	globalObjects->frametime->Frame();
	
	globalObjects->mouse->GetState();

	for (int k = 0; k < 4; k++)
	{
		if ((globalObjects->mouse->keyMap[k]->count > 0) && (globalObjects->mouse->keyMap[k]->repeater == CMouseDevice::KR_REPEATER))
		{
			switch (globalObjects->mouse->keyMap[k]->packet->type)
			{
				case CPacket::PT_DEFAULT:
				{
					count += 0.01250F;

					pixel->shaderHeap[0]->floats->values[CFloatBuffer::AMBIENT] += count * globalObjects->frametime->frametime;

					break;
				}
				case CPacket::PT_FPSMOVE:
				{
					globalObjects->mouse->MouseMove();

					camera->UpdateRotation((globalObjects->mouse->lY * globalObjects->mouse->sensitivity), (globalObjects->mouse->lX * globalObjects->mouse->sensitivity), (globalObjects->mouse->lZ * globalObjects->mouse->sensitivity));

					globalObjects->mouse->FirstPersonReset();

					break;
				}
			}
		}
		else if ((globalObjects->mouse->keyMap[k]->count == 1) && (globalObjects->mouse->keyMap[k]->repeater == CMouseDevice::KR_ONESHOT))
		{
			switch (globalObjects->mouse->keyMap[k]->packet->type)
			{
				case CPacket::PT_ATTACK:
				{
					globalObjects->soundMgr->GetSound("rifle.wav")->StartSound();

					break;
				}
			}
		}
	}

	globalObjects->keyboard->GetState();

	for (int k = 0; k < 256; k++)
	{
		if ((globalObjects->keyboard->keyMap[k]->count == 1) && (globalObjects->keyboard->keyMap[k]->repeater == CKeyboardDevice::KR_ONESHOT))
		{
			if (globalObjects->keyboard->keyMap[k]->packet)
			{
				switch (globalObjects->keyboard->keyMap[k]->packet->type)
				{
					case CPacket::PT_CONNECT:
					{
						CPacket* packet = new CPacket(0, 0, globalObjects->keyboard->keyMap[k]->packet->audience, globalObjects->keyboard->keyMap[k]->packet->type, "127.0.0.1:26105", 15);

						eventManager->SendEvent(packet);

						delete packet;

						break;
					}
					case CPacket::PT_ATTACK:
					{
						globalObjects->soundMgr->GetSound("rifle.wav")->StartSound();

						break;
					}
					default:
					{
						CPacket* packet = new CPacket(0, 0, globalObjects->keyboard->keyMap[k]->packet->audience, globalObjects->keyboard->keyMap[k]->packet->type);

						eventManager->SendEvent(packet);

						delete packet;

						break;
					}
				}
			}
		}
		else if ((globalObjects->keyboard->keyMap[k]->count > 0) && (globalObjects->keyboard->keyMap[k]->repeater == CKeyboardDevice::KR_REPEATER))
		{
			switch (globalObjects->keyboard->keyMap[k]->packet->type)
			{
				case CPacket::PT_FORWARD:
				{
					XMFLOAT3 direction;

					direction.x = (globalObjects->frametime->frametime * 32.0F) * camera->look.x;
					direction.y = (globalObjects->frametime->frametime * 32.0F) * camera->look.y;
					direction.z = (globalObjects->frametime->frametime * 32.0F) * camera->look.z;

					camera->position.x += direction.x;
					camera->position.y += direction.y;
					camera->position.z += direction.z;

					break;
				}
				case CPacket::PT_BACKWARD:
				{
					XMFLOAT3 direction;

					direction.x = (globalObjects->frametime->frametime * -32.0F) * camera->look.x;
					direction.y = (globalObjects->frametime->frametime * -32.0F) * camera->look.y;
					direction.z = (globalObjects->frametime->frametime * -32.0F) * camera->look.z;

					camera->position.x += direction.x;
					camera->position.y += direction.y;
					camera->position.z += direction.z;

					break;
				}
			}
		}
	}

	camera->UpdateView();

	if (overhead)
	{
		overhead->camera->position.x = camera->position.x;
		overhead->camera->position.y = camera->position.y + 1024.0F;
		overhead->camera->position.z = camera->position.z;

		overhead->camera->UpdateView();
	}

	XMStoreFloat4x4(&vertex->wvp->values, camera->wvpMat);
	XMStoreFloat4x4(&pixel->wvp->values, camera->wvpMat);
	XMStoreFloat4x4(&modelObject->wvp->values, camera->wvpMat);

	DrawFrame();
}

void DrawFrame()
{
	globalObjects->videoDevice->commandAllocators[globalObjects->videoDevice->frameIndex]->Reset();

	globalObjects->videoDevice->commandList->Reset(globalObjects->videoDevice->commandAllocators[globalObjects->videoDevice->frameIndex].Get(), nullptr);
	
	globalObjects->videoDevice->swapChainRenderTargets[globalObjects->videoDevice->frameIndex]->SetPreDrawBarrier();
	globalObjects->videoDevice->swapChainDepthBuffers[globalObjects->videoDevice->frameIndex]->SetPreDrawBarrier();

	globalObjects->videoDevice->commandList->ResourceBarrier(1, &globalObjects->videoDevice->swapChainRenderTargets[globalObjects->videoDevice->frameIndex]->barrier);
	
	globalObjects->videoDevice->commandList->ClearRenderTargetView(globalObjects->videoDevice->swapChainRenderTargets[globalObjects->videoDevice->frameIndex]->handle, CornflowerBlue.GetFloatArray(), 0, nullptr);
	globalObjects->videoDevice->commandList->ClearDepthStencilView(globalObjects->videoDevice->swapChainDepthBuffers[globalObjects->videoDevice->frameIndex]->handle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	
	if (overhead)
	{
		globalObjects->videoDevice->commandList->ClearRenderTargetView(overhead->renderTargets[globalObjects->videoDevice->frameIndex]->handle, LovelyPurple.GetFloatArray(), 0, nullptr);
		globalObjects->videoDevice->commandList->ClearDepthStencilView(overhead->depthBuffers[globalObjects->videoDevice->frameIndex]->handle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	}


	if (pixel->shaderHeap[0]->floats->values[CFloatBuffer::AMBIENT] > 1.0F)
	{
		pixel->shaderHeap[0]->floats->values[CFloatBuffer::AMBIENT] = 0.0F;
		
		count = 0.0F;
	}

	pixel->shaderHeap[0]->ints->values[3] = 1;// rand() % 5;


	deg += 90.0F * globalObjects->frametime->frametime;

	if (deg >= 360.0F)
	{
		deg = 0.0F;
	}
	
	vertex->Draw();

	pixel->Draw();


	//modelObject->yaw = ((rand() % 180) - 90) * globalObjects->frametime->frametime;
	//modelObject->yaw = 45.0F * globalObjects->frametime->frametime;

	for (UINT mc = 0; mc < CMaterial::MATERIAL_COUNT; mc++)
	{
		if (modelObject->shaderHeap[mc])
		{
			modelObject->shaderHeap[mc]->floats->values[5] = pixel->shaderHeap[0]->floats->values[CFloatBuffer::AMBIENT];

			if (overhead)
			{
				modelObject->overheadHeap[mc]->floats->values[5] = pixel->shaderHeap[0]->floats->values[CFloatBuffer::AMBIENT];
			}
		}
	}


	modelObject->Draw();

	sprite->Draw();


	if (overhead)
	{
		overhead->Draw();
	}

	
	globalObjects->videoDevice->commandList->Close();

	ID3D12CommandList* commandLists[] =
	{
		globalObjects->videoDevice->commandList.Get(),
		modelObject->commandList.Get(),
		vertex->commandList.Get(),
		pixel->commandList.Get(),
		sprite->commandList.Get(),
		overhead->commandList.Get()
	};

	
	globalObjects->videoDevice->commandQueue->ExecuteCommandLists(_countof(commandLists), commandLists);

	RenderUI();

	globalObjects->videoDevice->MoveToNextFrame();
}

void RenderUI()
{
	globalObjects->videoDevice->Init2DDraw();

	char text[50] = {};

	sprintf_s(text, 50, "FPS: %i %f", globalObjects->frametime->rframecount, globalObjects->frametime->rframetime);

	if (d12Font)
	{
		float h = 10.0F;

		d12Font->Draw(text, 10.0F, h, &SunYellow);

		h += d12Font->height;

		memset(text, 0x00, 50);

		sprintf_s(text, 50, "%i %i %i", globalObjects->mouse->rgbButtons[0], globalObjects->mouse->rgbButtons[1], globalObjects->mouse->rgbButtons[2]);

		d12Font->Draw(text, 10.0F, h, &SunYellow);
	}

	globalObjects->videoDevice->End2DDraw();
}