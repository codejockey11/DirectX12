#ifndef _CDXGIErrorItem
#define _CDXGIErrorItem

#include "standard.h"

#include "CString.h"

class CDXGIErrorItem
{
public:

	UINT nbr;

	CString* name;

	CString* msg;

	CDXGIErrorItem();

	CDXGIErrorItem(UINT n, const char* nm, const char* m);

	~CDXGIErrorItem();
};
#endif