#pragma
#include "global.h"

#include <format>
#include <memory>

namespace park18::chapter5
{
	constexpr int SIZEOF_OPERATOR = 1;
	constexpr int SIZEOF_OPERAND = 4;

	/// <summary>
	/// 계산기 서버 시작 메소드
	/// </summary>
	/// <param name="argc">애플리케이션 인자 개수</param>
	/// <param name="argv">애플리케이션 인자</param>
	void calc_server(int argc, char* argv[])
	{
		if (argc != 2)
		{
			utils::error::error_handling(std::format("[E] {} Argument: Port", argv[0]));
		}

		std::cout << "Service Start" << std::endl;

		// 1. 소켓 기능 활성화
		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		{
			utils::error::error_handling(std::format("[E] WSAStartup: {}", ::GetLastError()));
		}

		// 2. Listen 소켓 생성
		SOCKET listenSock = ::socket(AF_INET, SOCK_STREAM, 0);
		if (listenSock == INVALID_SOCKET)
		{
			utils::error::error_handling(std::format("[E] socket: {}", ::WSAGetLastError()));
		}

		// 3.1. Listen 소켓 주소 할당
		SOCKADDR_IN listenSockAddr;
		::memset(&listenSockAddr, 0, sizeof(listenSockAddr));
		listenSockAddr.sin_family = PF_INET;
		listenSockAddr.sin_addr.S_un.S_addr = ::htonl(INADDR_ANY);
		listenSockAddr.sin_port = ::htons(::atoi(argv[1]));

		// 3.2. Listen 소켓 주소 할당
		if (::bind(listenSock, (SOCKADDR*)&listenSockAddr, sizeof(listenSockAddr)) == SOCKET_ERROR)
		{
			utils::error::error_handling(std::format("[E] bind: {}", ::WSAGetLastError()));
		}

		// 4. 요청 가능 상태
		if (::listen(listenSock, 5) == SOCKET_ERROR)
		{
			utils::error::error_handling(std::format("[E] listen: {}", ::WSAGetLastError()));
		}

		// 요청 소켓 생성
		SOCKET acceptSock;
		SOCKADDR_IN acceptSockAddr;
		int acceptSockAddrSize = sizeof(acceptSockAddr);

		// 5. 요청 응답
		acceptSock = ::accept(listenSock, (SOCKADDR*)&acceptSockAddr, &acceptSockAddrSize);
		if (acceptSock == INVALID_SOCKET)
		{
			utils::error::error_handling(std::format("[E] accept: {}", ::WSAGetLastError()));
		}

		std::string hostIP = ::inet_ntoa(acceptSockAddr.sin_addr);
		types::ushort hostPort = ::ntohs(acceptSockAddr.sin_port);
		std::cout << std::format("connect info IP: {}, Port: {}", hostIP, hostPort) << std::endl;

 		while (true)
		{
			// 피연산자 개수 받기
			types::uchar operandCount = 0;
			if ((::recv(acceptSock, (char*)&operandCount, SIZEOF_OPERATOR, 0)) == SOCKET_ERROR)
			{
				utils::error::error_handling(std::format("[E] recv - operandCount: {}", ::WSAGetLastError()));
			}
			std::cout << std::format("[I] Operand count: {}", operandCount) << std::endl;

			// 종료 명령어
			if (operandCount < 2)
			{
				break;
			}

			// 버퍼 할당 및 계산 프로토콜 받기
			int commandSize = 0;
			std::unique_ptr<char[]> command = std::make_unique<char[]>((operandCount * SIZEOF_OPERAND) + 1);
			do
			{
				commandSize += ::recv(acceptSock, (command.get() + commandSize), SIZEOF_OPERAND, 0);
				if (commandSize == SOCKET_ERROR)
				{
					utils::error::error_handling(std::format("[E] recv - commandSize: {}", ::WSAGetLastError()));
				}

			} while (commandSize < (operandCount * SIZEOF_OPERAND) + 1);

			int result = *(int*)&command[0];
			for (int i = 1; i < operandCount; i++)
			{
				int currentIndex = (i * SIZEOF_OPERAND);
				//int num = *(int*)&command[currentIndex];
				int num = *reinterpret_cast<int*>(&command[currentIndex]);
				
				switch (command[(operandCount * SIZEOF_OPERAND)])
				{
				case '+':
					result += num;
					break;
				
				case '-':
					result -= num;
					break;
				
				case '*':
					result *= num;
					break;
				
				default:
					utils::error::error_handling(std::format("[E] Wong operator"));
					break;
				}

			}
			std::cout << std::format("[O] result: {}", result) << std::endl;
			
			if (::send(acceptSock, (char*)&result, SIZEOF_OPERAND, 0) == SOCKET_ERROR)
			{
				utils::error::error_handling(std::format("[E] send: {}", ::WSAGetLastError()));
			}
		}

		::closesocket(acceptSock);
		::closesocket(listenSock);
		::WSACleanup();
	}
}