#include "dg_echo.h"
#include <common/common.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <signal.h>
#include <string>
// #include <sys/wait.h>

// void
// wait_child_proc (int)
// {
//   ::pid_t pid;
//   int status;
//   while ((pid = ::waitpid (0, &status, WNOHANG)) > 0)
//     ;
// }

int
main (int argc, char *argv[])
{
  bool randomlyDrop = false;
  unsigned short port = DEFAULT_PORT;

  if (argc > 1)
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
      if (argc > 2 && ::std::strcmp (argv[2], "Drop") == 0)
        {
          randomlyDrop = true;
          ::srand ((unsigned)std::time (nullptr));
          std::printf ("[Info] Server will randomly drop packets!\n");
        }
    }

  std::printf ("Try to listen at port: %d\n", (int)port);

  auto sock = ::Socket (AF_INET, SOCK_DGRAM, 0);
  struct ::sockaddr_in server_addr, client_addr;
  ::zeromem ((void *)&server_addr, sizeof (server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl (INADDR_ANY);
  server_addr.sin_port = htons (port);
  ::Bind (sock, (struct ::sockaddr *)&server_addr, sizeof (server_addr));

  // Ignore SIGPIPE
  struct ::sigaction newsig;
  zeromemobj (newsig);
  ::sigemptyset (&newsig.sa_mask);
  newsig.sa_handler = SIG_IGN;
  ::sigaction (SIGPIPE, &newsig, 0);

  ::dg_echo (sock, (struct ::sockaddr *)&client_addr, sizeof (client_addr),
             randomlyDrop);
  return 0;
}
