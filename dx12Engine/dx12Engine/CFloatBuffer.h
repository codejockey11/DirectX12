#ifndef _CBuffers
#define _CBuffers

#include "standard.h"

#include "CConstantBuffer.h"
#include "CGlobalObjects.h"

// Buffers align on 256 byte boundaries
class CFloatBuffer
{
public:
	
	enum
	{
		AMBIENT = 4,

		VIEW_COUNT = 64
	};

	UINT size;

	UINT count;

	UINT stride;

	CConstantBuffer* constantBuffer;

	CGlobalObjects* globalObjects;

	ComPtr<ID3D12GraphicsCommandList> commandList;

	float values[CFloatBuffer::VIEW_COUNT];

	CFloatBuffer();

	CFloatBuffer(CGlobalObjects* g, ComPtr<ID3D12GraphicsCommandList> cl);

	~CFloatBuffer();
};

class CIntBuffer : public CFloatBuffer
{
public:

	enum
	{
		VIEW_COUNT = 64
	};

	int values[CIntBuffer::VIEW_COUNT];

	CIntBuffer(CGlobalObjects* g, ComPtr<ID3D12GraphicsCommandList> cl);
};

class CFloat4x4Buffer : public CFloatBuffer
{
public:

	enum
	{
		VIEW_COUNT = 1
	};

	XMFLOAT4X4 values;

	CFloat4x4Buffer(CGlobalObjects* g, ComPtr<ID3D12GraphicsCommandList> cl);
};
#endif