#include "laMiaSocket.h"

int main(int argc, char **argv)
{

#if 1
	laMiaSocket *ls = new laMiaSocket(laMiaSocketTypeClient, 4999);
	
	// ls->listenClient();
	ls->connectServer();
	
	while (true)
	{
		ls->sendMessage();
		ls->recvMessage();
	}

#endif

}