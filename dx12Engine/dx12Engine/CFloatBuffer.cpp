#include "CFloatBuffer.h"

CFloatBuffer::CFloatBuffer()
{
	memset(this, 0x00, sizeof(CFloatBuffer));
}

CFloatBuffer::CFloatBuffer(CGlobalObjects* g, ComPtr<ID3D12GraphicsCommandList> cl)
{
	memset(this, 0x00, sizeof(CFloatBuffer));

	globalObjects = g;

	size = CFloatBuffer::VIEW_COUNT * sizeof(float);

	count = CFloatBuffer::VIEW_COUNT;

	stride = sizeof(float);

	commandList = cl;

	constantBuffer = new CConstantBuffer(globalObjects, commandList, size);
}

CFloatBuffer::~CFloatBuffer()
{
	delete constantBuffer;
}

CIntBuffer::CIntBuffer(CGlobalObjects* g, ComPtr<ID3D12GraphicsCommandList> cl)
{
	memset(this, 0x00, sizeof(CIntBuffer));

	globalObjects = g;

	size = CIntBuffer::VIEW_COUNT * sizeof(int);

	count = CIntBuffer::VIEW_COUNT;

	stride = sizeof(int);

	commandList = cl;

	constantBuffer = new CConstantBuffer(globalObjects, commandList, size);
}

CFloat4x4Buffer::CFloat4x4Buffer(CGlobalObjects* g, ComPtr<ID3D12GraphicsCommandList> cl)
{
	memset(this, 0x00, sizeof(CFloat4x4Buffer));

	globalObjects = g;

	size = sizeof(XMFLOAT4X4);

	count = CFloat4x4Buffer::VIEW_COUNT;

	stride = sizeof(XMFLOAT4X4);

	commandList = cl;

	constantBuffer = new CConstantBuffer(globalObjects, commandList, size);
}
