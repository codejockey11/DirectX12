#include "CHeapArray.h"

CHeapArray::CHeapArray()
{
	memset(this, 0x00, sizeof(CHeapArray));
}

CHeapArray::CHeapArray(UINT sz, bool t, bool slc, UINT count, ...)
{
	memset(this, 0x00, sizeof(CHeapArray));

	limitCheck = slc;

	allocator = new CHeapAllocator[count]();

	size = sz;

	va_start(argptr, count);

	UINT c = va_arg(argptr, UINT);

	allocator[0].count = c;
	allocator[0].es = size * c;

	for (UINT i = 1; i < count; i++)
	{
		c = va_arg(argptr, UINT);

		allocator[i].count = c;
		allocator[i].es = allocator[i - 1].es * c;
	}

	va_end(argptr);

	mem = allocator[count - 1].es;

	if (t)
	{
		heap = (BYTE*)malloc(mem);

		if (heap)
		{
			memset(heap, 0x00, mem);
		}
	}
}

CHeapArray::~CHeapArray()
{
	delete[] allocator;

	if (heap)
	{
		free(heap);
	}
}

BYTE* CHeapArray::GetElement(UINT count, ...)
{
	p = heap;

	va_start(argptr, count);

	UINT c = va_arg(argptr, UINT);

	if (limitCheck)
	{
		if ((c < 0) || (c > allocator[0].count))
		{
			return nullptr;
		}
	}

	e = size * c;

	for (UINT i = 1; i < count; i++)
	{
		UINT c = va_arg(argptr, UINT);

		if (limitCheck)
		{
			if ((c < 0) || (c > allocator[i].count))
			{
				return nullptr;
			}
		}

		e += allocator[i - 1].es * c;
	}

	va_end(argptr);

	return (p + e);
}

UINT  CHeapArray::GetOffset(UINT count, ...)
{
	p = heap;

	va_start(argptr, count);

	UINT c = va_arg(argptr, UINT);

	e = size * c;

	for (UINT i = 1; i < count; i++)
	{
		c = va_arg(argptr, UINT);

		if (limitCheck)
		{
			if ((c < 0) || (c > allocator[i].count))
			{
				return 0;
			}
		}

		e += allocator[i - 1].es * c;
	}

	va_end(argptr);

	return e;
}
