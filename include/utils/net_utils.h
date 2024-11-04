#pragma once

#include "error.h"

#include <format>

#include <WinSock2.h>

namespace park18::utils::network
{
	/// <summary>
	/// Winsock 설정, 해제 RAII 객체
	/// </summary>
	class WSAObject
	{
	public:
		WSAObject()
		{
			WSADATA wsaData = { 0 };
			if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
			{
				error::error_handling(std::format("[E] utils.network.WSAObject() - code: {}", WSAGetLastError()));
			}
		}

		virtual ~WSAObject()
		{
			WSACleanup();
		}
	};

	/// <summary>
	/// 서비스(서버, 클라이언트) 기본 클래스
	/// </summary>
	class basic_service
	{
	public:
		basic_service() = default;
		//basic_service(int argc, char* argv[]) { init(argc, argv); }
		virtual ~basic_service() = default;

		/// <summary>
		/// 서비스 실행
		/// </summary>
		virtual void start() = 0;

	protected:
		
		/// <summary>
		/// 프로그램이 입력받은 정보를 초기화하는 메소드
		/// </summary>
		virtual void init(int argc, char* argv[]) = 0;

		/// <summary>
		/// 서비스가 실행되기 위해 소켓을 준비하는 메소드
		/// </summary>
		virtual void ready() = 0;
	};
}