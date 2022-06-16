#include "CConfigurationItem.h"

CConfigurationItem::CConfigurationItem()
{
	memset(this, 0x00, sizeof(CConfigurationItem));
}

CConfigurationItem::CConfigurationItem(const CHAR* ib, FLOAT v)
{
	variable = new CString(ib);

	value = v;
}

CConfigurationItem::~CConfigurationItem()
{
	delete variable;
}