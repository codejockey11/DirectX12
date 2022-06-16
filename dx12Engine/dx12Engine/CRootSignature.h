#ifndef _CRootSignature
#define _CRootSignature

#include "standard.h"

#include "CGlobalObjects.h"
#include "CRange.h"

class CRootSignature
{
public:

	enum
	{
		SAMPLER_COUNT = 2
	};

	ComPtr<ID3D12RootSignature> signature;

	CRootSignature();

	CRootSignature(CGlobalObjects* g, UINT n, CRange* range);

	~CRootSignature();

private:

	HRESULT hr;

	CGlobalObjects* globalObjects;

	ComPtr<ID3DBlob> tempSignature;

	ComPtr<ID3DBlob> errors;

	D3D12_VERSIONED_ROOT_SIGNATURE_DESC	desc;

	D3D12_ROOT_PARAMETER1 rootParameter;

	D3D12_FEATURE_DATA_ROOT_SIGNATURE featureData;

	D3D12_STATIC_SAMPLER_DESC sampler[CRootSignature::SAMPLER_COUNT];
};
#endif