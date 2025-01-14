#ifdef __cplusplus
extern "C"
{
#endif

#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <stdio.h>

    typedef EVP_PKEY key_type;

#define PRIVATE_KEY 0
#define PUBLIC_KEY 1
#define SIGN_HEADER_SIZE 4
#define SIGN_MAX_LEN 2048

    // ! Define unique interface
    int key_gen(const char *secret_name, const char *public_name);
    EVP_PKEY *read_key(char load_type, const char *file_name);

    int verify(uint8_t *msg_raw, uint8_t *msg_signed, int total_len, EVP_PKEY *public_key);
    int sign(uint8_t *msg_signed, uint8_t *msg_raw, unsigned int msg_len, EVP_PKEY *secret_key);

#ifdef __cplusplus
}
#endif
