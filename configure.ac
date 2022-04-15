#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([echo_proxy], [1.0.0.0])
AC_CONFIG_SRCDIR([src/common/common.cpp])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AC_PROG_RANLIB

AM_INIT_AUTOMAKE([foreign subdir-objects])

AX_CXX_COMPILE_STDCXX(11, [noext], [mandatory])
AX_APPEND_COMPILE_FLAGS([-Wall -Wpedantic -Wextra -Werror -O2], [CXXFLAGS])
AX_APPEND_COMPILE_FLAGS([-Isrc], [CXXFLAGS])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h netdb.h sys/socket.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_CHECK_HEADER_STDBOOL
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T
AC_TYPE_UINT64_T

# Checks for library functions.
AC_FUNC_FORK
AC_CHECK_FUNCS([socket])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
