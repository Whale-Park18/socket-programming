#pragma once
#include "../../pch.h"

#include <iostream>
#include <string>
#include <string_view>
#include <format>

namespace park18::chapter1
{	
	static int entry(int argc, char* argv[])
	{
		if (argc != 2)
		{
			std::cout << std::format("Usage: {} <port>", argv[0]) << std::endl;
			::exit(1);
		}
		
		// 소켓 기능 활성화
		WSADATA wsaData;
		if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		{
			utils::error::ErrorHandling("WSAStartup() error");
		}
		
		// 소켓 생성
		SOCKET hServerSock = ::socket(PF_INET, SOCK_STREAM, 0);
		if (hServerSock == INVALID_SOCKET)
		{
			utils::error::ErrorHandling("socket() error");
		}
		
		SOCKADDR_IN serverAddress = { 0, };
		serverAddress.sin_family = AF_INET;
		serverAddress.sin_addr.S_un.S_addr = ::htonl(INADDR_ANY);
		serverAddress.sin_port = htons(atoi(argv[1]));
		
		// 소켓에 주소, 포트 정보 할당
		if (::bind(hServerSock, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
		{
			utils::error::ErrorHandling("bind() error");	
		}
		
		// 연결 가능 상태 전환
		if (::listen(hServerSock, 5) == SOCKET_ERROR)
		{
			utils::error::ErrorHandling("listen() error");
		}
		
		SOCKADDR_IN clinetAddress = { 0, };
		int clientAddressLength = sizeof(clinetAddress);	
		
		// 연결 요청 응답
		SOCKET hClientSock = ::accept(hServerSock, (SOCKADDR*)&clinetAddress, &clientAddressLength);
		if (hClientSock == INVALID_SOCKET)
		{
			utils::error::ErrorHandling("accept() error");
		}
	
		// 메시지 전송
		char message[] = "Hello TCP/IP";
		::send(hClientSock, message, _countof(message), 0);
		
		// 소켓 정리 및 기능 비활성화
		::closesocket(hClientSock);
		::closesocket(hServerSock);
		::WSACleanup();
		
		return 0;
	}
}