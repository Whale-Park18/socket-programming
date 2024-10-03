#include "socket_option.h"

park18::chapter9::socket_option_echo_server::socket_option_echo_server(int argc, char* argv[])
{
	init(argc, argv);
}

park18::chapter9::socket_option_echo_server::~socket_option_echo_server()
{
	closesocket(this->acceptSock);
	closesocket(this->listenSock);
	WSACleanup();
}

void park18::chapter9::socket_option_echo_server::start()
{
	std::cout << "Start Server" << std::endl;

	ready();

	SOCKADDR_IN acceptAddr = { 0 };
	int acceptAddrSize = sizeof(acceptAddr);
	this->acceptSock = accept(this->listenSock, reinterpret_cast<SOCKADDR*>(&acceptAddr), &acceptAddrSize);

	print_accept_info(acceptAddr);
	while ((this->recvCount = recv(this->acceptSock, this->buf, 1024, 0)) != SOCKET_ERROR)
	{
		this->buf[this->recvCount] = '\0';
		std::cout << "[client -> server] " << this->buf << std::endl;
		send(this->acceptSock, this->buf, this->recvCount, 0);
	}

	std::cout << "Server End" << std::endl;
}

void park18::chapter9::socket_option_echo_server::init(int argc, char* argv[])
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

void park18::chapter9::socket_option_echo_server::ready()
{
	this->listenSock = socket(PF_INET, SOCK_STREAM, 0);
	if (listenSock == INVALID_SOCKET)
	{
		utils::error::error_handling(std::format("[E] listenSock - {}", WSAGetLastError()));
	}

	// 소켓 옵션 설정
	bool option = true;
	int optionSize = sizeof(option);
	if (setsockopt(listenSock, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char*>(&option), optionSize) != 0)
	{
		utils::error::error_handling(std::format("[E] setsockopt - {}", WSAGetLastError()));
	}

	SOCKADDR_IN listenAddr = { 0 };
	listenAddr.sin_family = AF_INET;
	listenAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	listenAddr.sin_port = htons(this->port);

	if (bind(listenSock, reinterpret_cast<SOCKADDR*>(&listenAddr), sizeof(listenAddr)) == SOCKET_ERROR)
	{
		utils::error::error_handling(std::format("[E] bind - {}", WSAGetLastError()));
	}

	if (listen(listenSock, 5) == SOCKET_ERROR)
	{
		utils::error::error_handling(std::format("[E] listen - {}", WSAGetLastError()));
	}
}

void park18::chapter9::socket_option_echo_server::print_accept_info(const sockaddr_in& acceptAddress)
{
	char ip[1024] = { 0 };
	strcpy_s(ip, inet_ntoa(acceptAddress.sin_addr));

	types::ushort port = ntohs(acceptAddress.sin_port);

	std::cout << std::format("AcceptInfo IP: {}, Port: {}", ip, port) << std::endl;
}
