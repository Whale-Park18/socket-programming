#pragma once
#include "global.h"

namespace park18::chapter4
{
	constexpr unsigned int BUF_SIZE = 1024;

	class echo_client
	{
	public:
		echo_client(int argc, char* argv[]);
		~echo_client();

		void start();

	private:
		void init(int argc, char* argv[]);

		void ready();

		WSADATA wsaData;
		std::string host;
		unsigned short port;

		SOCKET connectSock;
	};
}