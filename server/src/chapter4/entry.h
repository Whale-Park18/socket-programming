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
	/// echo server
	/// </summary>
	int entry(int argc, char* argv[])
	{
		if (argc != 2)
		{
			std::cout << std::format("Usage: {} <Port>", argv[0]) << std::endl;
			::exit(1);
		}
		
		// 1. 소켓 기능 활성화
		WSADATA wsaData;
		if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		{
			utils::error::ErrorHandling("WSAStatup() Error");
		}

		// 2. 호스트 연결 수신 소켓 생성
		SOCKET hListenSock = ::socket(PF_INET, SOCK_STREAM, 0);
		if (hListenSock == INVALID_SOCKET)
		{
			utils::error::ErrorHandling("socket() Error");
		}

		// 3.1. 소켓 주소 초기화
		SOCKADDR_IN listenAddress;
		::memset(&listenAddress, 0, sizeof(listenAddress));
		listenAddress.sin_family			= AF_INET;					// IPv4
		listenAddress.sin_addr.S_un.S_addr	= ::htonl(INADDR_ANY);		// 모든 주소
		listenAddress.sin_port				= ::htons(::atoi(argv[1]));

		// 3.2. 소켓 주소 할당
		if (::bind(hListenSock, (SOCKADDR*)&listenAddress, sizeof(listenAddress)) == SOCKET_ERROR)
		{
			utils::error::ErrorHandling("bind() Error");
		}

		// 4. 연결 가능 상태 전환
		if (::listen(hListenSock, 5) == SOCKET_ERROR)
		{
			utils::error::ErrorHandling("listen() Error");
		}

		SOCKET hClientSock = NULL;
		SOCKADDR_IN clientAddres;
		int clientAddresSize = sizeof(clientAddres);

		char buffer[BUF_SIZE] = { 0, };

		for (int i = 0; i < 5; i++)
		{
			// 5. 호스트 연결 요청 응답
			hClientSock = ::accept(hListenSock, (SOCKADDR*)&clientAddres, &clientAddresSize);
			if (hClientSock == -1)
			{
				utils::error::ErrorHandling("accept() Error");
			}
			
			std::string		hostIP	 = ::inet_ntoa(clientAddres.sin_addr);
			types::ushort	hostPort = ::ntohs(clientAddres.sin_port);
			std::cout << std::format("connect info IP: {}, Port: {}", hostIP, hostPort) << std::endl;

			// 5. 메시지 수신
			int inputSize = 0;
			while ((inputSize = ::recv(hClientSock, buffer, BUF_SIZE, 0)) != SOCKET_ERROR)
			{
				// 6. 메시지 전송
				std::cout << std::format("host({}): ", hostIP) << buffer << std::endl;
				::send(hClientSock, buffer, inputSize, 0);

				// 버퍼 초기화
				::memset(&buffer, 0, BUF_SIZE);
			}

			// 호스트 소켓 종료
			::closesocket(hClientSock);
		}

		// 7. 호스트 연결 수신 소켓 종료 및 소켓 기능 비활성화
		::closesocket(hListenSock);
		::WSACleanup();

		return 0;
	}
}