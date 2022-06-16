#ifndef _CDXGIError
#define _CDXGIError

#include "standard.h"

#include "CLinkList.h"
#include "CDXGIErrorItem.h"

class CDXGIError
{
public:

	CLinkList* errors;

	CDXGIErrorItem* defaultError;

	CDXGIError();

	~CDXGIError();

	CDXGIErrorItem* GetError(UINT e);

	void AddItem(CDXGIErrorItem* i);
};
#endif
