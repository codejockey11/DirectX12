#ifndef _CWavFileHeader
#define _CWavFileHeader

#include "standard.h"

class CWavFileHeader
{
public:

	BYTE chunkID[4];		//Contains the letters "RIFF" in ASCII form
	INT  chunkSize;

	BYTE format[4];			//Contains the letters "WAVE"

	BYTE subchunk1ID[4];	//Contains the letters "fmt "
	INT  subchunk1Size;		//16 for PCM.This is the size of the rest of the Subchunk which follows this number.

	BYTE audioFormat[2];	//PCM = 1 (i.e.Linear quantization)

	BYTE numChannels[2];

	INT sampleRate;

	INT byteRate;

	BYTE blockAlign[2];

	BYTE bitsPerSample[2];

	BYTE subchunk2ID[4];	//Contains the letters "data" (0x64617461 big - endian form).
	INT  subchunk2Size;		//NumSamples * NumChannels * BitsPerSample / 8 size	of the read of the subchunk following this number

	CWavFileHeader();

	~CWavFileHeader();
};
#endif