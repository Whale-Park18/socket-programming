#pragma once
#include "global.h"

namespace park18::chapter9
{
	WSADATA wsaData = { 0 };

	void init()
	{
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		{
			utils::error::error_handling(std::format("[E] WSAStartup - {}", WSAGetLastError()));
		}
	}

	void cleanup()
	{
		WSACleanup();
	}

	void print_accept_info(const SOCKADDR_IN& acceptAddr)
	{
		char ip[1024] = { 0 };
		strcpy_s(ip, inet_ntoa(acceptAddr.sin_addr));

		types::ushort port = ntohs(acceptAddr.sin_port);

		std::cout << std::format("AcceptInfo IP: {}, Port: {}", ip, port) << std::endl;
	}

	void echo_server(int argc, char* argv[])
	{
		init();
		std::cout << "Start Server" << std::endl;

		SOCKET listenSock = socket(PF_INET, SOCK_STREAM, 0);
		if (listenSock == INVALID_SOCKET)
		{
			utils::error::error_handling(std::format("[E] listenSock - {}", WSAGetLastError()));
		}

		bool option = true;
		int optionSize = sizeof(option);
		if (setsockopt(listenSock, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char*>(&option), optionSize) != 0)
		{
			utils::error::error_handling(std::format("[E] setsockopt - {}", WSAGetLastError()));
		}

		SOCKADDR_IN listenAddr = { 0 };
		listenAddr.sin_family = AF_INET;
		listenAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
		listenAddr.sin_port = htons(atoi(argv[1]));

		if (bind(listenSock, reinterpret_cast<SOCKADDR*>(&listenAddr), sizeof(listenAddr)) == SOCKET_ERROR)
		{
			utils::error::error_handling(std::format("[E] bind - {}", WSAGetLastError()));
		}

		listen(listenSock, 5);

		SOCKADDR_IN acceptAddr = { 0 };
		int acceptAddrSize = sizeof(acceptAddr);
		SOCKET acceptSock = accept(listenSock, reinterpret_cast<SOCKADDR*>(&acceptAddr), &acceptAddrSize);

		print_accept_info(acceptAddr);

		int recvSize = 0;
		char buf[1024] = { 0 };
		while ((recvSize = recv(acceptSock, buf, 1024, 0)) != SOCKET_ERROR)
		{
			buf[recvSize] = '\0';
			std::cout << "[client -> server] " << buf << std::endl;
			send(acceptSock, buf, recvSize, 0);
		}

		std::cout << "Server End" << std::endl;

		closesocket(acceptSock);
		closesocket(listenSock);
		cleanup();
	}
}