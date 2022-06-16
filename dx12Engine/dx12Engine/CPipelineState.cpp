#include "CPipelineState.h"

CPipelineState::CPipelineState()
{
	memset(this, 0x00, sizeof(CPipelineState));
}

CPipelineState::CPipelineState(CGlobalObjects* g, BYTE type)
{
	memset(this, 0x00, sizeof(CPipelineState));

	globalObjects = g;

	switch (type)
	{
		case CVertex::VT_VERTEXRGBA:
		{
			vertexDescCount	= 2;

			vertexDesc = new D3D12_INPUT_ELEMENT_DESC[2];

			vertexDesc[0] = { "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
			vertexDesc[1] = { "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

			break;
		}

		case CVertex::VT_VERTEXT:
		{
			vertexDescCount	= 2;

			vertexDesc = new D3D12_INPUT_ELEMENT_DESC[2];

			vertexDesc[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
			vertexDesc[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

			break;
		}

		case CVertex::VT_VERTEXNT:
		{
			vertexDescCount	= 3;

			vertexDesc = new D3D12_INPUT_ELEMENT_DESC[3];

			vertexDesc[0] = { "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
			vertexDesc[1] = { "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
			vertexDesc[2] = { "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

			break;
		}

		case CVertex::VT_VERTEX2D:
		{
			vertexDescCount = 2;

			vertexDesc = new D3D12_INPUT_ELEMENT_DESC[2];

			vertexDesc[0] = { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
			vertexDesc[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

			break;
		}
	}


	desc.RasterizerState.FillMode = D3D12_FILL_MODE::D3D12_FILL_MODE_SOLID;
	desc.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_BACK;
	desc.RasterizerState.FrontCounterClockwise = FALSE;
	desc.RasterizerState.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
	desc.RasterizerState.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
	desc.RasterizerState.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
	desc.RasterizerState.DepthClipEnable = TRUE;
	desc.RasterizerState.MultisampleEnable = FALSE;
	desc.RasterizerState.AntialiasedLineEnable = FALSE;
	desc.RasterizerState.ForcedSampleCount = 0;
	desc.RasterizerState.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE::D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

	desc.BlendState.IndependentBlendEnable = FALSE;

	blendDesc.LogicOpEnable = FALSE;
	blendDesc.SrcBlend = D3D12_BLEND::D3D12_BLEND_ONE;
	blendDesc.DestBlend = D3D12_BLEND::D3D12_BLEND_INV_SRC_ALPHA;
	blendDesc.BlendOp = D3D12_BLEND_OP::D3D12_BLEND_OP_ADD;
	blendDesc.SrcBlendAlpha = D3D12_BLEND::D3D12_BLEND_ONE;
	blendDesc.DestBlendAlpha = D3D12_BLEND::D3D12_BLEND_INV_SRC_ALPHA;
	blendDesc.BlendOpAlpha = D3D12_BLEND_OP::D3D12_BLEND_OP_ADD;
	blendDesc.LogicOp = D3D12_LOGIC_OP::D3D12_LOGIC_OP_NOOP;
	blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE::D3D12_COLOR_WRITE_ENABLE_ALL;

	/*
	These are valid formats for a depth-stencil view:
	DXGI_FORMAT_D16_UNORM
	DXGI_FORMAT_D24_UNORM_S8_UINT
	DXGI_FORMAT_D32_FLOAT
	DXGI_FORMAT_D32_FLOAT_S8X24_UINT
	DXGI_FORMAT_UNKNOWN
	*/

	desc.DSVFormat = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;

	desc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK::D3D12_DEPTH_WRITE_MASK_ALL;
	desc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC::D3D12_COMPARISON_FUNC_LESS_EQUAL;

	desc.DepthStencilState.FrontFace.StencilFailOp = D3D12_STENCIL_OP::D3D12_STENCIL_OP_KEEP;
	desc.DepthStencilState.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP::D3D12_STENCIL_OP_KEEP;
	desc.DepthStencilState.FrontFace.StencilPassOp = D3D12_STENCIL_OP::D3D12_STENCIL_OP_KEEP;
	desc.DepthStencilState.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC::D3D12_COMPARISON_FUNC_ALWAYS;

	desc.DepthStencilState.BackFace.StencilFailOp = D3D12_STENCIL_OP::D3D12_STENCIL_OP_KEEP;
	desc.DepthStencilState.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP::D3D12_STENCIL_OP_KEEP;
	desc.DepthStencilState.BackFace.StencilPassOp = D3D12_STENCIL_OP::D3D12_STENCIL_OP_KEEP;
	desc.DepthStencilState.BackFace.StencilFunc = D3D12_COMPARISON_FUNC::D3D12_COMPARISON_FUNC_ALWAYS;

	desc.DepthStencilState.StencilEnable = FALSE;

	desc.SampleMask = UINT_MAX;
	desc.NumRenderTargets = 1;
	desc.RTVFormats[0] = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
}

CPipelineState::~CPipelineState()
{
	delete[] vertexDesc;
}

void CPipelineState::SetDesc(BOOL AlphaToCoverageEnable, BOOL BlendEnable, BYTE depthEnable, D3D12_PRIMITIVE_TOPOLOGY_TYPE primType, CShader* vs, CShader* ps, CShader* ds, CShader* hs, CShader* gs, ComPtr<ID3D12RootSignature> rs)
{
	// pixel shader blend for masked foliage - set true for masked blend and blendDesc.BlendEnable = false
	// valid alpha values are 0 for no pixel and 1 for pixel
	desc.BlendState.AlphaToCoverageEnable = AlphaToCoverageEnable;

	// regular texture channel alpha blending - set true for alpha blend and desc.BlendState.AlphaToCoverageEnable = false
	// can use alpha channel from texture or set in pixel shader;
	blendDesc.BlendEnable = BlendEnable;

	for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; i++)
	{
		desc.BlendState.RenderTarget[i] = blendDesc;
	}

	desc.InputLayout.pInputElementDescs = vertexDesc;
	desc.InputLayout.NumElements = vertexDescCount;

	desc.pRootSignature = rs.Get();

	if (vs)
	{
		desc.VS.pShaderBytecode = vs->shader->GetBufferPointer();
		desc.VS.BytecodeLength = vs->shader->GetBufferSize();
	}

	if (ps)
	{
		desc.PS.pShaderBytecode = ps->shader->GetBufferPointer();
		desc.PS.BytecodeLength = ps->shader->GetBufferSize();
	}

	if (ds)
	{
		desc.DS.pShaderBytecode = ds->shader->GetBufferPointer();
		desc.DS.BytecodeLength = ds->shader->GetBufferSize();
	}

	if (hs)
	{
		desc.HS.pShaderBytecode = hs->shader->GetBufferPointer();
		desc.HS.BytecodeLength = hs->shader->GetBufferSize();
	}

	if (gs)
	{
		desc.GS.pShaderBytecode = gs->shader->GetBufferPointer();
		desc.GS.BytecodeLength = gs->shader->GetBufferSize();
	}

	desc.DepthStencilState.DepthEnable = depthEnable;
	desc.PrimitiveTopologyType = primType;

	hr = globalObjects->videoDevice->device->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&pipelineState));

	if (hr < 0)
	{
		globalObjects->errorLog->WriteError("CPipelineState::SetDesc::CreateGraphicsPipelineState:%s\n", globalObjects->errorLog->GetDXGIErrorMessage(hr)->name->GetText());

		return;
	}

	pipelineState->SetName(L"CPipelineState::SetDesc");
}
