#include "CRootSignature.h"

CRootSignature::CRootSignature()
{
	memset(this, 0x00, sizeof(CRootSignature));
}

CRootSignature::CRootSignature(CGlobalObjects* g, UINT n, CRange* range)
{
	memset(this, 0x00, sizeof(CRootSignature));

	globalObjects = g;

	featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION::D3D_ROOT_SIGNATURE_VERSION_1_1;

	hr = globalObjects->videoDevice->device->CheckFeatureSupport(D3D12_FEATURE::D3D12_FEATURE_ROOT_SIGNATURE, &featureData, sizeof(D3D12_FEATURE_DATA_ROOT_SIGNATURE));

	if (hr < 0)
	{
		featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION::D3D_ROOT_SIGNATURE_VERSION_1_0;
	}

	rootParameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE::D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameter.ShaderVisibility = D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_ALL;

	if (n > 0)
	{
		rootParameter.DescriptorTable.NumDescriptorRanges = n;
		rootParameter.DescriptorTable.pDescriptorRanges = range->range;
	}

	sampler[0].Filter = D3D12_FILTER::D3D12_FILTER_ANISOTROPIC;
	sampler[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE::D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	sampler[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE::D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	sampler[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE::D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	sampler[0].MipLODBias = 0;
	sampler[0].MaxAnisotropy = 4;
	sampler[0].ComparisonFunc = D3D12_COMPARISON_FUNC::D3D12_COMPARISON_FUNC_NEVER;
	sampler[0].BorderColor = D3D12_STATIC_BORDER_COLOR::D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	sampler[0].MinLOD = 0.0F;
	sampler[0].MaxLOD = D3D12_FLOAT32_MAX;
	sampler[0].ShaderRegister = 0;
	sampler[0].RegisterSpace = 0;
	sampler[0].ShaderVisibility = D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_PIXEL;

	sampler[1].Filter = D3D12_FILTER::D3D12_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
	sampler[1].AddressU = D3D12_TEXTURE_ADDRESS_MODE::D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	sampler[1].AddressV = D3D12_TEXTURE_ADDRESS_MODE::D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	sampler[1].AddressW = D3D12_TEXTURE_ADDRESS_MODE::D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	sampler[1].MipLODBias = 0;
	sampler[1].MaxAnisotropy = 0;
	sampler[1].ComparisonFunc = D3D12_COMPARISON_FUNC::D3D12_COMPARISON_FUNC_NEVER;
	sampler[1].BorderColor = D3D12_STATIC_BORDER_COLOR::D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	sampler[1].MinLOD = 0.0F;
	sampler[1].MaxLOD = D3D12_FLOAT32_MAX;
	sampler[1].ShaderRegister = 1;
	sampler[1].RegisterSpace = 0;
	sampler[1].ShaderVisibility = D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_PIXEL;

	desc.Version = featureData.HighestVersion;

	desc.Desc_1_1.NumParameters = 1;
	desc.Desc_1_1.pParameters = &rootParameter;
	desc.Desc_1_1.NumStaticSamplers = CRootSignature::SAMPLER_COUNT;
	desc.Desc_1_1.pStaticSamplers = sampler;
	desc.Desc_1_1.Flags = D3D12_ROOT_SIGNATURE_FLAGS::D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	hr = D3D12SerializeVersionedRootSignature(&desc, &tempSignature, &errors);

	if (hr < 0)
	{
		globalObjects->errorLog->WriteError("CRootSignature::CRootSignature::D3D12SerializeVersionedRootSignature:%s\n", globalObjects->errorLog->GetDXGIErrorMessage(hr)->name->GetText());

		if (errors)
		{
			globalObjects->errorLog->WriteError("%s\n", errors->GetBufferPointer());
		}

		return;
	}

	hr = globalObjects->videoDevice->device->CreateRootSignature(0, tempSignature->GetBufferPointer(), tempSignature->GetBufferSize(), IID_PPV_ARGS(&signature));

	if (hr < 0)
	{
		globalObjects->errorLog->WriteError("CRootSignature::CRootSignature::CreateRootSignature:%s\n", globalObjects->errorLog->GetDXGIErrorMessage(hr)->name->GetText());

		return;
	}

	signature->SetName(L"CRootSignature::RootSignature");
}

CRootSignature::~CRootSignature()
{
}
