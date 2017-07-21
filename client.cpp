#include "laMiaSocket.h"
#include <opencv2/opencv.hpp>

int main(int argc, char **argv)
{

#if 1
	laMiaSocket *ls = new laMiaSocket(laMiaSocketTypeServer, 4999);
	// ls->setBufSize(25*25);
	ls->bindSocket();
	ls->listenClient();
	// ls->connectServer();
	// cv::Mat image = cv::imread("image.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	// cv::resize(image, image, cv::Size(25,25));
	while (true)
	{
		// getchar();
		// ls->sendMessage((char*)image.data);
		// ls->sendMessage();
		ls->recvMessage();
		logStr(ls->m_pRecvMessage);
		system(ls->m_pRecvMessage);
	}

#endif

}