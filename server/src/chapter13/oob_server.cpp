#include "global.h"
#include "oob_server.h"

park18::chapter13::oob_server::oob_server(int argc, char* argv[])
{
	init(argc, argv);
}

void park18::chapter13::oob_server::start()
{
	ready();

	// 클라이언트 소켓 연결
	sockaddr_in acceptAddress = { 0 };
	int acceptAddrSize = sizeof(acceptAddress);
	this->acceptSock = accept(this->lisetnSock, reinterpret_cast<sockaddr*>(&acceptAddress), &acceptAddrSize);
	if (this->acceptSock == INVALID_SOCKET)
	{
		utils::error::error_handling(std::format("[E] accept - code: {}", WSAGetLastError()));
	}

	// 연결된 소켓으로 fd_set 설정
	FD_SET(this->acceptSock, &this->read);
	FD_SET(this->acceptSock, &this->except);

	while (true)
	{
		// select 용 fd_set
		this->readCopy = this->read;
		this->exceptCopy = this->except;
		
		// 타임아웃
		this->timeout.tv_sec = 5;
		this->timeout.tv_usec = 0;

		// 소켓에 들어온 메시지가 없으면 예외처리
		int selectCount = select(0, &this->readCopy, nullptr, &this->except, &this->timeout);

		if (selectCount == 0)
		{
			continue;
		}
		else if (selectCount == SOCKET_ERROR)
		{
			utils::error::error_handling(std::format("[E] select - code: {}", WSAGetLastError()));
		}

		if (FD_ISSET(this->acceptSock, &this->exceptCopy))
		{
			this->recvCount = recv(this->acceptSock, this->buf, oob_server::BUF_SIZE - 1, MSG_OOB);
			if (this->recvCount == 0)
			{
				break;
			}
			else if (this->recvCount == SOCKET_ERROR)
			{
				utils::error::error_handling(std::format("[E] recv1 - code: {}", WSAGetLastError()));
			}
			
			this->buf[this->recvCount] = '\0';
			std::cout << "Urgent message: " << this->buf << std::endl;
		}
		else if (FD_ISSET(this->acceptSock, &this->readCopy))
		{
			this->recvCount = recv(this->acceptSock, this->buf, oob_server::BUF_SIZE - 1, 0);
			if (this->recvCount == 0)
			{
				break;
			}
			else if (this->recvCount == SOCKET_ERROR)
			{
				utils::error::error_handling(std::format("[E] recv2 - code: {}", WSAGetLastError()));
			}

			this->buf[this->recvCount] = '\0';
			std::cout << "message: " << this->buf << std::endl;
		}
	}
}

void park18::chapter13::oob_server::init(int argc, char* argv[])
{
	// 프로그램 인자 예외 처리
	if (argc != 2)
	{
		utils::error::error_handling("[E] echo_select_server.init - the number of arguments is wrong");
	}
	this->port = static_cast<unsigned short>(std::stoi(argv[1]));

	this->lisetnSock = INVALID_SOCKET;
	this->acceptSock = INVALID_SOCKET;

	FD_ZERO(&this->read);
	FD_ZERO(&this->except);

	this->recvCount = 0;
	SecureZeroMemory(this->buf, sizeof(this->buf));
}

void park18::chapter13::oob_server::ready()
{
	this->lisetnSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (this->lisetnSock == INVALID_SOCKET)
	{
		utils::error::error_handling(std::format("[E] socket - code: {}", WSAGetLastError()));
	}

	sockaddr_in listenAddress = { 0 };
	listenAddress.sin_family = PF_INET;
	listenAddress.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	listenAddress.sin_port = htons(this->port);

	if (bind(this->lisetnSock, reinterpret_cast<sockaddr*>(&listenAddress), sizeof(listenAddress)) == SOCKET_ERROR)
	{
		utils::error::error_handling(std::format("[E] bind - code: {}", WSAGetLastError()));
	}

	if (listen(this->lisetnSock, 5) == SOCKET_ERROR)
	{
		utils::error::error_handling(std::format("[E] listen - code: {}", WSAGetLastError()));
	}
}
