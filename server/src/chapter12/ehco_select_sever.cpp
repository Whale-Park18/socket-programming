#include "ehco_select_sever.h"

park18::chapter12::echo_select_server::echo_select_server(int argc, char* argv[])
{
	init(argc, argv);
}

park18::chapter12::echo_select_server::~echo_select_server()
{
	closesocket(listenSock);
	WSACleanup();
}

void park18::chapter12::echo_select_server::start()
{
	ready();

	while (true)
	{
		// 원본 fd_set(read)를 복사본에 저장
		this->copyRead = this->read;

		set_timeout();
		int fdCount = select(0, &this->copyRead, nullptr, nullptr, &this->timeout);
		if (fdCount == SOCKET_ERROR) // 
		{
			break;
		}
		else if (fdCount == 0)		 // timeout
		{
			continue;
		}

		for (int i = 0; i < this->read.fd_count; i++)
		{
			// 변화가 발생하지 않았으면 다음을 확인
			if (FD_ISSET(this->read.fd_array[i], &this->copyRead) == 0)
			{
				continue;
			}

			SOCKET eventSock = this->read.fd_array[i];
			// listenSock에서 읽기 발생 = 클라이언트에서 연결 요청
			if (eventSock == this->listenSock)
			{
				// 연결 수락
				sockaddr_in acceptAddress = { 0 };
				int acceptAddressSize = sizeof(acceptAddress);
				this->acceptSock = accept(this->listenSock, reinterpret_cast<sockaddr*>(&acceptAddress), &acceptAddressSize);
				
				// 원본 fd_set에 등록
				FD_SET(this->acceptSock, &this->read);
				std::cout << "connected client: " << this->acceptSock << std::endl;
			}
			// acceptSock에서 읽기 발생 = 메시지 수신
			else
			{
				this->recvCount = recv(eventSock, this->buf, _countof(this->buf) - 1, 0);

				// 연결 종료 수신
				if (this->recvCount == 0)
				{
					FD_CLR(eventSock, &this->read);
					closesocket(eventSock);
					std::cout << "closed client: " << eventSock << std::endl;
				}
				// 메시지 에코
				else
				{
					this->buf[this->recvCount] = '\0';
					std::cout << std::format("client {}: {}", eventSock, this->buf) << std::endl;
					send(eventSock, this->buf, this->recvCount, 0);
				}
			}
		}
	}
}

void park18::chapter12::echo_select_server::init(int argc, char* argv[])
{
	// 프로그램 인자 예외 처리
	if (argc != 2)
	{
		utils::error::error_handling("[E] echo_select_server.init - the number of arguments is wrong");
	}
	this->port = static_cast<unsigned short>(std::stoi(argv[1]));

	// Win sock 예외 처리
	SecureZeroMemory(&this->wsaData, sizeof(this->wsaData));
	utils::network::net_startup(this->wsaData);
}

void park18::chapter12::echo_select_server::ready()
{
	this->listenSock = socket(AF_INET, SOCK_STREAM, 0);

	sockaddr_in listenAddress = { 0 };
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

	// listenSock을 읽기에 등록
	SecureZeroMemory(&this->read, sizeof(this->read));
	SecureZeroMemory(&this->copyRead, sizeof(this->copyRead));
	FD_ZERO(&this->read);
	FD_SET(this->listenSock, &this->read);
}

void park18::chapter12::echo_select_server::set_timeout()
{
	SecureZeroMemory(&this->timeout, sizeof(this->timeout));
	this->timeout.tv_sec = 5;
	this->timeout.tv_usec = 5000;
}
