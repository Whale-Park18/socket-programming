#pragma once
#include "pch.h"

namespace park18::chapter7
{
	void FileClientStart(int argc, char* argv[])
	{
		WSADATA wsaData = { 0 };
		WSAStartup(MAKEWORD(2, 2), &wsaData);

		SOCKET serverSock = socket(PF_INET, SOCK_STREAM, 0);

		SOCKADDR_IN serverAddress = { 0 };
		serverAddress.sin_family = AF_INET;
		serverAddress.sin_addr.S_un.S_addr = inet_addr(argv[1]);
		serverAddress.sin_port = htons(atoi(argv[2]));

		connect(serverSock, reinterpret_cast<SOCKADDR*>(&serverAddress), sizeof(serverAddress));

		char buffer[types::BufSize] = { 0 };
		FILE* pFile = nullptr;
		fopen_s(&pFile, "test.txt", "wb");

		int readCount = 0;
		while ((readCount = recv(serverSock, buffer, types::BufSize, 0)) != 0)
		{
			fwrite(reinterpret_cast<void*>(buffer), sizeof(char), readCount, pFile);
		}
		fclose(pFile);

		std::cout << "[server to client] recv file" << std::endl;
		send(serverSock, "Thanck you", 10, 0);

		closesocket(serverSock);
		WSACleanup();
	}
}