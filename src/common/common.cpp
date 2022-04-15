#include "common.h"
#include <arpa/inet.h>
#include <cstdio>
#include <cstdlib>

[[noreturn]] void
sys_err (const char *msg)
{
  std::perror (msg);
  std::exit (errno);
}

socket_t
Socket (int domain, int type, int protocol)
{
  socket_t sock = ::socket (domain, type, protocol);
  if (sock == -1)
    {
      ::sys_err ("Create socket failed!");
    }
  return sock;
}

void
Bind (socket_t sock, const struct sockaddr *addr, socklen_t addrlen)
{
  auto ret = ::bind (sock, addr, addrlen);
  if (ret != 0)
    {
      close (sock);
      ::sys_err ("Bind failed!");
    }
}

int
Recvfrom (socket_t sock, void *__restrict__ buf, ::size_t n, int flags,
          struct ::sockaddr *__restrict__ addr,
          socklen_t *__restrict__ addr_len)
{
  ::ssize_t ret = ::recvfrom (sock, buf, n, flags, addr, addr_len);
  if (ret < 0)
    {
      sys_err ("Receive error!");
    }
  return ret;
}

int
Sendto (socket_t sock, const void *buf, ::size_t n, int flags,
        const struct ::sockaddr *addr, socklen_t addr_len)
{
  auto ret = ::sendto (sock, buf, n, flags, addr, addr_len);
  if (ret == -1)
    {
      sys_err ("Fail to send a message!");
    }
  return ret;
}

void
Inet_pton (int af, const char *__restrict__ cp, void *__restrict__ buf)
{
  auto ret = ::inet_pton (af, cp, buf);
  if (ret == 1)
    {
      // Success
      return;
    }
  else if (ret == 0)
    {
      std::printf ("The IP address specified is invalid!\n");
      exit (1);
    }
  else
    {
      sys_err ("IP address convertion failed!");
    }
}

void
Setsockopt (socket_t sock, int level, int optname, const void *optval,
            socklen_t optlen)
{
  auto ret = ::setsockopt (sock, level, optname, optval, optlen);
  if (ret == -1)
    {
      sys_err ("Setsockopt faild!");
    }
}
