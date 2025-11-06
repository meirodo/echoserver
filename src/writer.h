#ifndef __ES_WRITER_H__
#define __ES_WRITER_H__

#include "socket.h"
#include <stddef.h>

namespace es {

/**
 * 接続ソケットにバイト列を書き込むためのクラス。
 */
class Writer {
  public:
    Writer(Socket* sock);
    ~Writer();

    /**
     * 渡されたバッファをソケットに書き込む。
     */
    bool Write(const char* buf, size_t size);

  private:
    /** 接続ソケット */
    Socket* sock_;
};

} // namespace es

#endif // #ifndef __ES_WRITER_H__
