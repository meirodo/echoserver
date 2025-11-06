#include "writer.h"

#include <errno.h>
#include <iostream>
#include <sys/socket.h>

namespace es {

Writer::Writer(Socket* sock) : sock_(sock) {}

Writer::~Writer() {}

bool Writer::Write(const char* buf, size_t size) {
    size_t write_len = 0;
    while (write_len < size) {
        ssize_t len = send((*sock_)(), buf, size - write_len, 0);
        if (len == -1) {
            if (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK) {
                continue;
            }
            std::cerr << "error: failed to write message" << std::endl;
            return true;
        } else if (len == 0) {
            std::cerr << "error: connection closed" << std::endl;
            return true;
        }
        buf += len;
        write_len = (size_t)len;
    }
    return false;
}

} // namespace es
