#include "src/chapter1/hello_client.h"
#include "src/chapter4/echo_client.h"
#include "src/chapter5/calc_client.h"
#include "src/chapter6/udp_client.h"
#include "src/chapter7/file_client.h"
#include "src/chapter9/ehco_client.h"

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        park18::utils::error::error_handling("[E] need IP, Port");
    }

    //park18::chapter1::client_start(argc, argv);
    park18::chapter4::echo_client(argc, argv);
    //park18::chapter5::calc_client(argc, argv);
    //park18::chapter6::udp_echo_client(argc, argv);
    //park18::chapter6::udp_connect_echo_client(argc, argv);
    //park18::chapter7::file_client(argc, argv);
    //park18::chapter9::echo_client(argc, argv);
}
