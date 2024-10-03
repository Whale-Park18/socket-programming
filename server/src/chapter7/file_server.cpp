#include "file_server.h"

park18::chapter7::file_server::file_server(int argc, char* argv[])
{
	init(argc, argv);
}

park18::chapter7::file_server::~file_server()
{
	closesocket(this->acceptSock);
	closesocket(this->listenSock);
	WSACleanup();
}

void park18::chapter7::file_server::start()
{
	ready();

	SOCKADDR_IN clientAddress = { 0 };
	int clientAddressSize = sizeof(clientAddress);
	this->acceptSock = accept(this->listenSock, reinterpret_cast<SOCKADDR*>(&clientAddress), &clientAddressSize);

	FILE* pFile = nullptr;
	fopen_s(&pFile, "test.txt", "rb");
	char buffer[types::BufSize] = { 0 };

	if (pFile == nullptr)
	{
		utils::error::error_handling("[E] Falied fopen_s");
	}

	while (true)
	{
		size_t readCount = fread(buffer, sizeof(char), types::BufSize, pFile);
		if (readCount < types::BufSize)
		{
			send(this->acceptSock, buffer, readCount, 0);
			break;
		}
		send(this->acceptSock, buffer, types::BufSize, 0);
	}
	fclose(pFile);

	shutdown(this->acceptSock, SD_SEND);
	recv(this->acceptSock, buffer, types::BufSize, 0);
	std::cout << "[client to server] " << buffer << std::endl;
}

void park18::chapter7::file_server::init(int argc, char* argv[])
{
	// 프로그램 인자 예외 처리
	if (argc != 2)
	{
		utils::error::error_handling("[E] echo_select_server.init - the number of arguments is wrong");
	}
	this->port = static_cast<unsigned short>(std::stoi(argv[1]));

	// Win sock 예외 처리
	SecureZeroMemory(&this->wsaData, sizeof(this->wsaData));
	if (WSAStartup(MAKEWORD(2, 2), &this->wsaData) != 0)
	{
		utils::error::error_handling("[E] echo_select_server.init - WSADATA initialization falied");
	}
}

void park18::chapter7::file_server::ready()
{
	this->listenSock = socket(PF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN listenAddress = { 0 };
	listenAddress.sin_family = AF_INET;
	listenAddress.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	listenAddress.sin_port = htons(this->port);

	if (bind(this->listenSock, reinterpret_cast<sockaddr*>(&listenAddress), sizeof(listenAddress)) == SOCKET_ERROR)
	{
		utils::error::error_handling(std::format("[E] bind - code: {}", WSAGetLastError()));
	}

	if (listen(this->listenSock, 5) == SOCKET_ERROR)
	{
		utils::error::error_handling(std::format("[E] listen - code: {}", WSAGetLastError()));
	}
}
