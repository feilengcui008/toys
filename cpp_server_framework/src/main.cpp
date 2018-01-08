#include <string>
#include <iostream>

#include "ExampleServer.h"

int main(int argc, char *argv[]) {

    if (argc < 2) {
        // fprintf(stdout, "%s\n", "usage : ./example_server epoll[thread]");
        fprintf(stdout, "%s\n", "usage : ./example_server conf.json");
        return 0;
    }
    std::string conf(argv[1]);
    // std::string mode(argv[1]);
    // std::cout << mode << std::endl;
    ExampleServer server("localhost", 8000, 10);
    server.parseConf(conf);
    server.printConf();
    /*
       if(mode == "epoll") {
       server.setMode(EPOLL);
       } else {
       server.setMode(THREAD);
       }
       */
    // server.setKeepAlived(false);
    server.start();
    return 0;
}
