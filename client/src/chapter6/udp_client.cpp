#include "global.h"
#include "udp_client.h"

park18::chapter6::udp_client::udp_client(int argc, char* argv[])
{
	init(argc, argv);
}

park18::chapter6::udp_client::~udp_client()
{
	closesocket(this->connectSock);
	WSACleanup();
}

void park18::chapter6::udp_client::start()
{
	ready();

	// 연결된 서버의 주소 정보
	this->connectAddress.sin_family = AF_INET;
	this->connectAddress.sin_addr.S_un.S_addr = ::inet_addr(this->host.c_str());
	this->connectAddress.sin_port = htons(this->port);

	char message[1024] = { 0 };
	while (true)
	{
		std::cout << "[client -> server] ";
		std::cin.getline(message, 1024);

		// 메시지를 전송할 때 마다 수신 주소 정보를 같이 첨부해야 함
		::sendto(this->connectSock, message, std::strlen(message), 0, reinterpret_cast<sockaddr*>(&this->connectAddress), this->connectAddressSize);

		// 메시지를 수신할 때 마다 송신 주소 정보를 같이 첨부해야 함
		int recvSize = ::recvfrom(this->connectSock, message, 1024, 0, reinterpret_cast<sockaddr*>(&this->connectAddress), &this->connectAddressSize);
		std::cout << "[server -> client] " << message << std::endl;
	}
}

void park18::chapter6::udp_client::init(int argc, char* argv[])
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

	SecureZeroMemory(&this->connectAddress, sizeof(this->connectAddress));
	this->connectAddressSize = sizeof(this->connectAddress);
}

void park18::chapter6::udp_client::ready()
{
	// 소켓 생성
	this->connectSock = ::socket(PF_INET, SOCK_DGRAM, 0);
	if (this->connectSock == INVALID_SOCKET)
	{
		utils::error::error_handling("socket() error");
	}
}

park18::chapter6::udp_connect_client::udp_connect_client(int argc, char* argv[]) : udp_client(argc, argv)
{
	
}

void park18::chapter6::udp_connect_client::start()
{
	ready();

	char message[1024] = { 0 };
	while (true)
	{
		std::cout << "[client -> server] ";
		std::cin.getline(message, 1024);

		// udp를 연결했기 때문에 send() 함수를 호출할 수 있음
		//sendto(this->connectSock, message, std::strlen(message), 0, reinterpret_cast<sockaddr*>(&this->connectAddress), this->connectAddressSize);
		send(this->connectSock, message, std::strlen(message), 0);

		// udp를 연결했기 때문에 recv() 함수를 호출할 수 있음
		//int recvSize = recvfrom(this->connectSock, message, 1024, 0, reinterpret_cast<sockaddr*>(&this->connectAddress), &this->connectAddressSize);
		int recvSize = recv(this->connectSock, message, 1024, 0);
		std::cout << "[server -> client] " << message << std::endl;
	}
}

void park18::chapter6::udp_connect_client::ready()
{
	udp_client::ready();

	this->connectAddress.sin_family = AF_INET;
	this->connectAddress.sin_addr.S_un.S_addr = ::inet_addr(this->host.c_str());
	this->connectAddress.sin_port = htons(this->port);

	// udp 연결
	if (::connect(this->connectSock, reinterpret_cast<sockaddr*>(&this->connectAddress), this->connectAddressSize) == SOCKET_ERROR)
	{
		utils::error::error_handling("connect() error");
	}
}