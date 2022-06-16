#ifndef _CWinsockErrorItem
#define _CWinsockErrorItem

#include "standard.h"

#include "CString.h"

class CWinsockErrorItem
{
public:

	UINT nbr;

	CString* name;

	CString* msg;

	CWinsockErrorItem();

	CWinsockErrorItem(UINT n, const char* nm, const char* m);

	~CWinsockErrorItem();
};
#endif