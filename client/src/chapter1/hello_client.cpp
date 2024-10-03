#include "hello_client.h"

park18::chapter1::client::client(int argc, char* argv[])
{
	init(argc, argv);
}

park18::chapter1::client::~client()
{
	closesocket(this->connectSock);
	WSACleanup();
}

void park18::chapter1::client::start()
{
	ready();

	// 메시지 수신
	char message[MAX_PATH] = { 0, };
	int messageLength = ::recv(this->connectSock, message, _countof(message) - 1, 0);
	if (messageLength == -1)
	{
		utils::error::error_handling("read() error");
	}

	std::cout << std::format("Message from [Server]: {}", message) << std::endl;
}

void park18::chapter1::client::init(int argc, char* argv[])
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

void park18::chapter1::client::ready()
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
