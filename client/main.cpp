
#include "pch.h"
#include "main.h"

#include "./src/chapter1/entry.h"
#include "./src/chapter4/entry.h"
#include "./src/chapter5/calc-client.h"
#include "./src/chapter6/udp_sock.h"
#include "./src/chapter7/file_client.h"
#include "./src/chapter9/ehco_client.h"

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        park18::utils::error::ErrorHandling("[E] need IP, Port");
    }

    //park18::chapter6::EchoClient(argc, argv);
    //park18::chapter6::ConnectEchoClient(argc, argv);
    //park18::chapter7::FileClientStart(argc, argv);
    park18::chapter9::StartClient(argc, argv);
}
