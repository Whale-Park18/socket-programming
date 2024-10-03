#include "calc_client.h"

park18::chapter5::calc_client::calc_client(int argc, char* argv[])
{
	init(argc, argv);
}

park18::chapter5::calc_client::~calc_client()
{
	closesocket(this->connectSock);
	WSACleanup();
}

void park18::chapter5::calc_client::start()
{
	ready();

	while (true)
	{
		// 1. 피연산자 개수 입력
		int operandCount = 0;
		std::cout << "[I] Operand count: ";
		std::cin >> operandCount;

		// 종료 명령어
		if (operandCount < 2)
		{
			::send(this->connectSock, reinterpret_cast<char*>(&operandCount), sizeof(char), 0);
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
		if (::send(this->connectSock, command.get(), bufferSize, 0) == SOCKET_ERROR)
		{
			utils::error::error_handling(std::format("[E] send: {}", ::WSAGetLastError()));
		}

		// 7. 결과 받기
		int result = 0;
		if (::recv(this->connectSock, (char*)&result, SIZEOF_OPERAND, 0) == SOCKET_ERROR)
		{
			utils::error::error_handling(std::format("[E] recv: {}", ::WSAGetLastError()));
		}

		std::cout << "[O] result: " << result << std::endl;
	}
}

void park18::chapter5::calc_client::init(int argc, char* argv[])
{
	if (argc != 3)
	{
		utils::error::error_handling(std::format("Usage: {} <IP> <port>", argv[0]));
	}
	this->host = argv[1];
	this->port = std::atoi(argv[2]);

	// 소켓 기능 활성화
	WSADATA wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		utils::error::error_handling("WSAStartup() error");
	}
}

void park18::chapter5::calc_client::ready()
{
	// 소켓 생성
	this->connectSock = ::socket(PF_INET, SOCK_STREAM, 0);
	if (this->connectSock == INVALID_SOCKET)
	{
		utils::error::error_handling("socket() error");
	}

	SOCKADDR_IN serverAddress = { 0, };
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.S_un.S_addr = ::inet_addr(this->host.c_str());
	serverAddress.sin_port = htons(this->port);

	// 연결 요청
	if (::connect(this->connectSock, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
	{
		utils::error::error_handling("connect() error");
	}
}
