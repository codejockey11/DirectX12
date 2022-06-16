#include "CWavLoader.h"

CWavLoader::CWavLoader()
{
	memset(this, 0x00, sizeof(CWavLoader));
}

CWavLoader::CWavLoader(CErrorLog* e, const CHAR* fn)
{
	memset(this, 0x00, sizeof(CWavLoader));

	errorLog = e;

	filename = new CString(fn);

	err = fopen_s(&fp, filename->GetText(), "rb");

	if (err)
	{
		errorLog->WriteError("CWavLoader::CWavLoader::Failed:%i:%s\n", err, filename->GetText());

		return;
	}

	br = fread_s(&header.chunkID, 4 * sizeof(BYTE), sizeof(BYTE), 4, fp);		//Contains the letters "RIFF" in ASCII form
	br = fread_s(&header.chunkSize, sizeof(INT), sizeof(INT), 1, fp);
	br = fread_s(&header.format, 4 * sizeof(BYTE), sizeof(BYTE), 4, fp);		//Contains the letters "WAVE"
	br = fread_s(&header.subchunk1ID, 4 * sizeof(BYTE), sizeof(BYTE), 4, fp);		//Contains the letters "fmt "
	br = fread_s(&header.subchunk1Size, sizeof(INT), sizeof(INT), 1, fp);		//16 for PCM.This is the size of the rest of the Subchunk which follows this number.
	br = fread_s(&header.audioFormat, 2 * sizeof(BYTE), sizeof(BYTE), 2, fp);		//PCM = 1 (i.e.Linear quantization)
	br = fread_s(&header.numChannels, 2 * sizeof(BYTE), sizeof(BYTE), 2, fp);
	br = fread_s(&header.sampleRate, sizeof(INT), sizeof(INT), 1, fp);
	br = fread_s(&header.byteRate, sizeof(INT), sizeof(INT), 1, fp);
	br = fread_s(&header.blockAlign[2], 2 * sizeof(BYTE), sizeof(BYTE), 2, fp);
	br = fread_s(&header.bitsPerSample, 2 * sizeof(BYTE), sizeof(BYTE), 2, fp);		//8 bits = 8, 16 bits = 16

	if (header.subchunk1Size > 16)
	{
		br = fread_s(&garbage, 32, ((size_t)header.subchunk1Size - 16), 1, fp);
	}

	br = fread_s(&header.subchunk2ID, 4 * sizeof(BYTE), sizeof(BYTE), 4, fp);		//Contains the letters "data"
	br = fread_s(&header.subchunk2Size, sizeof(INT), sizeof(INT), 1, fp);			//NumSamples * NumChannels * BitsPerSample / 8 size	of the read of the subchunk following this number

	data = (BYTE*)malloc(header.subchunk2Size);

	if (data)
	{
		memset(data, 0x00, header.subchunk2Size);

		br = fread_s(data, header.subchunk2Size * sizeof(BYTE), sizeof(BYTE), (size_t)header.subchunk2Size, fp);
	}

	fclose(fp);

	wfx.wFormatTag = header.audioFormat[0];
	wfx.nChannels = header.numChannels[0];
	wfx.nSamplesPerSec = header.sampleRate;
	wfx.nAvgBytesPerSec = header.byteRate;
	wfx.wBitsPerSample = header.bitsPerSample[0];
	wfx.nBlockAlign = (wfx.nChannels * wfx.wBitsPerSample) / 8;
	wfx.cbSize = 0;
}

CWavLoader::~CWavLoader()
{
	delete filename;

	if (data)
	{
		free(data);
	}
}