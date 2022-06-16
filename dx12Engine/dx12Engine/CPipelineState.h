#ifndef _CPipelineState
#define _CPipelineState

#include "standard.h"

#include "CGlobalObjects.h"
#include "CVertex.h"

class CPipelineState
{
public:

	ComPtr<ID3D12PipelineState> pipelineState;

	CPipelineState();

	CPipelineState(CGlobalObjects* g, BYTE type);

	~CPipelineState();

	void SetDesc(BOOL AlphaToCoverageEnable, BOOL BlendEnable, BYTE depthEnable, D3D12_PRIMITIVE_TOPOLOGY_TYPE primType, CShader* vs, CShader* ps, CShader* ds, CShader* hs, CShader* gs, ComPtr<ID3D12RootSignature> rs);

private:

	HRESULT hr;

	CGlobalObjects* globalObjects;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC desc;

	D3D12_RENDER_TARGET_BLEND_DESC blendDesc;

	D3D12_INPUT_ELEMENT_DESC* vertexDesc;

	UINT vertexDescCount;
};
#endif