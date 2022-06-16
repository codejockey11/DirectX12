#ifndef _COverhead
#define _COverhead

#include "standard.h"

#include "CCamera.h"
#include "CDepthBuffer.h"
#include "CFloatBuffer.h"
#include "CGlobalObjects.h"
#include "CIndexBuffer.h"
#include "CPipelineState.h"
#include "CRange.h"
#include "CRenderTarget.h"
#include "CRootSignature.h"
#include "CShader.h"
#include "CShaderHeap.h"
#include "CTexture.h"
#include "CVertex.h"
#include "CVertexBuffer.h"
#include "CViewport.h"

class COverhead
{
public:

	CRenderTarget* renderTargets[CVideoDevice::FRAME_COUNT];

	CDepthBuffer* depthBuffers[CVideoDevice::FRAME_COUNT];

	CViewport* viewport;

	CCamera* camera;

	ComPtr<ID3D12Resource> resources[CVideoDevice::FRAME_COUNT];

	ComPtr<ID3D12CommandAllocator> commandAllocators[CVideoDevice::FRAME_COUNT];

	ComPtr<ID3D12GraphicsCommandList> commandList;

	CPipelineState* pipelineState;

	CRange* range;

	CRootSignature* rootSignature;

	CShaderHeap* shaderHeap;

	CFloat4x4Buffer* wvp;

	CShader* vertexShader;

	CShader* pixelShader;

	CDynamicTexture* texture;

	CVertex2 position;

	CVertex2 scaleSize;

	COverhead();

	COverhead(CGlobalObjects* g, CVertex2* wh, CVertex2* p);

	~COverhead();

	void UpdatePosition();

	void Draw();

private:

	HRESULT hr;

	CGlobalObjects* globalObjects;

	CVertexBuffer* vertexBuffer;

	CVertexT vertices[4];

	CIndexBuffer* indexBuffer;

	DWORD index[6];
};
#endif