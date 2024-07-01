
#include "pch.h"
#include "framework.h"
#include "main.h"

#include "./src/chapter1/entry.h"
#include "./src/chapter4/entry.h"
#include "./src/chapter5/calc-client.h"
#include "./src/chapter6/udp_sock.h"

int main(int argc, char* argv[])
{
    park18::chapter6::EchoClient(argc, argv);
    //park18::chapter6::ConnectEchoClient(argc, argv);
}
