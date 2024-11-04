#pragma once
#include "global.h"

namespace park18::chapter13
{
	class oob_server : public utils::network::basic_service
	{
	public:
		oob_server(int argc, char* argv[]);
		virtual ~oob_server() = default;
	
		void start() override;

	private:
		static constexpr unsigned int BUF_SIZE = 1024;

		void init(int argc, char* argv[]) override;

		void ready() override;

		unsigned short port;

		// ╪рдо
		SOCKET lisetnSock;
		SOCKET acceptSock;

		// select
		fd_set read;
		fd_set except;
		fd_set readCopy;
		fd_set exceptCopy;
		timeval timeout;

		int recvCount;
		char buf[BUF_SIZE];
	};
}