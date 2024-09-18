#pragma once

#include "error.h"

#include <format>

#include <WinSock2.h>

namespace park18::utils::network
{
	inline void net_startup(WSAData& wsaData)
	{
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		{
			error::error_handling(std::format("[E] util.network.net_startup - code: {}", WSAGetLastError()));
		}
	}

	inline void net_cleanup()
	{
		WSACleanup();
	}
}