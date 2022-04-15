#ifndef COMMON_H_
#define COMMON_H_

#include <cstdint>
#include <cstring>
#include <errno.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

constexpr int MAX_BUFF_SIZE = 64;
constexpr unsigned short DEFAULT_PORT = 8888;

using socket_t = int;

#define zeromem(p, n) std::memset ((p), 0, (n))
#define zeromemobj(o) zeromem (&(o), sizeof (o))
#define UNLIKELY(cond) __builtin_expect (cond, 0)
#define LIKELY(cond) __builtin_expect (cond, 1)

[[noreturn]] void sys_err (const char *msg);

socket_t Socket (int domain, int type, int protocol);
void Bind (socket_t sock, const struct sockaddr *addr, socklen_t addrlen);
int Recvfrom (socket_t sock, void *__restrict__ buf, ::size_t n, int flags,
              struct ::sockaddr *__restrict__ addr,
              socklen_t *__restrict__ addr_len);
int Sendto (socket_t sock, const void *buf, ::size_t n, int flags,
            const struct ::sockaddr *addr, socklen_t addr_len);
void Inet_pton (int af, const char *__restrict__ cp, void *__restrict__ buf);
void Setsockopt (socket_t sock, int level, int optname, const void *optval,
                 socklen_t optlen);

static constexpr std::uint32_t BOM
    = ((std::uint32_t)'E' << 24) | ((std::uint32_t)'C' << 16)
      | ((std::uint32_t)'H' << 8) | ((std::uint32_t)'O' << 0);

static constexpr std::uint32_t REVERSE_BOM
    = ((std::uint32_t)'O' << 24) | ((std::uint32_t)'H' << 16)
      | ((std::uint32_t)'C' << 8) | ((std::uint32_t)'E' << 0);

constexpr int VERSION_LEN = 4;
constexpr int BOM_LEN = sizeof (BOM);
constexpr int SEQ_BEGIN_POS = BOM_LEN + VERSION_LEN;
constexpr int SEQ_LEN = 8u;
constexpr int MIN_LEN = sizeof (BOM) + VERSION_LEN + SEQ_LEN;
static_assert (MAX_BUFF_SIZE > MIN_LEN, "Buffer size is too small!");

#endif // !COMMON_H_
