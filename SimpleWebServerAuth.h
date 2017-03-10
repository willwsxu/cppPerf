#pragma once

class HttpListener
{
	SOCKET sListening;
	short	wListenPort;
	volatile bool bExit;

	void listening();
	void DoAccept();
	void GetResponse(char * szResp, UINT &iLen, const char * data);
	void PostResponse(char * szResp, UINT &iLen, const char * data);

public:
	HttpListener();
	void close() {
		bExit = true;
	}
};