#pragma once
#include "global.h"

#include <format>

namespace park18::chapter3
{
	class winsock_network_helper
	{
	public:
		winsock_network_helper();
		~winsock_network_helper();

		void start();

	private:
		void network_byte_drder();
		void init_address();
		void windows_helper();

		WSADATA wsaData;
	};
}