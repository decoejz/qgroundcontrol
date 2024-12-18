if (APPLE)
    add_compile_options(-I/opt/homebrew/opt/openssl@3/include)
endif()

find_package(OpenSSL REQUIRED)

set(SRCS
    key_gen.c
    read_key.c
    sign.c
    verify.c
    rsa.h
)

add_library(rsa STATIC ${SRCS})

target_link_libraries(rsa PRIVATE OpenSSL::SSL OpenSSL::Crypto)