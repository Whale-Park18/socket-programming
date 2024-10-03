#pragma once
#include "global.h"

namespace park18::chapter7
{
	class file_client
	{
	public:
		file_client(int argc, char* argv[]);
		~file_client();

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