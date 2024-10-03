#pragma once
#include "global.h"

#include <iostream>
#include <iostream>
#include <string>
#include <string_view>
#include <format>

namespace park18::chapter4
{
	constexpr unsigned int BUF_SIZE = 1024;

	class echo_server
	{
	public:
		echo_server(int argc, char* argv[]);
		~echo_server();

		void start();

	private:
		void init(int argc, char* argv[]);

		void ready();

		WSADATA wsaData;	 // WinSock
		unsigned short port; // 서버 포트

		SOCKET listenSock;	 // listen 소켓
		SOCKET acceptSock;	 // listen 소켓에 연결 요청한 클라이언트 소켓

		char buf[BUF_SIZE];		 // 메시지 수신 버퍼
		int recvCount;		 // 메시지 수신 길이
	};
}