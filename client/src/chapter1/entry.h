#pragma once
#include "../../pch.h"

#include <iostream>
#include <string>
#include <string_view>
#include <format>

#include <WS2tcpip.h>

namespace park18::chapter1
{
	int entry(int argc, char* argv[])
	{
		if (argc != 3)
		{
			std::cout << std::format("Usage: {} <IP> <port>", argv[0]) << std::endl;
			::exit(1);
		}

		// 소켓 기능 활성화
		WSADATA wsaData;
		if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		{
			utils::error::ErrorHandling("WSAStartup() error");
		}


		// 소켓 생성
		SOCKET hSocket = ::socket(PF_INET, SOCK_STREAM, 0);
		if (hSocket == INVALID_SOCKET)
		{
			utils::error::ErrorHandling("socket() error");
		}

		SOCKADDR_IN serverAddress = { 0, };
		serverAddress.sin_family = AF_INET;
		serverAddress.sin_addr.S_un.S_addr = ::inet_addr(argv[1]);
		serverAddress.sin_port = htons(atoi(argv[2]));

		// 연결 요청
		if (::connect(hSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
		{
			utils::error::ErrorHandling("connect() error");
		}

		// 메시지 수신
		char message[MAX_PATH] = { 0, };
		int messageLength = ::recv(hSocket, message, _countof(message) - 1, 0);
		if (messageLength == -1)
		{
			utils::error::ErrorHandling("read() error");
		}

		std::cout << std::format("Message from [Server]: {}", message) << std::endl;

		// 소켓 정리 및 기능 비활성화
		::closesocket(hSocket);
		::WSACleanup();
	}
}