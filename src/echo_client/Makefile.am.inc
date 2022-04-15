noinst_HEADERS +=                                                              \
    %reldir%/dg_cli.h                                                          \
    ${NOTHING}

bin_PROGRAMS += build/bin/echo_client
build_bin_echo_client_SOURCES =                                                \
    %reldir%/dg_cli.cpp                                                        \
    %reldir%/echo_client.cpp                                                   \
    ${NOTHING}

build_bin_echo_client_LDADD =                                                  \
    build/lib/libcommon.a                                                      \
    -lpthread                                                                  \
    ${NOTHING}
