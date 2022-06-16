#include "CEventManager.h"

unsigned __stdcall ClientReceiveThread(void* c)
{
	CEventManager* ec = (CEventManager*)c;

	ec->globalObjects->errorLog->WriteError("ClientReceiveThread Starting\n");

	while (ec->isRunning)
	{
		if (ec->client)
		{
			if (ec->client->connectionState == CClient::CS_NOTCONNECTED)
			{
				ec->isRunning = FALSE;
			}
			else
			{
				ec->client->ClientReceive();

				if (ec->client->connectionState == CClient::CS_NOTCONNECTED)
				{
					ec->isRunning = FALSE;
				}
				else if (ec->client->packet->type)
				{
					ec->ProcessEvent(ec->client->packet);
				}
			}
		}
	}

	ec->globalObjects->errorLog->WriteError("ClientReceiveThread Ending\n");

	_endthreadex(0);

	return 0;
}

CEventManager::CEventManager()
{
	memset(this, 0x00, sizeof(CEventManager));
}

CEventManager::CEventManager(CGlobalObjects* g)
{
	memset(this, 0x00, sizeof(CEventManager));

	globalObjects = g;

	chatText = new CString(80);
}

CEventManager::~CEventManager()
{
	delete chatText;
	
	delete client;
	
	delete server;
}

void CEventManager::SendEvent(CPacket* p)
{
	if ((client == nullptr) || (p->audience == CPacket::PT_LOCAL))
	{
		CEventManager::ProcessEvent(p);
		
		return;
	}

	client->SendEvent(p);
}

void CEventManager::ProcessEvent(CPacket* p)
{
	switch (p->type)
	{
		case CPacket::PT_EXIT:
		{
			CEventManager::Exit(p);

			break;
		}

		case CPacket::PT_WINDOWMODE:
		{
			if (globalObjects->videoDevice && globalObjects->videoDevice->tearingSupport)
			{
				globalObjects->videoDevice->ToggleFullscreenWindow();
			}

			break;
		}

		case CPacket::PT_STARTSERVER:
		{
			CEventManager::StartServer();

			break;
		}

		case CPacket::PT_STOPSERVER:
		{
			if (!server)
			{
				globalObjects->errorLog->WriteError("CEventManager::ProcessEvent:Server Not Running\n");

				break;
			}

			globalObjects->errorLog->WriteError("CEventManager::ProcessEvent:Stopping Server\n");

			server->Shutdown();

			break;
		}

		case CPacket::PT_CONNECT:
		{
			CEventManager::ClientConnect(p);

			break;
		}

		case CPacket::PT_PONG:
		{
			globalObjects->errorLog->WriteError("CEventManager::ProcessEvent:PONG\n");

			SIZE_T startTime;
			memcpy(&startTime, p->data, p->length);

			SIZE_T currTime = (SIZE_T)GetTickCount64();

			pingRate = currTime - startTime;

			//console->AddEventMessage("Pong %I64d %I64d", currTime, pingRate);

			break;
		}

		case CPacket::PT_DISCONNECT:
		{
			if (client)
			{
				if (client->connectionState != CClient::CS_CONNECTED)
				{
					globalObjects->errorLog->WriteError("CEventManager::ProcessEvent:Not Connected\n");

					break;
				}
			}
			else
			{
				break;
			}

			globalObjects->errorLog->WriteError("CEventManager::ProcessEvent:Disconnect:%i\n", p->fromClient);

			isRunning = FALSE;

			rv = WaitForSingleObjectEx(hThread, 2000, FALSE);

			CloseHandle(hThread);

			client->Disconnect();


			packet = new CPacket(p->fromClient, p->toClient, CPacket::PT_LOCAL, CPacket::PT_PLAYEREXIT, CPacket::PT_EMPTY, 1);

			CEventManager::SendEvent(packet);

			delete packet;

			break;
		}

		case CPacket::PT_ACCEPTED:
		{
			globalObjects->errorLog->WriteError("CEventManager::ProcessEvent:Accepted:%i\n", p->fromClient);

			client->clientNbr = p->toClient;

			client->connectionState = CClient::CS_CONNECTED;

			break;
		}

		case CPacket::PT_CHAT:
		{
			chatText->Clear();

			sprintf_s(chatText->GetText(), 80, "%i %s", p->fromClient, p->data);

			globalObjects->errorLog->WriteError("%s\n", chatText->GetText());

			//console->AddEventMessage(chatText->GetText());

			break;
		}

		case CPacket::PT_CONSOLEINPUT:
		{
			CEventManager::ConsoleInput(p);

			break;
		}

		case CPacket::PT_TOGGLECONSOLE:
		{
			if (consoleVisible)
			{
				consoleVisible = FALSE;
			}
			else
			{
				consoleVisible = TRUE;
			}

			break;
		}
	}
}

void CEventManager::ConsoleInput(CPacket* p)
{
	/*
	if (p->data[0] == VK_RETURN)
	{
		if (console->commandLine->GetText()[0] != '/')
		{
			CEventManager::SendGlobalChat();

			console->Reset();

			return;
		}
		else
		{
			CEventManager::CommandFromConsole();

			console->Reset();

			return;
		}
	}

	console->AddCHAR((char)* p->data);
	*/
}

void CEventManager::CommandFromConsole()
{
	/*
	if (strcmp(console->commandLine->GetText(), "/exit") == 0)
	{
		CPacket* packet = new CPacket(0, 0, CPacket::PT_LOCAL, CPacket::PT_EXIT, CPacket::PT_EXIT, 1);

		CEventManager::SendEvent(packet);

		delete packet;

		return;
	}

	if (strcmp(console->commandLine->GetText(), "/ping") == 0)
	{
		if (client == nullptr)
		{
			return;
		}

		SIZE_T startTime = (SIZE_T)GetTickCount64();

		console->AddEventMessage("Ping %I64d", startTime);

		CPacket* packet = new CPacket(client->clientNbr, client->clientNbr, CPacket::PT_SINGLE, CPacket::PT_PING, (BYTE*)&startTime, sizeof(SIZE_T));

		CEventManager::SendEvent(packet);

		delete packet;

		return;
	}

	if (strcmp(console->commandLine->GetText(), "/wire") == 0)
	{
		if (console->variables->drawWire)
		{
			console->variables->drawWire = FALSE;
		}
		else
		{
			console->variables->drawWire = TRUE;
		}

		console->AddEventMessage("drawWire %x", console->variables->drawWire);

		return;
	}

	UINT i = 0;

	char c[32] = {};

	while ((console->commandLine->GetText()[i] != ' ') && (console->commandLine->GetText()[i] != '\0'))
	{
		memcpy(&c[i], &console->commandLine->GetText()[i], 1);

		i++;
	}

	i++;

	UINT ii = 0;

	char v[32] = {};

	while (console->commandLine->GetText()[i] != '\0')
	{
		memcpy(&v[ii], &console->commandLine->GetText()[i], 1);

		ii++;
		i++;
	}

	if (strcmp(c, "/fog") == 0)
	{
		console->variables->fog = (float)atof(v);

		console->AddEventMessage("fog %f", console->variables->fog);

		return;
	}

	if (strcmp(c, "/tod") == 0)
	{
		console->variables->timeOfDay = (float)atof(v);

		console->AddEventMessage("timeOfDay %f", console->variables->timeOfDay);

		return;
	}

	if (strcmp(c, "/connect") == 0)
	{
		CPacket* packet = new CPacket(0, 0, CPacket::PT_LOCAL, CPacket::PT_CONNECT, v, (UINT)strlen(v));

		CEventManager::SendEvent(packet);

		delete packet;

		return;
	}

	if (strcmp(c, "/disconnect") == 0)
	{
		CPacket* packet = new CPacket(0, 0, CPacket::PT_LOCAL, CPacket::PT_DISCONNECT);

		CEventManager::SendEvent(packet);

		delete packet;

		return;
	}

	console->AddEventMessage("Unknown Command", c);
	*/
}

void CEventManager::ClientConnect(CPacket* p)
{
	if (client)
	{
		if (client->connectionState == CClient::CS_CONNECTED)
		{
			globalObjects->errorLog->WriteError("CEventManager::ProcessEvent:Already Connected:%i\n", p->fromClient);

			return;
		}

		delete client;
	}

	globalObjects->errorLog->WriteError("CEventManager::ProcessEvent:Connect\n");

	client = new CClient(globalObjects, p);

	isRunning = TRUE;

	hThread = (HANDLE)_beginthreadex(NULL, 0, &ClientReceiveThread, (VOID*)this, 0, &receiveThreadId);
}

void CEventManager::Exit(CPacket* p)
{
	isExiting = TRUE;

	globalObjects->errorLog->WriteError("CEventManager::Exit:Exiting\n");

	if (client)
	{
		if (client->connectionState == CClient::CS_CONNECTED)
		{
			isRunning = FALSE;

			rv = WaitForSingleObjectEx(hThread, 2000, FALSE);

			CloseHandle(hThread);

			client->Disconnect();
		}
	}

	if (server)
	{
		if (server->acceptRunning)
		{
			server->Shutdown();
		}
	}

	PostMessage(globalObjects->window->hWnd, WM_DESTROY, 0, 0);
}

void CEventManager::StartServer()
{
	if (server)
	{
		if (server->acceptRunning)
		{
			globalObjects->errorLog->WriteError("CEventManager::ProcessEvent:Server Already Running\n");

			return;
		}

		delete server;
	}

	globalObjects->errorLog->WriteError("CEventManager::ProcessEvent:Starting Server\n");

	server = new CServer(globalObjects, "26105");
}
