#ifndef _CWavefront
#define _CWavefront

#include "standard.h"

#include "CGlobalObjects.h"
#include "CLinkList.h"
#include "CMaterial.h"
#include "CString.h"
#include "CVertex.h"
#include "CVertexBuffer.h"

// Model texture needs to be a top left tga

class CWavefront
{
public:

	enum
	{
		VIEW_COUNT = 65536,
		LINE_LENGTH = 132,
	};

	CString* filename;

	CString* materialFilename;

	CVertex vertex[CWavefront::VIEW_COUNT];
	
	CVertex normal[CWavefront::VIEW_COUNT];
	
	CVertex2 uv[CWavefront::VIEW_COUNT];

	char materialName[CMaterial::MATERIAL_COUNT][CMaterial::MATERIAL_LENGTH];

	UINT vi[8];

	UINT ni[8];

	UINT uvi[8];

	CVertexNT vnt[CMaterial::MATERIAL_COUNT][CWavefront::VIEW_COUNT];

	CVertexNT vntl[CMaterial::MATERIAL_COUNT][CWavefront::VIEW_COUNT];

	int vntcount;

	CVertexBuffer* vertexBuffer;

	int icount[CMaterial::MATERIAL_COUNT];

	CMaterial* mat[CMaterial::MATERIAL_COUNT];

	CWavefront();

	CWavefront(CGlobalObjects* g, const char* fn, const char* mn);

	void TextureName(const char* p, char* s);

	~CWavefront();

	void WriteVertices(FILE* f, CVertex* p);

	void LoadBuffers(ID3D12GraphicsCommandList* cl, CVertex* p, CLinkList* vertexBuffers, CMaterial** materials);

private:

	CGlobalObjects* globalObjects;

	FILE* file;

	errno_t err;

	size_t br;

	char line[CWavefront::LINE_LENGTH];

	char path[CWavefront::LINE_LENGTH];

	UINT vindex;
	UINT nindex;
	UINT uvindex;

	int inbr;

	int mnbr;

	int maxIndex;
};
#endif
