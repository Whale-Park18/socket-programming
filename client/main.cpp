#include "src/chapter1/hello_client.h"
#include "src/chapter4/echo_client.h"
#include "src/chapter5/calc_client.h"
#include "src/chapter6/udp_client.h"
#include "src/chapter7/file_client.h"
#include "src/chapter9/ehco_client.h"
#include "src/chapter13/oob_client.h"

int main(int argc, char* argv[])
{
    park18::utils::network::WSAObject wsaObject;

    //park18::chapter1::client client(argc, argv);
    //park18::chapter4::echo_client client(argc, argv);
    //park18::chapter5::calc_client client(argc, argv);
    //park18::chapter6::udp_client client(argc, argv);
    //park18::chapter6::udp_connect_client client(argc, argv);
    //park18::chapter7::file_client client(argc, argv);
    park18::chapter13::oob_client client(argc, argv);

    client.start();
}
