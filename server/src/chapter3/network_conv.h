#pragma once
#include "global.h"

#include <format>

namespace park18::chapter3
{
	void network_byte_drder();
	void init_address();
	void windows_helper();

	void test_conv()
	{
		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		{
			utils::error::ErrorHandling("WSAStartup() error");
		}

		std::cout << "[CPU: AMD]" << std::endl;

		std::cout << "========== NetworkByhteOrder ==========" << std::endl;
		network_byte_drder();
		std::cout << "========== NetworkByhteOrder ==========\n" << std::endl;

		std::cout << "========== InitAddress ==========" << std::endl;
		init_address();
		std::cout << "========== InitAddress ==========\n" << std::endl;

		std::cout << "========== WindowsHelper ==========" << std::endl;
		windows_helper();
		std::cout << "========== WindowsHelper ==========\n" << std::endl;

		WSACleanup();
	}

	void network_byte_drder()
	{
		types::ushort hostPort = 0x1234;
		types::ushort netPort = 0;

		types::ulong hostAddress = 0x123456789;
		types::ulong netAddress = 0;

		netPort = ::htons(hostPort);
		netAddress = htonl(hostAddress);

		std::cout << std::showbase << std::hex 
			<< "hostPort: " << std::showbase << std::hex << hostPort << '\n'
			<< "netPort: " << std::hex << netPort << '\n'
			<< "hosAddr: " << std::hex << hostAddress << '\n'
			<< "netAddr: " << std::hex << netAddress << std::endl;
	}

	void init_address()
	{
		std::string address{ "210.220.163.82" };
		types::ulong conversionAddress = ::inet_addr(address.c_str());
		if (conversionAddress == INADDR_NONE)
		{
			utils::error::ErrorHandling("Error INADDR_NONE");
		}

		std::cout << std::showbase << std::hex
			<< "NetworkByteOrder Address: " << conversionAddress << std::endl;
	}

	void windows_helper()
	{
		char address[] = "210.220.163.82";
		SOCKADDR_IN serverAddress;
		int size = sizeof(serverAddress);

		auto ret = ::WSAStringToAddressA(address, AF_INET, NULL, (SOCKADDR*)&serverAddress, &size);
		if (ret != 0)
		{
			utils::error::ErrorHandling(std::format("[E] WSAStringToAddress: {} {}", ::GetLastError(), size));
		}

		char buffer[MAX_PATH] = { 0, };
		size = sizeof(buffer);
		
		ret = ::WSAAddressToStringA((SOCKADDR*)&serverAddress, sizeof(serverAddress), NULL, buffer, (DWORD*)&size);
		if (ret != 0)
		{
			utils::error::ErrorHandling(std::format("[E] WSAAddressToString: {} {}", ::GetLastError(), size));
		}

		std::cout << std::showbase << std::hex
			<< "host to network: " << serverAddress.sin_addr.S_un.S_addr << '\n'
			<< "network to host: " << buffer << std::endl;
	}
}