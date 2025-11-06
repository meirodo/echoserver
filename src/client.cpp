#include <cmath>
#include <format>
#include <iostream>
#include <memory.h>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>

#include "reader.h"
#include "socket.h"
#include "writer.h"

int main() {
    es::Socket sock{AF_INET, SOCK_STREAM, 0};
    if (sock() == -1) {
        std::cerr << std::format("error: errno={}, {}\n", errno, "socket()");
        return 1;
    }

    sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = ntohl(INADDR_LOOPBACK);
    addr.sin_port = ntohs(1234);
    if (connect(sock(), (const sockaddr*)&addr, sizeof(addr))) {
        std::cerr << std::format("error: errno={}, {}\n", errno, "connect()");
        return 1;
    }

    while (true) {
        std::string input_buf;
        std::cout << "Please enter: ";
        std::cin >> input_buf;

        // 入力したメッセージをサーバーに送信
        size_t msg_body_size = es::Reader::kMaxMsgBodySize;
        size_t input_size = std::min(input_buf.length(), msg_body_size);
        if (input_size < 1) {
            continue;
        }

        size_t write_buf_size = es::Reader::kMsgHeaderSize + input_size;
        char write_buf[write_buf_size];
        memcpy(write_buf, &input_size, es::Reader::kMsgHeaderSize);
        memcpy(write_buf + es::Reader::kMsgHeaderSize, input_buf.c_str(),
               input_size);

        es::Writer writer{&sock};
        if (writer.Write(write_buf, write_buf_size)) {
            break;
        }

        // サーバーからのレスポンスを読み込む
        es::Reader reader{&sock};
        if (reader.Read()) {
            break;
        }
        char buf[es::Reader::kMsgHeaderSize + reader.body_size()];
        if (reader.Copy(buf, sizeof(buf))) {
            break;
        }
        fprintf(stderr, "server: %.*s\n", (int)reader.body_size(),
                buf + es::Reader::kMsgHeaderSize);
    }

    return 0;
}
