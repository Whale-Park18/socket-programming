#include "pch.h"
#include "main.h"

#include "./src/chapter3/entry.h"
#include "./src/chapter4/entry.h"
#include "./src/chapter5/calc-server.h"
#include "./src/chapter6/udp_sock.h"
#include "./src/chapter7/file_server.h"
#include "./src/chapter8/entry.h"

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		park18::utils::error::ErrorHandling("[E] need Port");
	}

	//park18::chapter5::StartService(argc, argv);
	//park18::chapter6::EchoServer(argc, argv);
	//park18::chapter6::ConnectEchoServer(argc, argv);
	//park18::chapter7::FileServerStart(argc, argv);
	park18::chapter8::entry();
}
