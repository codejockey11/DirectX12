#ifndef _CHeapArray
#define _CHeapArray

#include "standard.h"

#include "CHeapAllocator.h"

class CHeapArray
{
public:

	UINT mem;

	BYTE* heap;

	CHeapArray();

	CHeapArray(UINT sz, bool t, bool slc, UINT count, ...);

	~CHeapArray();

	BYTE* GetElement(UINT count, ...);

	UINT GetOffset(UINT count, ...);

private:

	CHeapAllocator* allocator;

	UINT e;

	UINT size;

	BYTE* p;

	bool limitCheck;

	va_list argptr;
};
#endif