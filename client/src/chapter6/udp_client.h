#pragma once

namespace park18::chapter6
{
	class udp_client
	{
	public:
		udp_client(int argc, char* argv[]);
		virtual ~udp_client();

		virtual void start();

	protected:
		void init(int argc, char* argv[]);

		virtual void ready();

		WSADATA wsaData;
		std::string host;
		unsigned short port;

		SOCKET connectSock;

		sockaddr_in connectAddress;
		int connectAddressSize;
	};

	class udp_connect_client : public udp_client
	{
	public:
		udp_connect_client(int argc, char* argv[]);
		virtual ~udp_connect_client() = default;

		virtual void start() override;

	private:
		virtual void ready() override;
	};
}