# Compiler and flags
CC = gcc
LIBS = -lcrypto       # Link with the crypto library

ifeq "$(ARCH)" "macos"
	CRYPTOLIB = -I/opt/homebrew/opt/openssl@3/include
else
	CRYPTOLIB = 
endif

CFLAGS = -Wall -fPIC $(CRYPTOLIB)  # -fPIC for position-independent code (required for shared libraries)

# Source files
SRCS = key_gen.c read_key.c sign.c verify.c

# Object files derived from the source files
OBJS = $(SRCS:.c=.o)

# Library names
STATIC_LIB = librsa.a
# SHARED_LIB = librsa.so

# Build all (default target)
all: $(STATIC_LIB) # $(SHARED_LIB)

# Create static library (librsa.a)
$(STATIC_LIB): $(OBJS)
	ar rcs $(STATIC_LIB) $(OBJS)

# Create shared library (librsa.so)
# $(SHARED_LIB): $(OBJS)
# 	$(CC) -shared -o $(SHARED_LIB) $(OBJS) $(LIBS)

# Compile each .c file into an object file
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJS) $(STATIC_LIB) $(SHARED_LIB)
