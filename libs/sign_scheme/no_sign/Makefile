# Compiler and flags
CC = gcc

CFLAGS = -Wall -fPIC # -fPIC for position-independent code (required for shared libraries)

# Source files
SRCS = key_gen.c read_key.c sign.c verify.c

# Object files derived from the source files
OBJS = $(SRCS:.c=.o)

# Library names
STATIC_LIB = libno_sign.a

# Build all (default target)
all: $(STATIC_LIB)

# Create static library (libno_sign.a)
$(STATIC_LIB): $(OBJS)
	ar rcs $(STATIC_LIB) $(OBJS)

# Compile each .c file into an object file
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJS) $(STATIC_LIB)
