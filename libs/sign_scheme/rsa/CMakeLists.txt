
# if (APPLE)
#     add_compile_options(-I/opt/homebrew/opt/openssl@3/include)
# endif()

# find_package(OpenSSL REQUIRED)

# qt_add_library(rsa STATIC
# 	key_gen.c
# 	read_key.c
# 	sign.c
# 	verify.c
# 	rsa.h
# )

# target_link_libraries(rsa PUBLIC OpenSSL::SSL OpenSSL::Crypto)

set(SRCS
    key_gen.c
    read_key.c
    sign.c
    verify.c
    rsa.h
)

# ideia do que pode resolver o probnlema da lib importada
# target_compile_options(mavlink_c INTERFACE -Wno-address-of-packed-member -Wno-cast-align)

# add_library(rsa STATIC ${SRCS} OpenSSL::SSL OpenSSL::Crypto)