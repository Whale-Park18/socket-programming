#pragma
#include "global.h"

#include <format>
#include <memory>

namespace park18::chapter5
{
	constexpr int SIZEOF_OPERATOR = 1;
	constexpr int SIZEOF_OPERAND = 4;

	class calc_server
	{
	public:
		calc_server(int argc, char* argv[]);
		~calc_server();

		void start();

	private:
		void init(int argc, char* argv[]);

		void ready();

		int calc(char* command, types::uchar operandCount);

		WSADATA wsaData;	 // WinSock
		unsigned short port; // 서버 포트

		SOCKET listenSock;	 // listen 소켓
		SOCKET acceptSock;	 // listen 소켓에 연결 요청한 클라이언트 소켓

		char buf[512];		 // 메시지 수신 버퍼
		int recvCount;		 // 메시지 수신 길이
	};
}