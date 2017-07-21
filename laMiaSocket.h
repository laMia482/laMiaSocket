#ifndef Linux
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#else
#define SOCKET_ERROR -1
#define INVALID_SOCKET -1
typedef int SOCKET;
// tydedef sockaddr_in SOCKADDR_IN;
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#endif

#include <stdio.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")  

void logStr(const char *str)
{
	fprintf(stderr, "------------ %s -----------\n", str);
}

typedef enum
{
	laMiaSocketTypeServer,
	laMiaSocketTypeClient
}laMiaSocketType, laMiaSocketType_t;

typedef unsigned int laMiaSocketSize;

class laMiaSocket
{
public:
	laMiaSocket(const laMiaSocketType &type, const int &port)
	{
		m_BufSize = 256;
		m_pSendMessage = new char[m_BufSize];
		m_pRecvMessage = new char[m_BufSize];
	#ifndef Linux
		if (WSAStartup(MAKEWORD(2, 2), &m_WSD) != 0)
		{
			logStr("WSAStartup failed!~");
			exit(1);
		}
	#endif
		setPort(port);
		setRole(type);
	}

	virtual ~laMiaSocket(void)
	{
		delete[] m_pSendMessage;
		delete[] m_pRecvMessage;
		closesocket(m_Server);
		closesocket(m_Client);
		WSACleanup();
	}

	void setRole(const laMiaSocketType &type)
	{
		m_laMiaSocketType = type;
		m_Server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (m_Server == INVALID_SOCKET)
		{
			logStr("socket failed!~");
			WSACleanup();
		}
		m_AddrServ.sin_family = AF_INET;
		m_AddrServ.sin_port = htons(m_Port);
		if (type == laMiaSocketTypeServer)
		{
			m_AddrServ.sin_addr.s_addr = INADDR_ANY;
		}
		else if(type == laMiaSocketTypeClient)
		{
			m_AddrServ.sin_addr.s_addr = inet_addr("192.168.11.53");
		}
	}

	void setPort(const int &port)
	{
		m_Port = port;
	}

	void setBufSize(const laMiaSocketSize &size)
	{
		m_BufSize = size;
	}

	int bindSocket()
	{
		int retVal = bind(m_Server, (sockaddr*)&m_AddrServ, sizeof(sockaddr_in));
		if (retVal == SOCKET_ERROR)
		{
			logStr("bind failed!~");
			closesocket(m_Server);
			WSACleanup();
			return -2;
		}
		return 0;
	}

	void listenClient(void)
	{
		bindSocket();
		int retVal = listen(m_Server, 1);
		if (retVal == SOCKET_ERROR)
		{
			logStr("listen failed!~");
			closesocket(m_Server);
			WSACleanup();
			exit(-1);
		}
		sockaddr_in addrClient;
		int addrClientLen = sizeof(addrClient);
		m_Client = accept(m_Server, (sockaddr*)&addrClient, (socklen_t*)&addrClientLen);
		if (m_Client == INVALID_SOCKET)
		{
			logStr("accept failed!~");
			closesocket(m_Server);
			WSACleanup();
		}
		else
		{
			logStr("connect client success!~");
		}
	}

	void connectServer(void)
	{
		int retVal = connect(m_Server, (struct sockaddr *)&m_AddrServ, sizeof(struct sockaddr_in));
		if (0 != retVal)
		{
			logStr("connect failed!~");
			closesocket(m_Server);
			// WSACleanup(); 
			return;
		}
		else
		{
			logStr("connect success!~");
		}
	}

	void sendMessage(char *buf = NULL)
	{
		if(m_laMiaSocketType == laMiaSocketTypeServer)
			logStr("SEND DATA TO CLIENT:");
		else if(m_laMiaSocketType == laMiaSocketTypeClient)
			logStr("SEND DATA TO SERVER:");
		memset(m_pSendMessage, m_BufSize, 0);
		memset(m_pSendMessage, 0, m_BufSize);
		if(buf != NULL)
			memcpy(m_pSendMessage, buf, strlen(buf));
		else
			std::cin >> m_pSendMessage;
		int retVal;
		if(m_laMiaSocketType == laMiaSocketTypeServer)
			retVal = send(m_Client, m_pSendMessage, m_BufSize, 0);
		else if(m_laMiaSocketType == laMiaSocketTypeClient)
			retVal = send(m_Server, m_pSendMessage, m_BufSize, 0);
		if(SOCKET_ERROR == retVal)
		{
			logStr("send failed!~");
			closesocket(m_Server); 
			WSACleanup(); 
			exit(-4);
		}
	}

	void recvMessage(char *buf = NULL)
	{

		if (m_laMiaSocketType == laMiaSocketTypeServer)
		{
			memset(m_pRecvMessage, 0, m_BufSize);
			int retVal = recv(m_Client, m_pRecvMessage, m_BufSize, 0);
			if (retVal == SOCKET_ERROR)
			{
				logStr("recv failed!~");
				closesocket(m_Server);
				closesocket(m_Client);
				WSACleanup();
				exit(-2);
			}

			if (m_pRecvMessage[0] == '\0')
			{
				logStr("Waiting For Client");
				return;
			}
			if (buf != NULL)
				memcpy(buf, m_pRecvMessage, m_BufSize);
			logStr("RECV DATA FROM CLIENT:");
			fprintf(stderr, "------------ %s -----------\n", m_pRecvMessage);
		}
		else if(m_laMiaSocketType == laMiaSocketTypeClient)
		{
			memset(m_pRecvMessage, 0, m_BufSize);
			recv(m_Server, m_pRecvMessage, m_BufSize, 0);
			logStr("RECV DATA FROM SERVER: ");
			fprintf(stderr, "------------ %s -----------\n", m_pRecvMessage);
		}
	}

private:
#ifdef Linux
	void closesocket(SOCKET skt)
	{
		close(skt);
	}
	void WSACleanup(void)
	{
		logStr("WSACleanup!~");
	}
#else
	WSADATA m_WSD;
#endif // Linux

	int m_BufSize;
	SOCKET m_Server;
	SOCKET m_Client;
	char *m_pRecvMessage;
	char *m_pSendMessage;
	sockaddr_in m_AddrServ;
	int m_Port;
	laMiaSocketType m_laMiaSocketType;
};
