set(SRCS
    key_gen.c
    read_key.c
    sign.c
    verify.c
    no_sign.h
)

add_library(no_sign STATIC ${SRCS})