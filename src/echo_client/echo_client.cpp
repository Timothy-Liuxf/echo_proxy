#include "dg_cli.h"
#include <common/common.h>
#include <cstdio>
#include <string>

int
main (int argc, char *argv[])
{
  const char *host = "127.0.0.1";

  unsigned short port = DEFAULT_PORT;

  if (argc >= 2)
    {
      char *port_str = argv[1];
      try
        {
          int val = std::stoi (std::string (port_str));
          if (val > 1023 && val < 65536)
            {
              port = (unsigned short)val;
            }
          else
            {
              std::printf (
                  "Port should be between 1024 and 65535! Use default: %d\n",
                  (int)DEFAULT_PORT);
            }
        }
      catch (...)
        {
          std::printf ("Port specified invalid! Use default: %d\n",
                       (int)DEFAULT_PORT);
        }

      if (argc >= 3)
        {
          host = argv[2];
        }
    }

  std::printf ("Try to connect to server: %s:%d\n", host, (int)port);

  struct ::sockaddr_in server_addr;
  ::zeromemobj (server_addr);
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl (INADDR_ANY);
  server_addr.sin_port = htons (port);
  Inet_pton (AF_INET, host, &server_addr.sin_addr);
  auto sock = ::Socket (AF_INET, SOCK_DGRAM, 0);
  ::dg_cli (sock, (struct ::sockaddr *)&server_addr, sizeof (server_addr));
  return 0;
}
