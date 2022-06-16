#include "CWinsockErrorItem.h"

CWinsockErrorItem::CWinsockErrorItem()
{
	memset(this, 0x00, sizeof(CWinsockErrorItem));
}

CWinsockErrorItem::CWinsockErrorItem(UINT n, const char* nm, const char* m)
{
	memset(this, 0x00, sizeof(CWinsockErrorItem));

	nbr = n;

	name = new CString(nm);
	msg = new CString(m);
}

CWinsockErrorItem::~CWinsockErrorItem()
{
	delete name;
	delete msg;
}
