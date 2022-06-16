#include "CWavefront.h"

CWavefront::CWavefront()
{
	memset(this, 0x00, sizeof(CWavefront));
}

CWavefront::CWavefront(CGlobalObjects* g, const char* fn, const char* mn)
{
	memset(this, 0x00, sizeof(CWavefront));

	globalObjects = g;

	filename = new CString(fn);

	materialFilename = new CString(mn);

	err = fopen_s(&file, filename->GetText(), "r");

	if (err)
	{
		globalObjects->errorLog->WriteError("CWavefront::CWavefront::fopen_s:%s\n", filename->GetText());

		return;
	}

	BYTE ft = TRUE;

	inbr = -1;

	if (file)
	{
		while (fscanf_s(file, "%s", line, CWavefront::LINE_LENGTH) != EOF)
		{
			if (strcmp(line, "usemtl") == 0)
			{
				inbr++;

				br = fscanf_s(file, "%s\n", &materialName[inbr], CMaterial::MATERIAL_LENGTH);

				if (ft)
				{
					ft = FALSE;
				}
				else if (inbr > 0)
				{
					icount[inbr - 1] = vntcount;

					vntcount = 0;
				}
			}

			if (strcmp(line, "v") == 0)
			{
				br = fscanf_s(file, "%f %f %f\n", &vertex[vindex].p.x, &vertex[vindex].p.y, &vertex[vindex].p.z);
				
				vindex++;
			}

			if (strcmp(line, "vt") == 0)
			{
				br = fscanf_s(file, "%f %f\n", &uv[uvindex].p.x, &uv[uvindex].p.y);
				
				uvindex++;
			}

			if (strcmp(line, "vn") == 0)
			{
				br = fscanf_s(file, "%f %f %f\n", &normal[nindex].p.x, &normal[nindex].p.y, &normal[nindex].p.z);

				nindex++;
			}

			if (strcmp(line, "f") == 0)
			{
				for (UINT i = 0; i < 8; i++)
				{
					vi[i] = uvi[i] = ni[i] = 0;
				}

				br = fscanf_s(file, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n",
					&vi[0], &uvi[0], &ni[0],
					&vi[1], &uvi[1], &ni[1],
					&vi[2], &uvi[2], &ni[2],
					&vi[3], &uvi[3], &ni[3],
					&vi[4], &uvi[4], &ni[4],
					&vi[5], &uvi[5], &ni[5],
					&vi[6], &uvi[6], &ni[6],
					&vi[7], &uvi[7], &ni[7]
				);

				for (UINT i = 0; i < 3; i++)
				{
					if ((vi[i] > 0) && (ni[i] > 0) && (uvi[i] > 0))
					{
						vnt[inbr][vntcount].p.x = vertex[vi[i] - 1].p.x;
						vnt[inbr][vntcount].p.y = vertex[vi[i] - 1].p.y;
						vnt[inbr][vntcount].p.z = vertex[vi[i] - 1].p.z;

						vnt[inbr][vntcount].n.x = normal[ni[i] - 1].p.x;
						vnt[inbr][vntcount].n.y = normal[ni[i] - 1].p.y;
						vnt[inbr][vntcount].n.z = normal[ni[i] - 1].p.z;

						vnt[inbr][vntcount].uv.x = uv[uvi[i] - 1].p.x;
						vnt[inbr][vntcount].uv.y = uv[uvi[i] - 1].p.y;

						vntcount++;
					}
				}

				maxIndex = 0;

				for (UINT i = 3; i < 8; i++)
				{
					if (vi[i] != 0)
					{
						maxIndex = i - 2;
					}
				}

				for (int i = 0; i < maxIndex; i++)
				{
					vnt[inbr][vntcount].p.x = vertex[vi[i] - 1].p.x;
					vnt[inbr][vntcount].p.y = vertex[vi[i] - 1].p.y;
					vnt[inbr][vntcount].p.z = vertex[vi[i] - 1].p.z;

					vnt[inbr][vntcount].n.x = normal[ni[i] - 1].p.x;
					vnt[inbr][vntcount].n.y = normal[ni[i] - 1].p.y;
					vnt[inbr][vntcount].n.z = normal[ni[i] - 1].p.z;

					vnt[inbr][vntcount].uv.x = uv[uvi[i] - 1].p.x;
					vnt[inbr][vntcount].uv.y = uv[uvi[i] - 1].p.y;

					vntcount++;


					vnt[inbr][vntcount].p.x = vertex[vi[i + 2] - 1].p.x;
					vnt[inbr][vntcount].p.y = vertex[vi[i + 2] - 1].p.y;
					vnt[inbr][vntcount].p.z = vertex[vi[i + 2] - 1].p.z;

					vnt[inbr][vntcount].n.x = normal[ni[i + 2] - 1].p.x;
					vnt[inbr][vntcount].n.y = normal[ni[i + 2] - 1].p.y;
					vnt[inbr][vntcount].n.z = normal[ni[i + 2] - 1].p.z;

					vnt[inbr][vntcount].uv.x = uv[uvi[i + 2] - 1].p.x;
					vnt[inbr][vntcount].uv.y = uv[uvi[i + 2] - 1].p.y;

					vntcount++;


					vnt[inbr][vntcount].p.x = vertex[vi[i + 3] - 1].p.x;
					vnt[inbr][vntcount].p.y = vertex[vi[i + 3] - 1].p.y;
					vnt[inbr][vntcount].p.z = vertex[vi[i + 3] - 1].p.z;

					vnt[inbr][vntcount].n.x = normal[ni[i + 3] - 1].p.x;
					vnt[inbr][vntcount].n.y = normal[ni[i + 3] - 1].p.y;
					vnt[inbr][vntcount].n.z = normal[ni[i + 3] - 1].p.z;

					vnt[inbr][vntcount].uv.x = uv[uvi[i + 3] - 1].p.x;
					vnt[inbr][vntcount].uv.y = uv[uvi[i + 3] - 1].p.y;

					vntcount++;
				}
			}
		}

		if (inbr < CMaterial::MATERIAL_COUNT)
		{
			icount[inbr] = vntcount;
		}

		fclose(file);
	}


	err = fopen_s(&file, materialFilename->GetText(), "r");

	if (err)
	{
		globalObjects->errorLog->WriteError("CWavefront::CWavefront::fopen_s:%s\n", materialFilename->GetText());

		return;
	}

	mnbr = -1;

	char str[CMaterial::MATERIAL_LENGTH];

	if (file)
	{
		while (fscanf_s(file, "%s", line, CWavefront::LINE_LENGTH) != EOF)
		{
			if (strcmp(line, "newmtl") == 0)
			{
				mnbr++;

				br = fscanf_s(file, "%s\n", &line, CMaterial::MATERIAL_LENGTH);

				mat[mnbr] = new CMaterial(line);
			}

			if (strcmp(line, "Ka") == 0)
			{
				br = fscanf_s(file, "%f %f %f\n", &mat[mnbr]->ambient.x, &mat[mnbr]->ambient.y, &mat[mnbr]->ambient.z);
			}

			if (strcmp(line, "Kd") == 0)
			{
				br = fscanf_s(file, "%f %f %f\n", &mat[mnbr]->diffuse.x, &mat[mnbr]->diffuse.y, &mat[mnbr]->diffuse.z);
			}

			if (strcmp(line, "Ks") == 0)
			{
				br = fscanf_s(file, "%f %f %f\n", &mat[mnbr]->specular.x, &mat[mnbr]->specular.y, &mat[mnbr]->specular.z);
			}

			if (strcmp(line, "Ke") == 0)
			{
				br = fscanf_s(file, "%f %f %f\n", &mat[mnbr]->emissive.x, &mat[mnbr]->emissive.y, &mat[mnbr]->emissive.z);
			}

			if (strcmp(line, "Ni") == 0)
			{
				br = fscanf_s(file, "%f\n", &mat[mnbr]->opticalDensity);
			}

			if (strcmp(line, "d") == 0)
			{
				br = fscanf_s(file, "%f\n", &mat[mnbr]->opacity);
			}

			if (strcmp(line, "illum") == 0)
			{
				br = fscanf_s(file, "%i\n", &mat[mnbr]->illum);
			}

			if (strcmp(line, "map_Kd") == 0)
			{
				UINT c = 0;

				memset((void*)path, 0x00, CMaterial::MATERIAL_LENGTH);

				while (path[c] != '\n')
				{
					br = fscanf_s(file, "%c", &path[c], 1);
					
					if (path[c] == '\n')
					{
						path[c] = 0x00;

						memset(str, 0x00, CMaterial::MATERIAL_LENGTH);

						CWavefront::TextureName(path, str);

						if (globalObjects)
						{
							mat[mnbr]->map_Kd = globalObjects->textureMgr->MakeTexture(str);
						}
						
						break;
					}
					else if (path[c] != ' ')
					{
						c++;
					}
				}
			}

			if (strcmp(line, "map_Ks") == 0)
			{
				UINT c = 0;

				memset((void*)path, 0x00, CMaterial::MATERIAL_LENGTH);

				while (path[c] != '\n')
				{
					br = fscanf_s(file, "%c", &path[c], 1);

					if (path[c] == '\n')
					{
						path[c] = 0x00;

						memset(str, 0x00, CMaterial::MATERIAL_LENGTH);

						CWavefront::TextureName(path, str);

						if (globalObjects)
						{
							mat[mnbr]->map_Ks = globalObjects->textureMgr->MakeTexture(str);
						}

						break;
					}
					else if (path[c] != ' ')
					{
						c++;
					}
				}
			}

			if (strcmp(line, "map_Bump") == 0)
			{
				UINT c = 0;

				memset((void*)path, 0x00, CMaterial::MATERIAL_LENGTH);

				while (path[c] != '\n')
				{
					br = fscanf_s(file, "%c", &path[c], 1);

					if (path[c] == '\n')
					{
						path[c] = 0x00;

						memset(str, 0x00, CMaterial::MATERIAL_LENGTH);

						CWavefront::TextureName(path, str);

						if (globalObjects)
						{
							mat[mnbr]->map_Kb = globalObjects->textureMgr->MakeTexture(str);
						}

						break;
					}
					else if (path[c] != ' ')
					{
						c++;
					}
				}
			}
		}

		fclose(file);
	}

}

void CWavefront::TextureName(const char* path, char* str)
{
	UINT start = 0;

	for (UINT sp = 0; sp < strlen(path); sp++)
	{
		if (path[sp] == '\\')
		{
			start = sp;
		}
	}

	UINT sc = 0;

	for (UINT sp = start + 1; sp < strlen(path); sp++)
	{
		str[sc] = path[sp];

		sc++;
	}
}

CWavefront::~CWavefront()
{
	delete filename;

	delete materialFilename;

	for (UINT mc = 0; mc < CMaterial::MATERIAL_COUNT; mc++)
	{
		delete mat[mc];
	}
}

void CWavefront::WriteVertices(FILE* f, CVertex* p)
{
	if (f == nullptr)
	{
		return;
	}

	XMFLOAT3 pos;
	XMFLOAT3 nrm;
	XMFLOAT3 uv;

	int mcount = inbr + 1;

	fwrite(&mcount, sizeof(int), 1, f);

	for (int mc = 0; mc < inbr + 1; mc++)
	{
		fwrite(&icount[mc], sizeof(int), 1, f);

		for (int vc = 0; vc < icount[mc]; vc++)
		{
			pos.x = vnt[mc][vc].p.x + p->p.x;
			pos.y = vnt[mc][vc].p.y + p->p.y;
			pos.z = vnt[mc][vc].p.z + p->p.z;

			nrm.x = vnt[mc][vc].n.x;
			nrm.y = vnt[mc][vc].n.y;
			nrm.z = vnt[mc][vc].n.z;

			uv.x = vnt[mc][vc].uv.x;
			uv.y = vnt[mc][vc].uv.y;

			fwrite(&pos.x, sizeof(float), 1, f);
			fwrite(&pos.y, sizeof(float), 1, f);
			fwrite(&pos.z, sizeof(float), 1, f);

			fwrite(&nrm.x, sizeof(float), 1, f);
			fwrite(&nrm.y, sizeof(float), 1, f);
			fwrite(&nrm.z, sizeof(float), 1, f);

			fwrite(&uv.x, sizeof(float), 1, f);
			fwrite(&uv.y, sizeof(float), 1, f);
		}
	}
}

void CWavefront::LoadBuffers(ID3D12GraphicsCommandList* cl, CVertex* p, CLinkList* vertexBuffers, CMaterial** materials)
{
	if ((p->p.x == 0.0F) && (p->p.y == 0.0F) && (p->p.z == 0.0F))
	{
		for (int mc = 0; mc < inbr + 1; mc++)
		{
			vertexBuffer = new CVertexBuffer(globalObjects, cl, CVertex::VT_VERTEXNT, icount[mc], vnt[mc]);

			vertexBuffers->Add(vertexBuffer, 0);

			if (mat[mc])
			{
				materials[mc] = new CMaterial();

				materials[mc]->Copy(mat[mc]);
			}
		}
	}
	else
	{
		for (int mc = 0; mc < inbr + 1; mc++)
		{
			for (int vc = 0; vc < icount[mc]; vc++)
			{
				vntl[mc][vc].p.x = vnt[mc][vc].p.x + p->p.x;
				vntl[mc][vc].p.y = vnt[mc][vc].p.y + p->p.y;
				vntl[mc][vc].p.z = vnt[mc][vc].p.z + p->p.z;

				vntl[mc][vc].n.x = vnt[mc][vc].n.x;
				vntl[mc][vc].n.y = vnt[mc][vc].n.y;
				vntl[mc][vc].n.z = vnt[mc][vc].n.z;

				vntl[mc][vc].uv.x = vnt[mc][vc].uv.x;
				vntl[mc][vc].uv.y = vnt[mc][vc].uv.y;
			}
		}

		for (int mc = 0; mc < inbr + 1; mc++)
		{
			vertexBuffer = new CVertexBuffer(globalObjects, cl, CVertex::VT_VERTEXNT, icount[mc], vntl[mc]);

			vertexBuffers->Add(vertexBuffer, 0);

			if (mat[mc])
			{
				materials[mc] = new CMaterial();

				materials[mc]->Copy(mat[mc]);
			}
		}
	}
}
//
// f: 0 1 2 3
//
// 0 1 2
// 0 2 3
//
// 3--2
// | /|
// |/ |
// 0--1
//
// f: A B C D E F G
//
// A B C
// A C D
// B D E
// C E F
// D F G
//
