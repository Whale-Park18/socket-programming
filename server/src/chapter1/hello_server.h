#pragma once
#include "global.h"

namespace park18::chapter1
{
	class server
	{
	public:
		server(int argc, char* argv[]);
		~server();

		void start();

	private:
		void init(int argc, char* argv[]);

		void ready();

		WSADATA wsaData;	 // WinSock
		unsigned short port; // 서버 포트

		SOCKET listenSock;	 // listen 소켓
		SOCKET acceptSock;	 // listen 소켓에 연결 요청한 클라이언트 소켓
	};
}
