#pragma once
#include "global.h"

namespace park18::chapter6
{
	class udp_echo_server
	{
	public:
		udp_echo_server(int argc, char* argv[]);
		~udp_echo_server();

		void start();

	private:
		void init(int argc, char* argv[]);

		void ready();

		WSADATA wsaData;   // WinSock
		unsigned port;	   // 서버 포트

		SOCKET listenSock; // listen 소켓
		SOCKET acceptSock; // listen 소켓에 연결 요청한 클라이언트 소켓

		char buf[512];	   // 메시지 수신 버퍼
		int recvCount;	   // 메시지 수신 길이
	};
}