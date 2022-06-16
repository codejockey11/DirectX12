#ifndef _CRange
#define _CRange

#include "standard.h"

class CRange
{
public:

	enum
	{
		RANGE_COUNT = 2
	};

	UINT index;

	D3D12_DESCRIPTOR_RANGE1 range[CRange::RANGE_COUNT];

	CRange();

	void SetRange(UINT i, D3D12_DESCRIPTOR_RANGE_TYPE rt, UINT n);

	~CRange();
};
#endif