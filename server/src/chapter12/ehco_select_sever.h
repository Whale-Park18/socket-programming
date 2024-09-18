#pragma once
#include "global.h"

namespace park18::chapter12
{
	class echo_select_server
	{
	public:
		echo_select_server(int argc, char* argv[]);
		~echo_select_server();

		void start();

	private:
		void init(int argc, char* argv[]);

		void ready();

		void set_timeout();

		WSADATA wsaData;	 // WinSock
		unsigned short port; // 서버 포트

		SOCKET listenSock;	 // listen 소켓
		SOCKET acceptSock;	 // listen 소켓에 연결 요청한 클라이언트 소켓

		fd_set read;		 // 원본 fd_set(Read)
		fd_set copyRead;	 // 원본 fd_set의 복사본

		timeval timeout;	 // select 타임 아웃 구조체

		char buf[512];		 // 메시지 수신 버퍼
		int recvCount;		 // 메시지 수신 길이
	};
}