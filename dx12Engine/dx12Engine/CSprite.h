#ifndef _CSprite
#define _CSprite

#include "standard.h"

#include "CBoundBox.h"
#include "CGlobalObjects.h"
#include "CIndexBuffer.h"
#include "CRange.h"
#include "CRootSignature.h"
#include "CShaderHeap.h"
#include "CTexture.h"
#include "CVertexBuffer.h"

class CSprite
{
public:

	CVertex2 scaleSize;
			 
	CVertex2 position;

	CBoundBox* box;

	ComPtr<ID3D12CommandAllocator> commandAllocators[CVideoDevice::FRAME_COUNT];

	ComPtr<ID3D12GraphicsCommandList> commandList;

	CPipelineState* pipelineState;

	CRange* range;

	CRootSignature* rootSignature;

	CShaderHeap* shaderHeap;

	CShader* vertexShader;

	CShader* pixelShader;

	CFloat4x4Buffer* wvp;


	CSprite();

	CSprite(CGlobalObjects* g, CTexture* tc, CVertex2* wh, CVertex2* p);

	~CSprite();

	void UpdatePosition();

	void Draw();

private:

	HRESULT hr;

	CGlobalObjects* globalObjects;

	CTexture* texture;

	CVertex2 sheetSize;
	
	CVertex2 cellSize;

	CVertexT vertices[4];

	CVertexBuffer* vertexBuffer;
	
	CIndexBuffer* indexBuffer;

	DWORD color;

	DWORD index[6];

	float cl;
	
	float ct;
	
	float cr;
	
	float cb;
};
#endif