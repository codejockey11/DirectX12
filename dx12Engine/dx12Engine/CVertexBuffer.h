#ifndef _CVertexBuffer
#define _CVertexBuffer

#include "standard.h"

#include "CGlobalObjects.h"
#include "CPipelineState.h"
#include "CVertex.h"

class CVertexBuffer
{
public:

	float yaw;
	float pitch;
	float roll;

	ComPtr<ID3D12GraphicsCommandList> commandList;

	CVertexBuffer();

	CVertexBuffer(CGlobalObjects* g, ComPtr<ID3D12GraphicsCommandList> cl, BYTE type, UINT count, void* vertices);

	~CVertexBuffer();

	BYTE LoadBuffer(void* vertices);

	void Draw();

	void DrawIndexed(UINT c);

	void UpdateRotation(float x, float y, float z);

	void Update();

private:

	HRESULT hr;

	CGlobalObjects* globalObjects;

	ComPtr<ID3D12Resource> vertexBuffer;
	
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;

	D3D12_HEAP_PROPERTIES heapProperties;
	
	D3D12_RESOURCE_DESC resourceDesc;

	D3D12_RANGE range;

	void* pData;

	BYTE type;

	UINT count;

	XMFLOAT3 rotation;

	XMFLOAT3 position;

	BYTE CreateResource();
};
#endif