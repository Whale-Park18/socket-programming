#include "file_client.h"

park18::chapter7::file_client::file_client(int argc, char* argv[])
{
	init(argc, argv);
}

park18::chapter7::file_client::~file_client()
{
	closesocket(this->connectSock);
	WSACleanup();
}

void park18::chapter7::file_client::start()
{
	ready();

	char buffer[types::BufSize] = { 0 };
	FILE* pFile = nullptr;
	fopen_s(&pFile, "test.txt", "wb");

	int readCount = 0;
	while ((readCount = recv(this->connectSock, buffer, types::BufSize, 0)) != 0)
	{
		fwrite(reinterpret_cast<void*>(buffer), sizeof(char), readCount, pFile);
	}
	fclose(pFile);

	std::cout << "[server to client] recv file" << std::endl;
	send(this->connectSock, "Thanck you", 10, 0);
}

void park18::chapter7::file_client::init(int argc, char* argv[])
{
	if (argc != 3)
	{
		utils::error::error_handling(std::format("Usage: {} <IP> <port>", argv[0]));
	}
	this->host = argv[1];
	this->port = std::atoi(argv[2]);

	// 소켓 기능 활성화
	WSADATA wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		utils::error::error_handling("WSAStartup() error");
	}
}

void park18::chapter7::file_client::ready()
{
	// 소켓 생성
	this->connectSock = ::socket(PF_INET, SOCK_STREAM, 0);
	if (this->connectSock == INVALID_SOCKET)
	{
		utils::error::error_handling("socket() error");
	}

	SOCKADDR_IN serverAddress = { 0, };
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.S_un.S_addr = ::inet_addr(this->host.c_str());
	serverAddress.sin_port = htons(this->port);

	// 연결 요청
	if (::connect(this->connectSock, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
	{
		utils::error::error_handling("connect() error");
	}
}
