#include "CErrorLog.h"

CErrorLog::CErrorLog()
{
	memset(this, 0x00, sizeof(CErrorLog));

	comError = new CComError();
}

CErrorLog::CErrorLog(const char* f)
{
	memset(this, 0x00, sizeof(CErrorLog));

	fopen_s(&file, f, "w");

	comError = new CComError();

	wsError = new CWinsockError();

	dxgiError = new CDXGIError();
}

CErrorLog::~CErrorLog()
{
	delete comError;

	delete wsError;

	delete dxgiError;

	if (file)
	{
		fclose(file);
	}
}

void CErrorLog::WriteError(const char* format, ...)
{
	if (strlen(format) == 0)
	{
		return;
	}

	GetLocalTime(&st);
	
	sprintf_s(timeText, CErrorLog::TIME_LENGTH, "%02d:%02d:%02d - ", st.wHour, st.wMinute, st.wSecond);

#ifdef _DEBUG
	OutputDebugStringA(timeText);
#endif

	va_start(argptr, format);

	vsprintf_s(text, CErrorLog::ERROR_LENGTH, format, argptr);

	va_end(argptr);

#ifdef _DEBUG
	OutputDebugStringA(text);
#endif

	if (file)
	{
		fwrite(timeText, strlen(timeText), 1, file);

		fwrite(text, strlen(text), 1, file);

#ifdef _DEBUG
		fflush(file);
#endif
	}

	memset(text, 0x00, 1024);
}

char* CErrorLog::GetComErrorMessage(HRESULT hr)
{
	return comError->GetComErrorMessage(hr);
}

CWinsockErrorItem* CErrorLog::GetWinsockErrorMessage(UINT e)
{
	return wsError->GetError(e);
}

CDXGIErrorItem* CErrorLog::GetDXGIErrorMessage(UINT e)
{
	return dxgiError->GetError(e);
}