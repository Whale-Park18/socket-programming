#include "echo_client.h"

park18::chapter4::echo_client::echo_client(int argc, char* argv[])
{
	init(argc, argv);
}

park18::chapter4::echo_client::~echo_client()
{
	closesocket(this->connectSock);
	WSACleanup();
}

void park18::chapter4::echo_client::start()
{
	ready();

	char message[BUF_SIZE] = { 0 };
	while (true)
	{
		std::cout << "me: ";
		std::cin >> message;

		if (::strcmp(message, "q") == 0 || ::strcmp(message, "Q") == 0)
		{
			break;
		}

		// 메시지 전송
		::send(this->connectSock, message, ::strlen(message), 0);

		// 메시지 수신
		auto inputSize = ::recv(this->connectSock, message, BUF_SIZE - 1, 0);
		message[inputSize] = 0;

		std::cout << "Server: " << message << std::endl;
	}
}

void park18::chapter4::echo_client::init(int argc, char* argv[])
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

void park18::chapter4::echo_client::ready()
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
