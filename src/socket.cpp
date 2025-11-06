#include "socket.h"

#include <sys/socket.h>
#include <unistd.h>

#include <iostream>

namespace es {

Socket::Socket(int domain, int type, int protocol) {
    sock_ = socket(domain, type, protocol);
}

Socket::Socket(int sock) : sock_(sock) {}

Socket::~Socket() {
    if (sock_ != -1) {
        close(sock_);
        sock_ = 0;
    }
}

int Socket::sock() const { return sock_; }

int Socket::operator()() const { return sock_; }

} // namespace es
