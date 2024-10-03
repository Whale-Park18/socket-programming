#pragma once
#include "global.h"

namespace park18::chapter9
{
	class socket_option_echo_server
	{
	public:
		socket_option_echo_server(int argc, char* argv[]);
		~socket_option_echo_server();

		void start();

	private:
		void init(int argc, char* argv[]);

		void ready();

		void print_accept_info(const sockaddr_in& acceptAddress);

		WSADATA wsaData;
		unsigned short port;

		SOCKET listenSock;	 // listen 소켓
		SOCKET acceptSock;	 // listen 소켓에 연결 요청한 클라이언트 소켓

		char buf[512];		 // 메시지 수신 버퍼
		int recvCount;		 // 메시지 수신 길이
	};
}