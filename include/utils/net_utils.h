#pragma once

#include "error.h"

#include <format>

#include <WinSock2.h>

namespace park18::utils::network
{
	/// <summary>
	/// Winsock ����, ���� RAII ��ü
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
	/// ����(����, Ŭ���̾�Ʈ) �⺻ Ŭ����
	/// </summary>
	class basic_service
	{
	public:
		basic_service() = default;
		//basic_service(int argc, char* argv[]) { init(argc, argv); }
		virtual ~basic_service() = default;

		/// <summary>
		/// ���� ����
		/// </summary>
		virtual void start() = 0;

	protected:
		
		/// <summary>
		/// ���α׷��� �Է¹��� ������ �ʱ�ȭ�ϴ� �޼ҵ�
		/// </summary>
		virtual void init(int argc, char* argv[]) = 0;

		/// <summary>
		/// ���񽺰� ����Ǳ� ���� ������ �غ��ϴ� �޼ҵ�
		/// </summary>
		virtual void ready() = 0;
	};
}