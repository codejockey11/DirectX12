#include "CConfiguration.h"

CConfiguration::CConfiguration()
{
	memset(this, 0x00, sizeof(CConfiguration));
}

CConfiguration::CConfiguration(const CHAR* fn)
{
	memset(this, 0x00, sizeof(CConfiguration));

	items = new CLinkList();

	filename = new CString(fn);

	err = fopen_s(&cfp, filename->GetText(), "r");

	if (cfp)
	{
		err = fscanf_s(cfp, "%s %f\n", ib, 80, &v);

		while (err != 0xFFFFFFFF)
		{
			CConfigurationItem* ci = new CConfigurationItem(ib, v);

			items->Add(ci, ci->variable->GetText());

			err = fscanf_s(cfp, "%s %f\n", ib, 80, &v);
		}
	}

	if (cfp)
	{
		fclose(cfp);
	}
}

CConfiguration::~CConfiguration()
{
	delete items;
	delete filename;
}