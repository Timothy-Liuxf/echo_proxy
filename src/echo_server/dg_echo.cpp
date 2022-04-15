#include "dg_echo.h"
#include <cstdlib>

char server_buffer[MAX_BUFF_SIZE];

[[noreturn]] void
dg_echo (socket_t sock, struct ::sockaddr *client_addr, socklen_t addr_len)
{
  while (true)
    {
      auto n = ::Recvfrom (sock, (void *)server_buffer, (::size_t)MAX_BUFF_SIZE,
                           0, client_addr, &addr_len);

      if (n == 0)
        {
          continue;
        }

      if (n < MIN_LEN)
        {
          continue;
        }

      char *p = server_buffer;
      uint32_t version = 0u;

      if (*(std::uint32_t *)p == BOM)
        {
          version = *(std::uint32_t *)(p + BOM_LEN);
        }
      else if (*(std::uint32_t *)p == REVERSE_BOM)
        {
          std::uint8_t *q = (std::uint8_t *)server_buffer + BOM_LEN;
          version = ((std::uint32_t)q[0] << 0) | ((std::uint32_t)q[1] << 8)
                    | ((std::uint32_t)q[2] << 16) | ((std::uint32_t)q[3] << 24);
        }

      switch (version)
        {
        case 1u:
          {
            *(uint32_t *)((char *)server_buffer) = BOM;
            *(uint32_t *)((char *)server_buffer + BOM_LEN)
                = 1u; // Use Version 1 protocol
            sendto (sock, (void *)server_buffer, n, 0, client_addr, addr_len);
          }
          break;
        default:
          continue; // Not any version!
        }

      // Originally used for server parallel
      //
      // auto pid = ::fork ();
      // if (UNLIKELY (pid < 0))
      //   {
      //     sys_err ("Fork process failed!");
      //   }
      // else if (pid == 0)
      //   {
      //     ::close (sock);
      //     ::std::exit (0);
      //   }
      // else
      //   {
      //     (void)0; // Do nothing
      //   }
    }
}
