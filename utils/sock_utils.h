#pragma once
#include <WinSock2.h>

#include <format>

namespace park18::utils::sock
{
	int EnableSock(WSADATA& wsaData)
	{
		return WSAStartup(MAKEWORD(2, 2), &wsaData);
	}

	void DisableSock()
	{
		WSACleanup();
	}
}