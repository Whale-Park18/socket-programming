#include "calc_server.h"

park18::chapter5::calc_server::calc_server(int argc, char* argv[])
{
	init(argc, argv);
}

park18::chapter5::calc_server::~calc_server()
{
	closesocket(acceptSock);
	closesocket(listenSock);
	WSACleanup();
}

void park18::chapter5::calc_server::start()
{
	ready();

	sockaddr_in acceptAddress = { 0 };
	int acceptAddressSize = sizeof(acceptAddress);
	this->acceptSock = accept(this->listenSock, reinterpret_cast<sockaddr*>(&acceptAddress), &acceptAddressSize);
	if (this->acceptSock == INVALID_SOCKET)
	{
		utils::error::error_handling(std::format("[E] accept - error: {}", ::WSAGetLastError()));
	}

	std::string hostIP = ::inet_ntoa(acceptAddress.sin_addr);
	types::ushort hostPort = ::ntohs(acceptAddress.sin_port);
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
		
		int result = calc(command.get(), operandCount);
		std::cout << std::format("[O] result: {}", result) << std::endl;

		if (::send(acceptSock, (char*)&result, SIZEOF_OPERAND, 0) == SOCKET_ERROR)
		{
			utils::error::error_handling(std::format("[E] send: {}", ::WSAGetLastError()));
		}
	}
}

void park18::chapter5::calc_server::init(int argc, char* argv[])
{
	// 프로그램 인자 예외 처리
	if (argc != 2)
	{
		utils::error::error_handling("[E] calc_server.init - the number of arguments is wrong");
	}
	this->port = static_cast<unsigned short>(std::stoi(argv[1]));

	// Win sock 예외 처리
	SecureZeroMemory(&this->wsaData, sizeof(this->wsaData));
	if (WSAStartup(MAKEWORD(2, 2), &this->wsaData) != 0)
	{
		utils::error::error_handling("[E] calc_server.init - WSADATA initialization falied");
	}
}

void park18::chapter5::calc_server::ready()
{
	this->listenSock = socket(AF_INET, SOCK_STREAM, 0);
	if (this->listenSock == INVALID_SOCKET)
	{
		utils::error::error_handling(std::format("[E] socket - error: {}", ::WSAGetLastError()));
	}

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
}

int park18::chapter5::calc_server::calc(char* command, types::uchar operandCount)
{
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

	return result;
}
