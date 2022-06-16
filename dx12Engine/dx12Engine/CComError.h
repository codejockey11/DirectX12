#ifndef _CComError
#define _CComError

#include "standard.h"

class CComError
{
public:

	CHAR msg[32];

	CComError();

	~CComError();

	CHAR* GetComErrorMessage(HRESULT hr)
	{
		memset(msg, 0x00, 32);

		_com_error err(hr);

		memcpy_s(msg, 32, err.ErrorMessage(), 32);

		return msg;
	}
};
#endif