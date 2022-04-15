# echo_proxy

## Platform

Linux x86-64

## How to build

Requires: `autoconf`, `automake`, GCC or Clang supports C++11

```shell
$ autoreconf -i
$ ./configure
$ make -j$(nproc)
```

To specify a specific compiler (`clang++-8`, for example):

```shell
$ autoreconf -i
$ CXX=clang++-8 ./configure
$ make -j$(nproc)
```

Then run:

```shell
$ ./build/bin/echo_server <port> [Drop]
```

Or

```shell
$ ./build/bin/echo_client <port> <IP>
```
