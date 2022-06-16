#include "CConsole.h"

CConsole::CConsole()
{
	memset(this, 0x00, sizeof(CConsole));
}

CConsole::CConsole(CGlobalObjects* g, const CHAR* in, const CHAR* fn)
{
	memset(this, 0x00, sizeof(CConsole));

	globalObjects = g;

	commandLine = new CString(50);
	entries = new CLinkList<CString>();
	packet = new CPacket();

	variables = new CConsoleVars();

	background = globalObjects->textureMgr->GetTexture(in);

	font = globalObjects->fontMgr->GetFont(fn);
	
	color = D3DCOLOR_ARGB(255, 255, 255, 255);

	corners.left		= (globalObjects->window->width / 2) / 2;
	corners.top		= (globalObjects->window->height / 2) / 2;
	corners.right		= corners.left + (globalObjects->window->width / 2);
	corners.bottom	= corners.top + (globalObjects->window->height / 2);

	vertices[0].X = (FLOAT)corners.left;
	vertices[0].Y = (FLOAT)corners.top;
	vertices[0].Z = 0.0F;
	vertices[0].W = 1.0F;

	vertices[0].U = 0.0F;
	vertices[0].V = 0.0F;

	vertices[1].X = (FLOAT)corners.right;
	vertices[1].Y = (FLOAT)corners.top;
	vertices[1].Z = 0.0F;
	vertices[1].W = 1.0F;

	vertices[1].U = 1.0F;
	vertices[1].V = 0.0F;

	vertices[2].X = (FLOAT)corners.right;
	vertices[2].Y = (FLOAT)corners.bottom;
	vertices[2].Z = 0.0F;
	vertices[2].W = 1.0F;

	vertices[2].U = 1.0F;
	vertices[2].V = 1.0F;

	vertices[3].X = (FLOAT)corners.left;
	vertices[3].Y = (FLOAT)corners.bottom;
	vertices[3].Z = 0.0F;
	vertices[3].W = 1.0F;

	vertices[3].U = 0.0F;
	vertices[3].V = 1.0F;

	vertexBuffer = new CVertexBuffer(globalObjects, CVertex::VT_VERTEX2D, 4, (VOID*)vertices);

	index[0] = 0;
	index[1] = 2;
	index[2] = 3;
	index[3] = 0;
	index[4] = 1;
	index[5] = 2;

	indexBuffer = new CIndexBuffer(globalObjects, 6, 2, index);
}

CConsole::~CConsole()
{
	delete commandLine;
	delete entries;
	delete packet;
	delete variables;
	delete vertexBuffer;
	delete indexBuffer;
}

VOID CConsole::DisplayConsole()
{
	globalObjects->videoDevice->device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	globalObjects->videoDevice->device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	globalObjects->videoDevice->device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	hr = globalObjects->videoDevice->device->SetTexture(0, background->image);

	vertexBuffer->DrawIndexed(indexBuffer);

	lines = ((corners.bottom - corners.top) / font->desc.Height) - 1;

	CLinkListNode<CString>* el = entries->list;
	
	start = (corners.bottom - font->desc.Height - (font->desc.Height * entries->count));
	
	if (start < corners.top)
	{
		start = (corners.bottom - font->desc.Height - (font->desc.Height * lines));
	}
	
	if (entries->count > lines)
	{
		el = entries->Search(entries->count - lines + scroll);
	}
	else
	{
		el = entries->list;
	}

	if (el)
	{
		while (el->element)
		{
			CString* sc = (CString*)el->element;
			
			font->Draw(sc->GetText(), corners.left + 4, start, &Green);
			
			start += font->desc.Height;
			
			if (start > (corners.bottom - font->desc.Height - font->desc.Height))
			{
				break;
			}
			
			el = el->next;
		}
	}

	font->Draw(commandLine->GetText(), corners.left + 4, corners.bottom - font->desc.Height, &Green);
}

VOID CConsole::AddEventMessage(const CHAR* format, ...)
{
	va_start(argptr, format);

	memset(text, 0x00, 1024);

	vsprintf_s(text, 1024, format, argptr);

	va_end(argptr);

	CString* cl = new CString(text);

	entries->Add(cl, entryNbr);

	entryNbr++;
}

VOID CConsole::AddCHAR(const CHAR wParam)
{
	switch (wParam)
	{
		case VK_BACK:
		{
			cursorLocation--;

			if (cursorLocation < 0)
			{
				cursorLocation = 0;
			}

			commandLine->GetText()[cursorLocation] = 0x00;

			break;
		}

		default:
		{
			if (cursorLocation == commandLine->GetLength())
			{
				cursorLocation = commandLine->GetLength() - 1;
			}

			commandLine->GetText()[cursorLocation] = (CHAR)wParam;
			
			cursorLocation++;

			break;
		}
	}
}

VOID CConsole::Paging(CHAR wParam)
{
	if (wParam == VK_PRIOR)
	{
		scroll--;
	}
	
	if ((entries->count - lines + scroll) < 0)
	{
		scroll++;
	}

	if (wParam == VK_NEXT)
	{
		scroll++;
	}

	if (scroll > 0)
	{
		scroll = 0;
	}
}

VOID CConsole::Reset()
{
	if (commandLine->GetLength() == 0)
	{
		return;
	}

	cursorLocation = 0;
	commandLine->Clear();
}