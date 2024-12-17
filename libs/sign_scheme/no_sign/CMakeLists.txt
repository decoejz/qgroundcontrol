# Minimum CMake version
# cmake_minimum_required(VERSION 3.10)

# # Project name
# project(no_sign LANGUAGES C)

# # Compiler flags
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -fPIC") # -fPIC for position-independent code

# # Source files
# set(SRCS
#     key_gen.c
#     read_key.c
#     sign.c
#     verify.c
#     no_sign.h
# )

# # Add a static library target
# add_library(no_sign STATIC ${SRCS})

# # # Set the output name for the static library
# set_target_properties(no_sign PROPERTIES OUTPUT_NAME "no_sign")

# # Add a clean target
# add_custom_target(clean
#     COMMAND ${CMAKE_COMMAND} -P "${CMAKE_BINARY_DIR}/cmake_clean.cmake"
#     COMMENT "Cleaning up build files"
# )

# Generate the clean script
# file(GENERATE OUTPUT "${CMAKE_BINARY_DIR}/cmake_clean.cmake" CONTENT
# "file(REMOVE_RECURSE ${CMAKE_BINARY_DIR}/*.o ${CMAKE_BINARY_DIR}/libno_sign.a)"
# )


# qt_add_library(no_sign STATIC
#     key_gen.c
#     read_key.c
#     sign.c
#     verify.c
#     no_sign.h
# )

# target_link_libraries(no_sign PUBLIC)

# TEST!

set(SRCS
    key_gen.c
    read_key.c
    sign.c
    verify.c
    no_sign.h
)

add_library(no_sign STATIC ${SRCS})