#pragma once
#include "../../pch.h"

#include <iostream>
#include <iostream>
#include <string>
#include <string_view>
#include <format>

namespace park18::chapter4
{
	constexpr unsigned int BUF_SIZE = 1024;

	/// <summary>
	/// echo client
	/// </summary>
	int entry(int argc, char* argv[])
	{
		if (argc != 3)
		{
			std::cout << std::format("Usage: {} <IP>  <Port>", argv[0]) << std::endl;
			::exit(1);
		}

		// 1. 소켓 기능 활성화
		WSADATA wsaData;
		if (::WSAStartup(MAKEWORD(2, 2, ), &wsaData) != 0)
		{
			utils::error::ErrorHandling("WSAStatup() Error");
		}

		// 2. 서버 연결 소켓 생성
		SOCKET hServerSock = ::socket(PF_INET, SOCK_STREAM, 0);
		if (hServerSock == INVALID_SOCKET)
		{
			utils::error::ErrorHandling("socket() Error");
		}

		// 3. 소켓 주소 초기화
		SOCKADDR_IN serverAddress;
		::memset(&serverAddress, 0, sizeof(serverAddress));
		serverAddress.sin_family			= AF_INET;
		serverAddress.sin_addr.S_un.S_addr	= ::inet_addr(argv[1]);
		serverAddress.sin_port				= ::htons(::atoi(argv[2]));

		// 4. 서버 연결 요청
		if (::connect(hServerSock, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
		{
			utils::error::ErrorHandling("connect() Error");
		}
		else
		{
			std::cout << "Conntected..." << std::endl;
		}

		char message[BUF_SIZE] = { 0, };
		while (true)
		{
			// 버퍼 초기화
			::memset(&message, 0, BUF_SIZE);

			std::cout << "me: ";
			std::cin >> message;

			if (::strcmp(message, "q") == 0 || ::strcmp(message, "Q") == 0)
			{
				break;
			}

			// 5. 메시지 전송
			::send(hServerSock, message, ::strlen(message), 0);

			// 6. 메시지 수신
			auto inputSize = ::recv(hServerSock, message, BUF_SIZE - 1, 0);
			message[inputSize] = 0;

			std::cout << "Server: " << message << std::endl;
		}

		// 7.소켓 종료 및 소켓 기능 비활성화
		::closesocket(hServerSock);
		::WSACleanup();

		return 0;
	}
}