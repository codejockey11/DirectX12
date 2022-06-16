#include "CRange.h"

CRange::CRange()
{
	memset(this, 0x00, sizeof(CRange));
}

void CRange::SetRange(UINT i, D3D12_DESCRIPTOR_RANGE_TYPE rt, UINT n)
{
	index = i;

	range[index].RangeType = rt;
	range[index].NumDescriptors = n;
	range[index].BaseShaderRegister = 0;
	range[index].RegisterSpace = 0;
	range[index].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	range[index].Flags = D3D12_DESCRIPTOR_RANGE_FLAGS::D3D12_DESCRIPTOR_RANGE_FLAG_DESCRIPTORS_VOLATILE;
}

CRange::~CRange()
{
}
