#ifndef __ES_READER_H__
#define __ES_READER_H__

#include "socket.h"
#include <stddef.h>

namespace es {

/**
 * 接続ソケットからバイト列を読み込んでバッファリングするためのクラス。
 */
class Reader {
  public:
    /** メッセージヘッダーのサイズ */
    static const size_t kMsgHeaderSize = 4UL;
    /** メッセージボディの最大サイズ */
    static const size_t kMaxMsgBodySize = 256UL;
    /** メッセージの最大サイズ */
    static const size_t kMaxMsgSize = kMsgHeaderSize + kMaxMsgBodySize;

    Reader(Socket* sock);
    ~Reader();

    /**
     * 接続ソケットからバイト列を読み込んでバッファに保存する。
     * @returns 読み込みに成功したら`false`、エラーが発生したら`true`。
     * @note
     * メッセージ（ヘッダーとボディ）の読み込みが完了するか、エラーが発生するまで処理をブロックします。
     */
    bool Read();

    /**
     * バッファに保存したバイト列をコピーする。
     * @returns コピーに成功したら`false`、エラーが発生したら`false`。
     * @note
     * バッファはメッセージを表すバイト列で満たされているとは限りません。メッセージボディの実際のサイズを知りたい場合はヘッダーから取得してください。
     */
    bool Copy(char* dest, size_t size) const;

    size_t body_size() const;

  private:
    /** 接続ソケット */
    Socket* sock_;
    /** バイト列を保存するバッファ */
    char buf_[kMaxMsgSize];
    /** メッセージボディのサイズ */
    size_t body_size_;
};

} // namespace es

#endif // #ifndef __ES_READER_H__
