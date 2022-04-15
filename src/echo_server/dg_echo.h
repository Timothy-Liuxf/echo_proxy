#ifndef DG_ECHO_H_
#define DG_ECHO_H_

#include <common/common.h>

[[noreturn]] void dg_echo (socket_t sock, struct ::sockaddr *client_addr,
                           socklen_t addr_len);

#endif // !DG_ECHO_H_
