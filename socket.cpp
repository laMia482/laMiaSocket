#include "laMiaSocket.h"

int main(int argc, char **argv)
{
	char type = 'a';
	while (!(type - 'c' == 0 || type - 's' == 0))
	{
		logStr("Select to be client or server [c/s]?");
		std::cin.getline(&type, strlen(&type), '\n');
	}
	laMiaSocket *ls = new laMiaSocket;
	ls->setPort(4999);
	if (type == 'c')
	{

		ls->setRole(laMiaSocketTypeClient);
		char ip_addr[32];
		logStr("Client selected, choose server ip:");
		std::cin.getline(ip_addr, 32, '\n');
		ls->connectServer(ip_addr);
	}
	else if (type == 's')
	{
		ls->setRole(laMiaSocketTypeServer);
		logStr("Server selected");
		ls->bindClient();
		ls->listenClient();
	}

	while (true)
	{
		if (type == 'c')
			ls->sendMessage();
		else if (type == 's')
			ls->recvMessage();
	}

}
