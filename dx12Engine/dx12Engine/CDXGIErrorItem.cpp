#include "CDXGIErrorItem.h"

CDXGIErrorItem::CDXGIErrorItem()
{
	memset(this, 0x00, sizeof(CDXGIErrorItem));
};

CDXGIErrorItem::CDXGIErrorItem(UINT n, const char* nm, const char* m)
{
	memset(this, 0x00, sizeof(CDXGIErrorItem));

	nbr = n;

	name = new CString(nm);
	msg = new CString(m);
};

CDXGIErrorItem::~CDXGIErrorItem()
{
	delete name;
	delete msg;
};