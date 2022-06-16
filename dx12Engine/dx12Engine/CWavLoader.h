#ifndef _CWavLoader
#define _CWavLoader

#include "standard.h"

#include "CErrorLog.h"
#include "CString.h"
#include "CWavFileHeader.h"

class CWavLoader
{
public:

	CString* filename;

	CWavFileHeader header;

	BYTE* data;

	WAVEFORMATEX wfx;

	CWavLoader();

	CWavLoader(CErrorLog* el, const CHAR* fn);

	~CWavLoader();

private:

	HRESULT hr;

	CErrorLog* errorLog;

	CHAR garbage[32];

	FILE* fp;

	errno_t err;

	size_t br;
};
#endif