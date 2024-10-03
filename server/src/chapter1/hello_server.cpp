#include "hello_server.h"

park18::chapter1::server::server(int argc, char* argv[])
{
	init(argc, argv);
}

park18::chapter1::server::~server()
{
	closesocket(this->acceptSock);
	closesocket(this->listenSock);
	WSACleanup();
}

void park18::chapter1::server::start()
{
	ready();

	sockaddr_in acceptAddress = { 0 };
	int acceptAddressSize = sizeof(acceptAddress);

	this->acceptSock = accept(this->listenSock, reinterpret_cast<sockaddr*>(&acceptAddress), &acceptAddressSize);
	if (this->acceptSock == SOCKET_ERROR)
	{
		utils::error::error_handling("accept() error");
	}

	// 메시지 전송
	char message[] = "Hello TCP/IP";
	send(this->acceptSock, message, _countof(message), 0);
}

void park18::chapter1::server::init(int argc, char* argv[])
{
	// 프로그램 인자 예외 처리
	if (argc != 2)
	{
		utils::error::error_handling("[E] server.init - the number of arguments is wrong");
	}
	this->port = static_cast<unsigned short>(std::stoi(argv[1]));

	// Win sock 기능 활성화 및 예외 처리
	SecureZeroMemory(&this->wsaData, sizeof(this->wsaData));
	if (WSAStartup(MAKEWORD(2, 2), &this->wsaData) != 0)
	{
		utils::error::error_handling("[E] server.init - WSADATA initialization falied");
	}
}

void park18::chapter1::server::ready()
{
	// 서버 연결 요청 수신 소켓 생성
	this->listenSock = socket(AF_INET, SOCK_STREAM, 0);

	sockaddr_in listenAddress = { 0 };
	listenAddress.sin_family = AF_INET;
	listenAddress.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	listenAddress.sin_port = htons(this->port);

	// 서버 소켓에 주소, 포트 정보 할당
	if (bind(this->listenSock, reinterpret_cast<sockaddr*>(&listenAddress), sizeof(listenAddress)) == SOCKET_ERROR)
	{
		utils::error::error_handling(std::format("[E] bind - code: {}", WSAGetLastError()));
	}

	// 연결 가능 상태 전환
	if (listen(this->listenSock, 5) == SOCKET_ERROR)
	{
		utils::error::error_handling(std::format("[E] listen - code: {}", WSAGetLastError()));
	}
}
