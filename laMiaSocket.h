#ifndef LAMIA_SOCKET_H
#define LAMIA_SOCKET_H

#ifndef Linux
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
typedef int socklen_t;
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
#include <stdlib.h>
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

typedef size_t laMiaSocketSize;

class laMiaSocket
{
public:
	laMiaSocket(void)
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
	}
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
	}

	void setPort(const int &port)
	{
		m_Port = port;
		m_AddrServ.sin_family = AF_INET;
		m_AddrServ.sin_port = htons(m_Port);
	}

	void setBufSize(const laMiaSocketSize &size)
	{
		m_BufSize = size;
		delete[] m_pSendMessage;
		delete[] m_pRecvMessage;
		m_pSendMessage = new char[m_BufSize];
		m_pRecvMessage = new char[m_BufSize];
	}

	int bindClient(const char *client_ip = NULL)
	{
		if (m_laMiaSocketType == laMiaSocketTypeServer)
		{
			if(client_ip)
				m_AddrServ.sin_addr.s_addr = inet_addr(client_ip);
			else
				m_AddrServ.sin_addr.s_addr = INADDR_ANY;
		}
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
		// bindSocket();
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

	void connectServer(const char *server_ip = NULL)
	{
		if(m_laMiaSocketType == laMiaSocketTypeClient)
		{
			if(server_ip)
				m_AddrServ.sin_addr.s_addr = inet_addr(server_ip);
			else
				m_AddrServ.sin_addr.s_addr = inet_addr("127.0.0.1");
		}
		int retVal = connect(m_Server, (struct sockaddr *)&m_AddrServ, sizeof(struct sockaddr_in));
		if (0 != retVal)
		{
			logStr("connect failed!~");
			closesocket(m_Server);
			 WSACleanup(); 
			return;
		}
		else
		{
			logStr("connect success!~");
		}
	}

	void sendMessage(const char *buf = NULL)
	{
		if(m_laMiaSocketType == laMiaSocketTypeServer)
			logStr("SEND DATA TO CLIENT:");
		else if(m_laMiaSocketType == laMiaSocketTypeClient)
			logStr("SEND DATA TO SERVER:");
		memset(m_pSendMessage, 0, m_BufSize);
		if(buf != NULL)
			memcpy(m_pSendMessage, buf, m_BufSize);
		else
			std::cin.getline(m_pSendMessage, m_BufSize, '\n');
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
				listenClient();
			}
			else
			{
				if (buf != NULL)
					memcpy(buf, m_pRecvMessage, m_BufSize);
				logStr("RECV DATA FROM CLIENT:");
				fprintf(stderr, "------------ %s -----------\n", m_pRecvMessage);
			}
		}
		else if(m_laMiaSocketType == laMiaSocketTypeClient)
		{
			memset(m_pRecvMessage, 0, m_BufSize);
			recv(m_Server, m_pRecvMessage, m_BufSize, 0);
			logStr("RECV DATA FROM SERVER: ");
			fprintf(stderr, "------------ %s -----------\n", m_pRecvMessage);
		}
	}

// private:
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

#endif