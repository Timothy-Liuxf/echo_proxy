#ifndef DG_CLI_H
#define DG_CLI_H

#include <common/common.h>

[[noreturn]] void dg_cli (int sock, const struct ::sockaddr *server_addr,
                          ::socklen_t addr_len);

#endif // !DG_CLI_H
