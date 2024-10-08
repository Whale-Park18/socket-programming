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

	void echo_client(int argc, char* argv[])
	{
		init();

		SOCKET connectSock = socket(PF_INET, SOCK_STREAM, 0);
		if (connectSock == INVALID_SOCKET)
		{
			utils::error::error_handling(std::format("[E] socket - {}", WSAGetLastError()));
		}

		sockaddr_in connectAddr = { 0 };
		connectAddr.sin_family = AF_INET;
		connectAddr.sin_addr.S_un.S_addr = inet_addr(argv[1]);
		connectAddr.sin_port = htons(atoi(argv[2]));

		if (connect(connectSock, reinterpret_cast<sockaddr*>(&connectAddr), sizeof(connectAddr)) != 0)
		{
			utils::error::error_handling(std::format("[E] connect - {}", WSAGetLastError()));
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
		cleanup();
	}
}