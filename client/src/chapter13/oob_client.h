#pragma once
#include "global.h"

namespace park18::chapter13
{
	class oob_client : public utils::network::basic_service
	{
	public:
		oob_client(int argc, char* argv[]);
		virtual ~oob_client() = default;

		void start() override;
	private:

		void init(int argc, char* argv[]) override;

		void ready() override;

		// ȣ��Ʈ ����
		std::string host;
		unsigned short port;

		// ����
		SOCKET connectSock;
	};
}