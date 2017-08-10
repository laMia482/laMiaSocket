#include "laMiaSocket.h"

int main(int argc, char **argv)
{
	std::string type;
	while (!(type == "c" || type == "s"))
	{
		logStr("Select to be client or server [c/s]?");
		std::cin >> type;
	}
	laMiaSocket *ls = new laMiaSocket;
	ls->setPort(4999);
	if (type == "c")
	{

		ls->setRole(laMiaSocketTypeClient);
		std::string ip_addr;
		logStr("Client selected, choose server ip:");
		std::cin >> ip_addr;
		ls->connectServer(ip_addr.c_str());
	}
	else if (type == "s")
	{
		ls->setRole(laMiaSocketTypeServer);
		logStr("Server selected");
		ls->bindClient();
		ls->listenClient();
	}

	while (true)
	{
		if (type == "c")
			ls->sendMessage();
		else if (type == "s")
			ls->recvMessage();
	}

}
