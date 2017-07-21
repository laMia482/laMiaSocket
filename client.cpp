// #define _WINSOCK_DEPRECATED_NO_WARNINGS

//#include <WinSock2.h>
//#include <stdio.h>
//#include <iostream>
#include "laMiaSocket.h"

//#pragma comment(lib, "ws2_32.lib")  


int main(int argc, char **argv)
{

#if 1
	laMiaSocket *ls = new laMiaSocket(laMiaSocketTypeClient, 4999);
	// logStr("A");
	// ls->bindSocket();
	// logStr("B");
	// ls->listenClient();
	ls->connectServer();
	// logStr("C");
	while (true)
	{
		
		// logStr("D");
		//char str[50];
		//std::cin >> str;
		ls->sendMessage();
		ls->recvMessage();
	}

#endif


#if 0
	const int BUF_SIZE = 64;
	WSADATA			wsd;
	SOCKET			sServer;
	SOCKET			sClient;
	SOCKADDR_IN		addrServ;
	char			buf[BUF_SIZE];
	char			sendBuf[BUF_SIZE];
	int				retVal;

	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		logStr("WSAStartup failed!~");
		return 1;
	}

	sServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sServer == INVALID_SOCKET)
	{
		logStr("socket failed!~");
		WSACleanup();
		return -1;
	}

	addrServ.sin_family = AF_INET;
	addrServ.sin_port = htons(4999);
	addrServ.sin_addr.s_addr = INADDR_ANY;
	retVal = bind(sServer, (LPSOCKADDR)&addrServ, sizeof(SOCKADDR_IN));
	if (retVal == SOCKET_ERROR)
	{
		logStr("bind failed!~");
		closesocket(sServer);
		WSACleanup();
		return -2;
	}

	retVal = listen(sServer, 1);
	if (retVal == SOCKET_ERROR)
	{
		logStr("listen failed!~");
		closesocket(sServer);
		WSACleanup();
		return -3;
	}

	sockaddr_in addrClient;
	int addrClientLen = sizeof(addrClient);
	sClient = accept(sServer, (sockaddr FAR*)&addrClient, &addrClientLen);
	if (sClient == INVALID_SOCKET)
	{
		logStr("accept failed!~");
		closesocket(sServer);
		WSACleanup();
		return -4;
	}

	while (true)
	{
		ZeroMemory(buf, BUF_SIZE);
		retVal = recv(sClient, buf, BUF_SIZE, 0);
		if (retVal == SOCKET_ERROR)
		{
			logStr("recv failed!~");
			closesocket(sServer);
			closesocket(sClient);
			WSACleanup();
			return -5;
		}

		if (buf[0] == '\0')
		{
			logStr("Waiting For Client");
			break;
		}
		logStr("DATA FROM CLIENT:");
		fprintf(stderr, "------------ %s -----------\n", buf);

		logStr("SEND DATA TO CLIENT:");
		memset(sendBuf, BUF_SIZE, 0);
		std::cin >> sendBuf;
		send(sClient, sendBuf, strlen(sendBuf), 0);
	}

	closesocket(sServer);
	closesocket(sClient);
	WSACleanup();

#endif
	return 0;
}