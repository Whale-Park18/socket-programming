#include "src/chapter1/hello_server.h"
#include "src/chapter3/network_conv.h"
#include "src/chapter4/echo_server.h"
#include "src/chapter5/calc_server.h"
#include "src/chapter6/udp_echo_server.h"
#include "src/chapter7/file_server.h"
#include "src/chapter8/domain.h"
#include "src/chapter9/socket_option.h"
//		 chapter10: Linux.multi_process
//		 chapter11: Linux.IPC.pipe
#include "src/chapter12/ehco_select_sever.h"
#include "src/chapter13/oob_server.h"

int main(int argc, char* argv[])
{
	park18::utils::network::WSAObject wsaObject;

	//park18::chapter1::server server(argc, argv);
	//park18::chapter3::winsock_network_helper server;
	//park18::chapter4::echo_server server(argc, argv);
	//park18::chapter5::calc_server server(argc, argv);
	//park18::chapter6::udp_echo_server server(argc, argv);
	//park18::chapter7::file_server server(argc, argv);
	//park18::chapter8::test_domain();
	//park18::chapter9::socket_option_echo_server server(argc, argv);
	//park18::chapter12::echo_select_server server(argc, argv);
	park18::chapter13::oob_server server(argc, argv);

	server.start();
}
