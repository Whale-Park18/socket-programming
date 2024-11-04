#include "oob_client.h"
#include <WS2tcpip.h>

park18::chapter13::oob_client::oob_client(int argc, char* argv[])
{
	init(argc, argv);
}

void park18::chapter13::oob_client::start()
{
	ready();

	send(this->connectSock, "123", 3, 0);
	Sleep(100);

	send(this->connectSock, "4", 1, MSG_OOB);
	Sleep(100);

	send(this->connectSock, "567", 3, 0);
	Sleep(100);

	send(this->connectSock, "890", 3, MSG_OOB);
	Sleep(100);

	system("pause");
}

void park18::chapter13::oob_client::init(int argc, char* argv[])
{
	if (argc != 3)
	{
		utils::error::error_handling(std::format("Usage: {} <IP> <port>", argv[0]));
	}
	this->host = argv[1];
	this->port = std::atoi(argv[2]);

	this->connectSock = INVALID_SOCKET;
}

void park18::chapter13::oob_client::ready()
{
	this->connectSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (this->connectSock == INVALID_SOCKET)
	{
		utils::error::error_handling(std::format("[E] socket - code: {}", WSAGetLastError()));
	}

	sockaddr_in connectAddress = { 0 };
	connectAddress.sin_family = PF_INET;
	inet_pton(PF_INET, this->host.c_str(), &connectAddress.sin_addr);
	connectAddress.sin_port = htons(this->port);

	if (connect(this->connectSock, reinterpret_cast<sockaddr*>(&connectAddress), sizeof(connectAddress)) == SOCKET_ERROR)
	{
		utils::error::error_handling(std::format("[E] bind - connect: {}", WSAGetLastError()));
	}
}