#ifndef _CObject
#define _CObject

#include "standard.h"

#include "CFloatBuffer.h"
#include "CGlobalObjects.h"
#include "CLinkList.h"
#include "CMaterial.h"
#include "COverhead.h"
#include "CPipelineState.h"
#include "CRange.h"
#include "CRootSignature.h"
#include "CShader.h"
#include "CShaderHeap.h"
#include "CVertexBuffer.h"
#include "CModel.h"

class CObject
{
public:

	CString* name;

	CFloat4x4Buffer* wvp;

	CGlobalObjects* globalObjects;

	CLinkList* vertexBuffers[CMaterial::MATERIAL_COUNT];

	CPipelineState* pipelineState;

	CRange* range;

	CRootSignature* rootSignature;

	CShaderHeap* shaderHeap[CMaterial::MATERIAL_COUNT];

	CMaterial* material[CMaterial::MATERIAL_COUNT];

	CModel* model;

	COverhead* overhead;
	
	CFloat4x4Buffer* overheadWvp;

	CShaderHeap* overheadHeap[CMaterial::MATERIAL_COUNT];

	float pitch;

	float roll;

	float yaw;

	ComPtr<ID3D12CommandAllocator> commandAllocators[CVideoDevice::FRAME_COUNT];

	ComPtr<ID3D12GraphicsCommandList> commandList;

	SHORT materialCount;

	CShader* vertexShader;

	CShader* pixelShader;

	CObject();

	CObject(CGlobalObjects* g);

	CObject(CGlobalObjects* g, COverhead* o, const char* fn, const char* vsn, const char* vep, const char* vv, const char* psn, const char* pep, const char* pv, BOOL AlphaToCoverageEnable, BOOL BlendEnable);

	void Draw();

	~CObject();

private:

	FILE* file;

	CTexture* texture;
};
#endif