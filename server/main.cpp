#pragma
#include "main.h"

#include "./src/chapter3/entry.h"
#include "./src/chapter4/entry.h"
#include "./src/chapter5/calc-server.h"
#include "./src/chapter6/udp_sock.h"

int main(int argc, char* argv[])
{
	park18::chapter6::EchoServer(argc, argv);
	//park18::chapter6::ConnectEchoServer(argc, argv);
}
