#include "echo_server.h"

park18::chapter4::echo_server::echo_server(int argc, char* argv[])
{
	init(argc, argv);
}

park18::chapter4::echo_server::~echo_server()
{
	closesocket(this->acceptSock);
	closesocket(this->listenSock);
	WSACleanup();
}

void park18::chapter4::echo_server::start()
{
	ready();

	sockaddr_in acceptAddress = { 0 };
	int acceptAddressSize = sizeof(acceptAddress);

	for (int i = 0; i < 5; i++)
	{
		// 5. 호스트 연결 요청 응답
		this->acceptSock = accept(this->listenSock, reinterpret_cast<sockaddr*>(&acceptAddress), &acceptAddressSize);
		if (this->acceptSock == SOCKET_ERROR)
		{
			utils::error::error_handling("accept() Error");
		}

		std::string	  hostIP = ::inet_ntoa(acceptAddress.sin_addr);
		types::ushort hostPort = ::ntohs(acceptAddress.sin_port);
		std::cout << std::format("connect info IP: {}, Port: {}", hostIP, hostPort) << std::endl;

		// 6. 메시지 수신
		while ((this->recvCount = recv(this->acceptSock, this->buf, BUF_SIZE, 0)) != SOCKET_ERROR)
		{
			this->buf[this->recvCount] = '\0';

			// 7. 메시지 전송(에코)
			std::cout << std::format("host({}): ", hostIP) << this->buf << std::endl;
			send(this->acceptSock, this->buf, this->recvCount, 0);
		}
	}
}

void park18::chapter4::echo_server::init(int argc, char* argv[])
{
	// 프로그램 인자 예외 처리
	if (argc != 2)
	{
		utils::error::error_handling("[E] echo_select_server.init - the number of arguments is wrong");
	}
	this->port = static_cast<unsigned short>(std::stoi(argv[1]));

	// 1. Win sock 활성화 및 예외 처리
	SecureZeroMemory(&this->wsaData, sizeof(this->wsaData));
	if (WSAStartup(MAKEWORD(2, 2), &this->wsaData) != 0)
	{
		utils::error::error_handling("[E] echo_select_server.init - WSADATA initialization falied");
	}
}

void park18::chapter4::echo_server::ready()
{
	// 2. 서버 연결 수신 소켓 생성
	this->listenSock = socket(AF_INET, SOCK_STREAM, 0);

	// 3.1. 서버 소켓 주소 설정
	sockaddr_in listenAddress = { 0 };
	listenAddress.sin_family = AF_INET;
	listenAddress.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	listenAddress.sin_port = htons(this->port);

	// 3.2. 서버 소켓 주소 할당
	if (bind(this->listenSock, reinterpret_cast<sockaddr*>(&listenAddress), sizeof(listenAddress)) == SOCKET_ERROR)
	{
		utils::error::error_handling(std::format("[E] bind - code: {}", WSAGetLastError()));
	}

	// 4. 서버 소켓 연결 가능 상태 전환
	if (listen(this->listenSock, 5) == SOCKET_ERROR)
	{
		utils::error::error_handling(std::format("[E] listen - code: {}", WSAGetLastError()));
	}
}
