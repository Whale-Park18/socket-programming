#include "pch.h"
#include "udp_sock.h"

#include <string>
#include <format>

void park18::chapter6::EchoClient(int argc, char* argv[])
{
	if (argc != 3)
	{
		utils::error::ErrorHandling(std::format("[E] Need infomation IP, Port: {}", ::WSAGetLastError()));
	}

	std::cout << "Client Start EchoClient" << std::endl;

	WSADATA wsaData = { 0 };
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		utils::error::ErrorHandling(std::format("[E] WSAStartup(): {}", ::WSAGetLastError()));
	}

	SOCKET clientSock = ::socket(PF_INET, SOCK_DGRAM, 0);
	if (clientSock == INVALID_SOCKET)
	{
		utils::error::ErrorHandling(std::format("[E] socket(): {}", ::WSAGetLastError()));
	}

	SOCKADDR_IN serverAddress{ 0 };
	int serverAddressSize = sizeof(serverAddress);

	char message[1024] = { 0 };
	while (true)
	{
		std::cout << "[client -> server] ";
		std::cin.getline(message, 1024);
		::sendto(clientSock, message, std::strlen(message), 0, reinterpret_cast<SOCKADDR*>(&serverAddress), serverAddressSize);

		int recvSize = ::recvfrom(clientSock, message, 1024, 0, reinterpret_cast<SOCKADDR*>(&serverAddress), &serverAddressSize);
		std::cout << "[server -> client] " << message << std::endl;
	}

	closesocket(clientSock);
	WSACleanup();
}

void park18::chapter6::ConnectEchoClient(int argc, char* argv[])
{
	if (argc != 3)
	{
		utils::error::ErrorHandling(std::format("[E] Need infomation IP, Port: {}", ::WSAGetLastError()));
	}

	std::cout << "Client Start ConnectEchoClient" << std::endl;

	WSADATA wsaData = { 0 };
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		utils::error::ErrorHandling(std::format("[E] WSAStartup(): {}", ::WSAGetLastError()));
	}

	SOCKET clientSock = socket(PF_INET, SOCK_DGRAM, 0);
	if (clientSock == INVALID_SOCKET)
	{
		utils::error::ErrorHandling(std::format("[E] socket(): {}", ::WSAGetLastError()));
	}

	SOCKADDR_IN serverAddress = { 0 };
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.S_un.S_addr = inet_addr(argv[1]);
	serverAddress.sin_port = htons(atoi(argv[2]));
	int serverAddressSize = sizeof(serverAddress);

	if (connect(clientSock, reinterpret_cast<SOCKADDR*>(&serverAddress), sizeof(serverAddress)) == SOCKET_ERROR)
	{
		utils::error::ErrorHandling(std::format("[E] socket(): {}", ::WSAGetLastError()));
	}

	char message[1024] = { 0 };
	while (true)
	{
		std::cout << "[client -> server] ";
		std::cin.getline(message, 1024);
		sendto(clientSock, message, std::strlen(message), 0, reinterpret_cast<SOCKADDR*>(&serverAddress), sizeof(serverAddress));

		int recvSize = recvfrom(clientSock, message, 1024, 0, reinterpret_cast<SOCKADDR*>(&serverAddress), &serverAddressSize);
		std::cout << "[server -> client] " << message << std::endl;
	}

	closesocket(clientSock);
	WSACleanup();
}
