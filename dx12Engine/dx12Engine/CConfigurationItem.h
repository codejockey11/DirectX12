#ifndef _CConfigurationItem
#define _CConfigurationItem

#include "standard.h"

#include "CString.h"

class CConfigurationItem
{
public:

	CString* variable;

	FLOAT value;

	CConfigurationItem();

	CConfigurationItem(const CHAR* ib, FLOAT v);

	~CConfigurationItem();
};
#endif