#include "udp_echo_server.h"

park18::chapter6::udp_echo_server::udp_echo_server(int argc, char* argv[])
{
	init(argc, argv);
}

park18::chapter6::udp_echo_server::~udp_echo_server()
{
	closesocket(this->listenSock);
	WSACleanup();
}

void park18::chapter6::udp_echo_server::start()
{
	ready();

	SOCKADDR_IN clientAddress{ 0 };
	int clientAddressSize = sizeof(clientAddress);

	char message[1024] = { 0 };
	while (true)
	{
		int recvSize = ::recvfrom(this->listenSock, message, 1024, 0, reinterpret_cast<SOCKADDR*>(&clientAddress), &clientAddressSize);
		if (recvSize <= 0)
		{
			std::cout << "[E] recvSize <= 0: " << WSAGetLastError() << std::endl;
			break;
		}
		message[recvSize] = '\0';
		std::cout << "[client -> server] " << message << std::endl;

		::sendto(this->listenSock, message, recvSize, 0, reinterpret_cast<SOCKADDR*>(&clientAddress), clientAddressSize);
	}
}

void park18::chapter6::udp_echo_server::init(int argc, char* argv[])
{
	// 프로그램 인자 예외 처리
	if (argc != 2)
	{
		utils::error::error_handling("[E] udp_echo_server.init - the number of arguments is wrong");
	}
	this->port = static_cast<unsigned short>(std::stoi(argv[1]));

	// Win sock 예외 처리
	SecureZeroMemory(&this->wsaData, sizeof(this->wsaData));
	if (WSAStartup(MAKEWORD(2, 2), &this->wsaData) != 0)
	{
		utils::error::error_handling("[E] udp_echo_server.init - WSADATA initialization falied");
	}
}

void park18::chapter6::udp_echo_server::ready()
{
	this->listenSock = socket(AF_INET, SOCK_DGRAM, 0);

	sockaddr_in listenAddress = { 0 };
	listenAddress.sin_family = AF_INET;
	listenAddress.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	listenAddress.sin_port = htons(this->port);

	if (bind(this->listenSock, reinterpret_cast<sockaddr*>(&listenAddress), sizeof(listenAddress)) == SOCKET_ERROR)
	{
		utils::error::error_handling(std::format("[E] bind - code: {}", WSAGetLastError()));
	}

	//if (listen(this->listenSock, 5) == SOCKET_ERROR)
	//{
	//	utils::error::error_handling(std::format("[E] listen - code: {}", WSAGetLastError()));
	//}
}