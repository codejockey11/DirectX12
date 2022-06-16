#ifndef _CWinsockError
#define _CWinsockError

#include "standard.h"

#include "CLinkList.h"
#include "CWinsockErrorItem.h"

class CWinsockError
{
public:

	CLinkList* errors;

	CWinsockError();
	
	~CWinsockError();

	CWinsockErrorItem* GetError(UINT e);

	void AddItem(CWinsockErrorItem* i);
};
#endif