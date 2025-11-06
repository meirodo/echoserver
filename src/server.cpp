#include <errno.h>
#include <format>
#include <iostream>
#include <memory.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

#include "reader.h"
#include "socket.h"
#include "writer.h"

int main() {
    errno = 0;
    es::Socket listen_sock{AF_INET, SOCK_STREAM, 0};
    if (listen_sock() == -1) {
        std::cerr << std::format("error: errno={}, {}\n", errno, "socket()")
                  << std::endl;
        return 1;
    }

    sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = ntohl(0);
    addr.sin_port = ntohs(1234);
    errno = 0;
    if (bind(listen_sock(), (const sockaddr*)&addr, sizeof(addr))) {
        std::cerr << std::format("error: errno={}, {}\n", errno, "bind()")
                  << std::endl;
        return 1;
    }

    errno = 0;
    if (listen(listen_sock(), SOMAXCONN)) {
        std::cerr << std::format("error: errno={}, {}\n", errno, "listen()")
                  << std::endl;
        return 1;
    }

    while (true) {
        sockaddr_in client_addr = {};
        socklen_t addrlen = sizeof(client_addr);
        es::Socket conn_sock{
            accept(listen_sock(), (sockaddr*)&client_addr, &addrlen)};
        if (conn_sock() == -1) {
            std::cerr << std::format("error: errno={}, {}\n", errno,
                                     "accept()");
            continue;
        }

        es::Reader reader{&conn_sock};
        while (true) {
            if (reader.Read()) {
                break;
            }
            char buf[es::Reader::kMsgHeaderSize + reader.body_size()];
            if (reader.Copy(buf, sizeof(buf))) {
                break;
            }
            es::Writer writer{&conn_sock};
            if (writer.Write(buf, sizeof(buf))) {
                break;
            }
        }
    }

    return 0;
}
