#include "pch.h"
#include "udp_sock.h"

#include <format>

void park18::chapter6::EchoServer(int argc, char* argv[])
{
	if (argc != 2)
	{
		utils::error::ErrorHandling(std::format("[E] Need infomation, Port: {}", ::WSAGetLastError()));
	}

	WSADATA wsaData = { 0 };
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		utils::error::ErrorHandling(std::format("[E] WSAStartup(): {}", ::WSAGetLastError()));
	}

	std::cout << "[System] Server Start EchoServer" << std::endl;

	SOCKET serverSock = ::socket(PF_INET, SOCK_DGRAM, 0);
	if (serverSock == INVALID_SOCKET)
	{
		utils::error::ErrorHandling(std::format("[E] socket(): {}", ::WSAGetLastError()));
	}

	SOCKADDR_IN serverAddress{ 0 };
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.S_un.S_addr = ::htonl(INADDR_ANY);
	serverAddress.sin_port = ::htons(::atoi(argv[1]));
	if (::bind(serverSock, reinterpret_cast<SOCKADDR*>(&serverAddress), sizeof(serverAddress)) == SOCKET_ERROR)
	{
		utils::error::ErrorHandling(std::format("[E] bind(): {}", ::WSAGetLastError()));
	}

	SOCKADDR_IN clientAddress{ 0 };
	int clientAddressSize = sizeof(clientAddress);

	char message[1024] = { 0 };
	while (true)
	{
		int recvSize = ::recvfrom(serverSock, message, 1024, 0, reinterpret_cast<SOCKADDR*>(&clientAddress), &clientAddressSize);
		if (recvSize <= 0)
		{
			std::cout << "[E] recvSize <= 0: " << WSAGetLastError() << std::endl;
			break;
		}
		message[recvSize] = '\0';
		std::cout << "[client -> server] " << message << std::endl;

		::sendto(serverSock, message, recvSize, 0, reinterpret_cast<SOCKADDR*>(&clientAddress), clientAddressSize);
	}

	closesocket(serverSock);
	WSACleanup();
}

void park18::chapter6::ConnectEchoServer(int argc, char* argv[])
{
	if (argc != 2)
	{
		utils::error::ErrorHandling(std::format("[E] Need infomation, Port: {}", ::WSAGetLastError()));
	}

	std::cout << "[System] Server Start ConnectEchoServer" << std::endl;

	WSADATA wsaData = { 0 };
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		utils::error::ErrorHandling(std::format("[E] WSAStartup(): {}", ::WSAGetLastError()));
	}

	SOCKET serverSock = socket(PF_INET, SOCK_DGRAM, 0);
	if (serverSock == INVALID_SOCKET)
	{
		utils::error::ErrorHandling(std::format("[E] socket(): {}", ::WSAGetLastError()));
	}

	SOCKADDR_IN serverAddress = { 0 };
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	serverAddress.sin_port = htons(atoi(argv[1]));

	if (bind(serverSock, reinterpret_cast<SOCKADDR*>(&serverAddress), sizeof(serverAddress)) == SOCKET_ERROR)
	{
		utils::error::ErrorHandling(std::format("[E] bind(): {}", ::WSAGetLastError()));
	}

	SOCKADDR_IN clientAddress = { 0 };
	int clientAddressSize = sizeof(clientAddress);

	char message[1024] = { 0 };
	while (true)
	{
		int recvSize = recvfrom(serverSock, message, 1024, 0, reinterpret_cast<SOCKADDR*>(&clientAddress), &clientAddressSize);
		if (recvSize <= 0)
		{
			std::cout << "[E] recvSize <= 0: " << WSAGetLastError() << std::endl;
			break;
		}
		message[recvSize] = '\0';
		std::cout << "[client -> server] " << message << std::endl;

		sendto(serverSock, message, recvSize, 0, reinterpret_cast<SOCKADDR*>(&clientAddress), sizeof(clientAddress));
	}

	closesocket(serverSock);
	WSACleanup();
}
