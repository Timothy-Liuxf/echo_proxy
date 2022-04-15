noinst_HEADERS +=                                                              \
    %reldir%/dg_echo.h                                                         \
    ${NOTHING}

bin_PROGRAMS += build/bin/echo_server
build_bin_echo_server_SOURCES =                                                \
    %reldir%/dg_echo.cpp                                                       \
    %reldir%/echo_server.cpp                                                   \
    ${NOTHING}

build_bin_echo_server_LDADD =                                                  \
    build/lib/libcommon.a                                                      \
    ${NOTHING}
