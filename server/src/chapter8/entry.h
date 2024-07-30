#pragma once
#include "pch.h"

#include <format>
#include <string>

#include <WinSock2.h>
#include <WS2tcpip.h>

namespace park18::chapter8
{
	void PrintDomainToIP(char* domain, char* service)
	{
		if (std::strlen(domain) == 0)
		{
			std::cerr << "[E] domain: nullptr" << std::endl;
			return;
		}

		addrinfo hints = { 0 };
		hints.ai_family = PF_INET;
		hints.ai_socktype = SOCK_STREAM;

		addrinfo* res = nullptr;
		if (getaddrinfo(domain, service, &hints, &res) != 0)
		{
			std::cerr << "[E] getaddrinfo " << WSAGetLastError() << std::endl;
			return;
		}

		std::cout << "[" << domain << "]" << '\n';
		for (addrinfo* pVisit = res; pVisit != nullptr; pVisit = pVisit->ai_next)
		{
			sockaddr_in* pAddr = reinterpret_cast<sockaddr_in*>(pVisit->ai_addr);
			std::string canonname = pVisit->ai_canonname != nullptr ? pVisit->ai_canonname : "";

			std::cout << "-----------------------------------------" << '\n';
			std::cout << "flags    : " << pVisit->ai_flags			 << '\n';
			std::cout << "family   : " << pVisit->ai_family			 << '\n';
			std::cout << "socktype : " << pVisit->ai_socktype		 << '\n';
			std::cout << "protocol : " << pVisit->ai_protocol		 << '\n';
			std::cout << "addrlen  : " << pVisit->ai_addrlen		 << '\n';
			std::cout << "addr     : " << inet_ntoa(pAddr->sin_addr) << '\n';
			std::cout << "canonname: " << canonname					 << '\n';
		}
		std::cout << std::endl;

		freeaddrinfo(res);
	}

	void PrintIPToDomain(char* ip, short port = 0)
	{
		if (std::strlen(ip) == 0)
		{
			std::cerr << "[E] ip: nullptr" << std::endl;
			return;
		}

		sockaddr_in addr = { 0 };
		addr.sin_family			  = PF_INET;
		addr.sin_addr.S_un.S_addr = inet_addr(ip);
		addr.sin_port			  = port;

		char host[1024] = { 0 };
		char service[1024] = { 0 };
		if (getnameinfo(reinterpret_cast<sockaddr*>(&addr), sizeof(addr), host, 1024, service, 1024, NULL) != 0)
		{
			std::cerr << "[E] getnameinfo " << WSAGetLastError() << std::endl;
			return;
		}

		std::cout << '[' << ip << ']'							 << '\n';
		std::cout << "-----------------------------------------" << '\n';
		std::cout << "host    :" << host						 << '\n';
		std::cout << "service :" << service						 << '\n';
		std::cout << std::endl;
	}

	void entry()
	{
		WSADATA wsaData = { 0 };
		WSAStartup(MAKEWORD(2, 2), &wsaData);

		PrintDomainToIP((char*)"www.naver.com", nullptr);
		PrintDomainToIP((char*)"www.naver.com", (char*)"https");
		PrintDomainToIP((char*)"naver.com", nullptr);
		PrintDomainToIP((char*)"google.com", nullptr);

		PrintIPToDomain((char*)"223.130.200.236");
		PrintIPToDomain((char*)"223.130.200.219");
		PrintIPToDomain((char*)"223.130.192.247");
		PrintIPToDomain((char*)"223.130.192.248");
		PrintIPToDomain((char*)"142.250.206.238");

		WSACleanup();
	}
}