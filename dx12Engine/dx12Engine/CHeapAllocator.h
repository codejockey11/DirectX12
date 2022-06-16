#ifndef _CHeapAllocator
#define _CHeapAllocator

#include "standard.h"

class CHeapAllocator
{
public:

	UINT	count;
	UINT	es;

	CHeapAllocator();

	~CHeapAllocator();
};
#endif