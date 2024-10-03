#pragma once
#include "global.h"

namespace park18::chapter1
{
	class client
	{
	public:
		client(int argc, char* argv[]);
		~client();

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