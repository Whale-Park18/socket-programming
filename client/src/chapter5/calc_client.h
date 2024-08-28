#pragma once
#include "global.h"

#include <vector>
#include <memory>

namespace park18::chapter5
{
	constexpr int SIZEOF_OPERAND = 4;

	/// <summary>
	/// 계산기 클라이언트 시작 메소드
	/// </summary>
	/// <param name="argc">애플리케이션 인자 개수</param>
	/// <param name="argv">애플리케이션 인자</param>
	void calc_client(int argc, char* argv[])
	{
		if (argc != 3)
		{
			utils::error::ErrorHandling(std::format("[E] {} Argument: Server-IP, Server-Port", argv[0]));
		}

		// 1. 소켓 기능 활성화
		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		{
			utils::error::ErrorHandling(std::format("[E] WSAStartup: {}", ::GetLastError()));
		}

		// 2. 서버 소켓 생성
		SOCKET serverSock = ::socket(AF_INET, SOCK_STREAM, 0);
		if (serverSock == INVALID_SOCKET)
		{
			utils::error::ErrorHandling(std::format("[E] socket: {}", ::WSAGetLastError()));
		}

		// 3.1. 서버 소켓 주소 할당
		SOCKADDR_IN serverSockAddr;
		::memset(&serverSockAddr, 0, sizeof(serverSockAddr));
		serverSockAddr.sin_family = PF_INET;
		serverSockAddr.sin_addr.S_un.S_addr = ::inet_addr(argv[1]);
		serverSockAddr.sin_port = ::htons(::atoi(argv[2]));

		// 3.2. 서버 소켓 연결
		if (::connect(serverSock, (SOCKADDR*)&serverSockAddr, sizeof(serverSockAddr)) == SOCKET_ERROR)
		{
			utils::error::ErrorHandling(std::format("[E] connect: {}", ::WSAGetLastError()));
		}

		while (true)
		{
			// 1. 피연산자 개수 입력
			int operandCount = 0;
			std::cout << "[I] Operand count: ";
			std::cin >> operandCount;

			// 종료 명령어
			if (operandCount < 2)
			{
				::send(serverSock, reinterpret_cast<char*>(&operandCount), sizeof(char), 0);
				break;
			}

			// 2. 버퍼의 크기 계산
			// [1 BYTE]        [4 BYTE * n] [1 BYTE]
			// [operand count] [operand...] [operator]
			int bufferSize = 1 + (operandCount * SIZEOF_OPERAND) + 1;

			// 3. 버퍼 할당
			std::unique_ptr<char[]> command = std::make_unique<char[]>(bufferSize);
			command[0] = (char)operandCount;

			// 4. 피연산자 입력
			for (int i = 0; i < operandCount; i++)
			{
				int currentIndex = (i * SIZEOF_OPERAND) + 1;
				int operand = 0;
				
				std::cout << "[I] Operand: ";
				std::cin >> *reinterpret_cast<int*>(&command[currentIndex]);
			}

			// 5. 연산자 입력
			std::cout << "[I] Operator: ";
			std::cin >> command[bufferSize - 1];

			// 6. 버퍼 전송
			if (::send(serverSock, command.get(), bufferSize, 0) == SOCKET_ERROR)
			{
				utils::error::ErrorHandling(std::format("[E] send: {}", ::WSAGetLastError()));
			}

			// 7. 결과 받기
			int result = 0;
			if (::recv(serverSock, (char*)&result, SIZEOF_OPERAND, 0) == SOCKET_ERROR)
			{
				utils::error::ErrorHandling(std::format("[E] recv: {}", ::WSAGetLastError()));
			}

			std::cout << "[O] result: " << result << std::endl;
		}

		::closesocket(serverSock);
		::WSACleanup();
	}
}