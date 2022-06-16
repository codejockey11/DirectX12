#ifndef _CErrorLog
#define _CErrorLog

#include "standard.h"

#include "CComError.h"
#include "CDXGIError.h"
#include "CWinsockError.h"

class CErrorLog
{
public:

	enum
	{
		TIME_LENGTH = 24,
		ERROR_LENGTH = 1024
	};

	CErrorLog();

	CErrorLog(const char* f);

	~CErrorLog();

	void WriteError(const char* format, ...);

	char* GetComErrorMessage(HRESULT hr);

	CWinsockErrorItem* GetWinsockErrorMessage(UINT e);

	CDXGIErrorItem* GetDXGIErrorMessage(UINT e);

private:

	CComError* comError;
	
	CWinsockError* wsError;

	CDXGIError* dxgiError;

	FILE* file;

	char timeText[CErrorLog::TIME_LENGTH];

	char text[CErrorLog::ERROR_LENGTH];

	va_list argptr;

	SYSTEMTIME st;
};
#endif