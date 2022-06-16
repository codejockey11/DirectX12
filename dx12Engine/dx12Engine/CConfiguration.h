#ifndef _CConfiguration
#define _CConfiguration

#include "standard.h"

#include "CConfigurationItem.h"
#include "CLinkList.h"
#include "CString.h"

class CConfiguration
{
public:

	CString* filename;

	CLinkList* items;

	CConfiguration();

	CConfiguration(const CHAR* fn);

	~CConfiguration();

private:

	FILE* cfp;

	CHAR ib[80];

	FLOAT v;

	errno_t err;
};
#endif
