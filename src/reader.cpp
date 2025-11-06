#include "reader.h"

#include <errno.h>
#include <iostream>
#include <memory.h>
#include <sys/socket.h>

namespace es {

Reader::Reader(Socket* sock) : sock_(sock), buf_({}) {}

Reader::~Reader() {}

bool Reader::Read() {
    // メッセージヘッダー（4バイト）を読み込む
    char header_buf[kMsgHeaderSize];
    ssize_t read_len = 0;
    while (read_len < sizeof(header_buf)) {
        ssize_t len = recv((*sock_)(), &header_buf[read_len],
                           sizeof(header_buf) - read_len, 0);
        if (len == -1) {
            if (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK) {
                continue;
            }
            std::cerr << "error: failed to read message header" << std::endl;
            return true;
        } else if (len == 0) {
            std::cerr << "error: connection closed" << std::endl;
            return true;
        }
        read_len += len;
    }

    // メッセージヘッダーからメッセージボディのサイズを取得
    size_t body_size = 0;
    memcpy(&body_size, header_buf, kMsgHeaderSize);
    if (body_size > kMaxMsgBodySize) {
        std::cerr << "error: message body is too large. please make it fit "
                     "within 256 bytes"
                  << std::endl;
        return true;
    }

    // メッセージボディを読み込む
    char body_buf[body_size];
    read_len = 0;
    while (read_len < body_size) {
        ssize_t len =
            recv((*sock_)(), &body_buf[read_len], body_size - read_len, 0);
        if (len == -1) {
            if (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK) {
                continue;
            }
            std::cerr << "error: failed to read message body" << std::endl;
            return true;
        } else if (len == 0) {
            std::cerr << "error: connection closed" << std::endl;
            return true;
        }
        read_len += len;
    }

    // メッセージヘッダーとメッセージボディをバッファに保存
    memcpy(buf_, &body_size, kMsgHeaderSize);
    memcpy(buf_ + kMsgHeaderSize, body_buf, body_size);
    body_size_ = body_size;

    return false;
}

bool Reader::Copy(char* dest, size_t size) const {
    if (size > kMaxMsgSize) {
        return true;
    }
    memcpy(dest, buf_, size);
    return false;
}

size_t Reader::body_size() const { return body_size_; }

} // namespace es
