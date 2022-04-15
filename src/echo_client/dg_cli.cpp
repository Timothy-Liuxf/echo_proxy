#include "dg_cli.h"
#include <atomic>
#include <common/concurrency/unordered_map.hpp>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

std::uint64_t current_seq_num = 0u;
echo_proxy::concurrency::unordered_map<
    uint64_t, std::pair<std::shared_ptr<std::atomic_bool>, std::string> >
    storages;

static void
wait_for_reply (socket_t sock, std::string buf,
                const struct ::sockaddr *server_addr, ::socklen_t addr_len)
{
  try
    {
      const auto seq_num = current_seq_num++;
      char prefix[MIN_LEN];
      *(uint32_t *)(prefix) = BOM;
      *(uint32_t *)((char *)prefix + BOM_LEN) = 1u;
      *(uint64_t *)((char *)prefix + SEQ_BEGIN_POS) = seq_num;
      buf = std::string ((char *)prefix, MIN_LEN) + buf;

      ::Sendto (sock, (const void *)buf.c_str (), buf.size (), 0, server_addr,
                addr_len);

      storages.emplace (
          seq_num, std::make_pair (std::make_shared<std::atomic_bool> (false),
                                   std::move (buf)));
      std::vector<char> client_buf_vec (MAX_BUFF_SIZE);
      char *client_buf = client_buf_vec.data ();
      struct ::sockaddr_in src_addr;
      socklen_t src_addr_len = sizeof (src_addr);
      std::pair<std::shared_ptr<std::atomic_bool>, std::string> pr;

      while (true)
        {
          int ret = ::recvfrom (sock, client_buf, MAX_BUFF_SIZE, 0,
                                (struct ::sockaddr *)&src_addr, &src_addr_len);
          if (ret < 0)
            {
              if (errno == EWOULDBLOCK)
                {
                  if (storages.try_get (seq_num, pr) && *pr.first == false)
                    {
                      // Resend
                      ::Sendto (sock, (const void *)pr.second.c_str (),
                                pr.second.size (), 0, server_addr, addr_len);
                    }
                  else
                    {
                      return; // Has been received by another thread
                    }
                }
              else
                {
                  sys_err ("Receive error!");
                }
            }
          else
            {
              // Source authentication
              if (ret < MIN_LEN || addr_len != src_addr_len
                  || ::memcmp ((void *)server_addr, (void *)&src_addr, addr_len)
                         != 0)
                {
                  // Not mine
                  goto check_sent;
                }
              else
                {
                  // Parse sequence and version
                  char *p = client_buf;
                  uint32_t version = 0u;

                  if (*(std::uint32_t *)p == BOM)
                    {
                      version = *(std::uint32_t *)(p + BOM_LEN);
                    }
                  else if (*(std::uint32_t *)p == REVERSE_BOM)
                    {
                      std::uint8_t *q = (std::uint8_t *)client_buf + BOM_LEN;
                      version = ((std::uint32_t)q[0] << 0)
                                | ((std::uint32_t)q[1] << 8)
                                | ((std::uint32_t)q[2] << 16)
                                | ((std::uint32_t)q[3] << 24);
                    }

                  switch (version)
                    {
                    case 1u:
                      {
                        uint64_t recv_seq_num
                            = *(uint64_t *)(client_buf + SEQ_BEGIN_POS);
                        if (storages.try_pop (recv_seq_num, pr)
                            && pr.first->exchange (true) == false)
                          {
                            std::string ask = std::move (pr.second);
                            printf ("|Ask:   |%*s\n|Reply: |%.*s\n",
                                    (int)ask.size () - MIN_LEN,
                                    ask.c_str () + MIN_LEN, (int)ret - MIN_LEN,
                                    client_buf + MIN_LEN);
                          }
                        if (recv_seq_num == seq_num)
                          {
                            // Has sent
                            return;
                          }
                        goto check_sent;
                      }
                      break;
                    default:
                      goto check_sent; // Not any version!
                    }
                }

            check_sent:
              if (storages.try_get (seq_num, pr) == false || *pr.first == true)
                {
                  return; // Has been received by another thread
                }
            }
        }
    }
  catch (std::exception &e)
    {
      std::printf ("Fail in one message: %s\n", e.what ());
    }
}

[[noreturn]] void
dg_cli (int sock, const struct ::sockaddr *server_addr, ::socklen_t addr_len)
{
  struct timeval tv;
  tv.tv_sec = 2; // Time out: 5 seconds
  tv.tv_usec = 0;
  ::Setsockopt (sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof (tv));
  try
    {
      std::string buf;
      while (true)
        {
          std::getline (std::cin, buf);
          std::size_t itr = 0u;
          while (itr != buf.size ())
            {
              std::string tmp_buf;
              if (buf.size () - itr <= MAX_BUFF_SIZE - MIN_LEN)
                {
                  tmp_buf = std::string (buf, itr);
                  itr = buf.size ();
                }
              else
                {
                  tmp_buf = std::string (buf, itr, MAX_BUFF_SIZE - MIN_LEN);
                  itr += MAX_BUFF_SIZE - MIN_LEN;
                }
              try
                {
                  std::thread ([=] {
                    wait_for_reply (sock, std::move (tmp_buf), server_addr,
                                    addr_len);
                  }).detach ();
                }
              catch (std::exception &e)
                {
                  std::printf ("Create thread failed: %s\n", e.what ());
                  std::exit (-1);
                }
            }
        }
    }
  catch (const std::exception &e)
    {
      printf ("%s\n", e.what ());
      std::exit (-1);
    }
}
