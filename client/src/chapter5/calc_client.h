#pragma once
#include "global.h"

#include <vector>
#include <memory>

namespace park18::chapter5
{
	constexpr int SIZEOF_OPERAND = 4;

	class calc_client
	{
	public:
		calc_client(int argc, char* argv[]);
		~calc_client();

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