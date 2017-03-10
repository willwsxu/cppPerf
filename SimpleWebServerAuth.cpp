// simple http server with microsoft digest authentication

#include <winsock2.h>
#include <Ws2tcpip.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <thread>

#define MAX_CONNECTIONS 100

#include "SimpleWebServerAuth.h"

const char * szWSAErr(int iErr)
{
	return "";
}

typedef struct tag_CONNECTION
{
	SOCKET	sIn;
	char	szAddr[64];
	UINT	cbBufferSize; // size of pcBuf
	char	*pcBuf;    // pointer to character buffer, malloc-allocated
	DWORD	dwBytesOut;
	time_t	tLastUsed;
	BOOL	bConfigurable;
} CONNECTION;

CONNECTION Connection[MAX_CONNECTIONS];

static void	SetNonBlock(SOCKET sNew)
{
	u_long	nonblock = 1;
	int	iRet = 0;

	iRet = ioctlsocket(sNew, (long)FIONBIO, &nonblock);

	if (iRet)
	{
		int iErr = WSAGetLastError();
		printf("Error setting to nonblocking %d (%d)\n", sNew, iErr);
	}
}

static void	SetKeepAlive(SOCKET sNew)
{
	BOOL	bKeep = TRUE;
	int	iRet;

	iRet = (int)setsockopt(sNew, SOL_SOCKET, SO_KEEPALIVE, (const char FAR *)&bKeep, sizeof(BOOL));

	if (iRet)
	{
		printf("Error setting to keepalive %d %s (%d)", (int)sNew, szWSAErr(WSAGetLastError()), WSAGetLastError());
	}
}


static void	SetNoDelay(SOCKET sNew)
{
	BOOL	bNoDelay = FALSE;
	int	iRet = 0;

	bNoDelay = TRUE;
	iRet = setsockopt(sNew, IPPROTO_TCP, TCP_NODELAY, (char *)&bNoDelay, sizeof(BOOL));
	if (iRet)
	{
		printf("Error %s (%d) setting to TCP_NODELAY %d", szWSAErr(WSAGetLastError()), WSAGetLastError(), sNew);
	}
}

HttpListener::HttpListener()
{
	bExit = false;
	memset(Connection, 0, sizeof(Connection));
	for (int i = 0; i < MAX_CONNECTIONS; i++)
	{
		CONNECTION *pConnection = &Connection[i];

		pConnection->cbBufferSize = 65536;

		pConnection->pcBuf = (char *)malloc(pConnection->cbBufferSize);
		if (!pConnection->pcBuf)
		{
			printf("Cannot allocate raw data buffer size %d\n", (UINT)pConnection->cbBufferSize);
			return;
		}
	}
	WSADATA wsaData;
	if (WSAStartup(0x0101, &wsaData))
	{
		fprintf(stderr, "Could not initialize winsock: \n");
		return;
	}
	SOCKADDR_IN	sAddr;
	memset(&sAddr, 0, sizeof(sAddr));
	sListening = socket(AF_INET, SOCK_STREAM, 0); 
	if (sListening == INVALID_SOCKET) {
		fprintf(stderr, "Could not creating listening socket\n");
		return;
	}
	wListenPort = 8080;
	sAddr.sin_family = AF_INET;
	sAddr.sin_port = htons(wListenPort);
	sAddr.sin_addr.s_addr = INADDR_ANY;
	while (wListenPort < 9000) {
		if (SOCKET_ERROR == bind(sListening, (SOCKADDR *)&sAddr, sizeof(sAddr)))
		{
			printf("Error(%d) binding listening socket to port %d\n",	WSAGetLastError(), wListenPort);
			wListenPort++;
		}
		else
			break;
	}
	printf("HTTP using port %d\n", wListenPort);
	if (SOCKET_ERROR == listen(sListening, 0))
	{
		printf("Error (%d) in listen\n", WSAGetLastError());
		return;
	}

	SetNonBlock(sListening);
	std::thread webpoll(&HttpListener::listening, this);
	webpoll.join();
}

void HttpListener::listening()
{
	while (!bExit)
	{
		int iPort = wListenPort;
		struct	timeval t;
		FD_SET	fdRead;
		FD_SET	fdWrite;
		FD_SET	fdExcept;
		int iConnection;
		BOOL bAnyConnections = FALSE;
		int iRet;

		// new connections
		t.tv_sec = 0;
		t.tv_usec = 55000;

		FD_ZERO(&fdRead);
#pragma warning (disable : 4127)    // conditional expression is constant
		// winsock2.h defines FD_SET as a typedef not a macro for bit manipulations
		FD_SET(sListening, &fdRead);

		iRet = select(0, &fdRead, 0, 0, &t);
		if (iRet == SOCKET_ERROR)
			printf("%d error (%d) in select\n", iPort, WSAGetLastError());

		if (FD_ISSET(sListening, &fdRead))
			DoAccept();

		// existing connections
		t.tv_sec = 0;
		t.tv_usec = 500000;

		FD_ZERO(&fdRead);
		FD_ZERO(&fdWrite);
		FD_ZERO(&fdExcept);
		for (iConnection = 0; iConnection < MAX_CONNECTIONS; iConnection++)
		{
			CONNECTION *pConnection = &Connection[iConnection];
			if (pConnection->sIn)
			{
#pragma warning (disable : 4127) // FD_SET BEING USED but winsock2.h defines this as a typedef not a macro (as in unix)
				FD_SET(pConnection->sIn, &fdRead);
				FD_SET(pConnection->sIn, &fdWrite);
				FD_SET(pConnection->sIn, &fdExcept);
				bAnyConnections = TRUE;
			}
		}

		if (!bAnyConnections)
		{
			Sleep(500);
			continue;
		}
		iRet = select(0, &fdRead, &fdWrite, &fdExcept, &t);
		if (iRet == SOCKET_ERROR)
		{
			printf("%d Error (%d) in select\n", iPort, WSAGetLastError());
		}

		for (iConnection = 0; iConnection < MAX_CONNECTIONS; iConnection++)
		{
			CONNECTION *pConnection = &Connection[iConnection];
			if (pConnection->sIn)
			{
				if (FD_ISSET(pConnection->sIn, &fdExcept))
				{
					printf("HTTP socket err\n");
					shutdown(pConnection->sIn, 2);
					closesocket(pConnection->sIn);
					pConnection->sIn = 0;
				}
				else if (FD_ISSET(pConnection->sIn, &fdRead)
					&& FD_ISSET(pConnection->sIn, &fdWrite))
				{
					UINT cbRead = 0;

					pConnection->tLastUsed = time(0);

					// try to read socket
					cbRead = recv(pConnection->sIn, pConnection->pcBuf, pConnection->cbBufferSize - 1, 0);

					// if any data, send it to framer
					if ((int)cbRead > 0)
					{
						char szResponse[80000] = { 0 };
						UINT iLen = sizeof(szResponse);
						
						if (!strncmp(pConnection->pcBuf, "GET", 3))
							GetResponse(szResponse, iLen, pConnection->pcBuf + 4);
						else if (!strncmp(pConnection->pcBuf, "POST", 4))
						{
							PostResponse(szResponse, iLen, pConnection->pcBuf + 5);
						}
						if (szResponse[0])
						{
							size_t sendLen = send(pConnection->sIn, szResponse, iLen, 0);
							if (sendLen != iLen)
								printf("http response failed\n");
							else
								printf("%s", szResponse);
						}
					}

					if (cbRead == 0 || cbRead == (UINT)SOCKET_ERROR)
					{
						if (cbRead == (UINT)SOCKET_ERROR)
						{
							int iErr = WSAGetLastError();
							printf("%d Error reading (%d)\n", iPort, iErr);
						}
						else
							printf("%d socket closing gracefully\n", iPort);
						shutdown(pConnection->sIn, 2);
						closesocket(pConnection->sIn);
						pConnection->sIn = 0;
					}
				}
			}
		}
	}
}


void HttpListener::DoAccept()
{
	SOCKADDR_IN	sAddr;
	int		iAddrLen = sizeof(SOCKADDR_IN);
	CONNECTION *pConnection = 0;
	time_t tMinimum = 0x7fffffff;
	int iFoundIndex = 0;
	int i;
	memset(&sAddr, 0, sizeof(sAddr));

	// find free connection slot, or reuse most idle connection slot
	for (i = 0; i < MAX_CONNECTIONS; i++)
	{
		if (!Connection[i].sIn)
		{
			iFoundIndex = i;
			break;
		}
		else
		{
			if (Connection[i].tLastUsed < tMinimum)
			{
				tMinimum = Connection[i].tLastUsed;
				iFoundIndex = i;
			}
		}
	}
	pConnection = &Connection[iFoundIndex];
	// if already in use, close former connection
	if (pConnection->sIn)
	{
		shutdown(pConnection->sIn, 2);
		closesocket(pConnection->sIn);
		pConnection->sIn = 0;
	}

	pConnection->sIn = accept(sListening, (SOCKADDR *)&sAddr, &iAddrLen);

	if (pConnection->sIn == INVALID_SOCKET)
	{
		int	iErr = WSAGetLastError();
		printf("Error (%d) in accept on socket %d\n", iErr, sListening);
		pConnection->sIn = 0;
		return;
	}

	pConnection->tLastUsed = time(0);

	{
		char pBuf[20] = "";
		int	iVal = 1;

		if (SOCKET_ERROR == setsockopt(pConnection->sIn, SOL_SOCKET, SO_DONTLINGER,
			(char *)&iVal, sizeof(int)))
			printf("Err (%d) setting SO_DONTLINGER\n", WSAGetLastError());

		SetKeepAlive(pConnection->sIn);
		SetNoDelay(pConnection->sIn);

		inet_ntop(AF_INET, &sAddr.sin_addr, pBuf, sizeof(pBuf));

		printf("HTTP connection accepted from %s\n", pBuf);
		strncpy_s(pConnection->szAddr, pBuf, _TRUNCATE);
	}

}
static long counter = 0;
void HttpListener::GetResponse(char * szBuf, UINT &iLen, const char * data)
{
	counter++;
	sprintf_s(szBuf, iLen, "HTTP/1.1 200 OK\r\n"
		"Server: minimal http server\r\n"
		"Cache-control: private\r\n"
		"Index: no-cache\r\n"
		"Connection: Keep-Alive\r\n"
		"Content-Length: %d\r\n"
//		"Set - Cookie: fake = fake_value\n"
//		"Www - Authenticate : Digest realm = \"me@ezesoft.com\"\n"
		"Content-Type: text/html\r\n\r\n", 200
		);

	strcat_s(szBuf, iLen, "<HTML><HEAD><TITLE>HELLO WORD</TITLE></HEAD><BODY>"
		"<h2>TEST Authentication</h2>");
//	sprintf_s(szBuf+strlen(szBuf), iLen, "<h3>Counter %lu</h3>", counter);
//	strcat_s(szBuf, iLen, "<A HREF=\"index.htm\">auto-refresh this web page</A>");
	strcat_s(szBuf, iLen, "\n<PRE>");

	strcat_s(szBuf, iLen, "</PRE>");
//	strcat_s(szBuf, iLen, "<meta http-equiv=\"Refresh\" content=\"10; URL=index.htm\">");
	strcat_s(szBuf, iLen, "<FORM method=""POST"">"
		"<INPUT type=""submit"" name=""Reconfigure"" value=""Reconfigure""></FORM></BODY></HTML>\r\n");

	iLen = static_cast<UINT>(strlen(szBuf));
}

void HttpListener::PostResponse(char * szResp, UINT &iLen, const char * data)
{

}