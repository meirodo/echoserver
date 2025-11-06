#ifndef __ES_SOCKET_H__
#define __ES_SOCKET_H__

namespace es {

/**
 * ソケットを管理するクラス。
 */
class Socket {
  public:
    Socket(int domain, int type, int protocol);
    Socket(int sock);
    ~Socket();

    int sock() const;

    int operator()() const;

    Socket& operator=(const Socket& other);

  private:
    int sock_;
};

} // namespace es

#endif // #ifndef __ES_SOCKET_H__
