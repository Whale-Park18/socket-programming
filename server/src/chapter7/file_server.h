#pragma once

namespace park18::chapter7
{
	void FileServerStart(int argc, char* argv[])
	{
		WSADATA wsaData = { 0 };
		WSAStartup(MAKEWORD(2, 2), &wsaData);

		SOCKET listenSock = socket(PF_INET, SOCK_STREAM, 0);

		SOCKADDR_IN listenAddress = { 0 };
		listenAddress.sin_family = AF_INET;
		listenAddress.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
		listenAddress.sin_port = htons(atoi(argv[1]));

		bind(listenSock, reinterpret_cast<SOCKADDR*>(&listenAddress), sizeof(listenAddress));
		listen(listenSock, 5);

		SOCKADDR_IN clientAddress = { 0 };
		int clientAddressSize = sizeof(clientAddress);
		SOCKET clientSock = accept(listenSock, reinterpret_cast<SOCKADDR*>(&clientAddress), &clientAddressSize);

		FILE* pFile = nullptr;
		fopen_s(&pFile, "test.txt", "rb");
		char buffer[types::BufSize] = { 0 };
		while (true)
		{
			size_t readCount = fread(buffer, sizeof(char), types::BufSize, pFile);
			if (readCount < types::BufSize)
			{
				send(clientSock, buffer, readCount, 0);
				break;
			}
			send(clientSock, buffer, types::BufSize, 0);
		}
		fclose(pFile);

		shutdown(clientSock, SD_SEND);
		recv(clientSock, buffer, types::BufSize, 0);
		std::cout << "[client to server] " << buffer << std::endl;

		closesocket(clientSock);
		closesocket(listenSock);
		WSACleanup();
	}
}