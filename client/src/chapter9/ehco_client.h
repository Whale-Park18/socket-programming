#pragma once
#include "pch.h"

#include <format>

namespace park18::chapter9
{
	WSADATA wsaData = { 0 };

	void Init()
	{
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		{
			utils::error::ErrorHandling(std::format("[E] WSAStartup - {}", WSAGetLastError()));
		}
	}

	void Cleanup()
	{
		WSACleanup();
	}

	void StartClient(int argc, char* argv[])
	{
		Init();

		SOCKET connectSock = socket(PF_INET, SOCK_STREAM, 0);
		if (connectSock == INVALID_SOCKET)
		{
			utils::error::ErrorHandling(std::format("[E] socket - {}", WSAGetLastError()));
		}

		sockaddr_in connectAddr = { 0 };
		connectAddr.sin_family = AF_INET;
		connectAddr.sin_addr.S_un.S_addr = inet_addr(argv[1]);
		connectAddr.sin_port = htons(atoi(argv[2]));

		if (connect(connectSock, reinterpret_cast<sockaddr*>(&connectAddr), sizeof(connectAddr)) != 0)
		{
			utils::error::ErrorHandling(std::format("[E] connect - {}", WSAGetLastError()));
		}

		std::cout << "connect IP: " << argv[1] << " Port: " << argv[2] << std::endl;

		char buf[1024] = { 0 };
		while (true)
		{
			std::cin >> buf;
			int bufSize = std::strlen(buf);
			send(connectSock, buf, bufSize, 0);

			if (recv(connectSock, buf, bufSize, 0) == 0)
			{
				break;
			}
			std::cout << "[server -> client] " << buf << std::endl;
		}

		std::cout << "Client End" << std::endl;

		closesocket(connectSock);
		Cleanup();
	}
}