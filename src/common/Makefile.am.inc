noinst_HEADERS +=                                                              \
    %reldir%/common.h                                                          \
    %reldir%/concurrency/unordered_map.hpp                                     \
    ${NOTHING}

lib_LIBRARIES += build/lib/libcommon.a
build_lib_libcommon_a_SOURCES =                                                \
    %reldir%/common.cpp                                                        \
    ${NOTHING}
