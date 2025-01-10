#include "ecdsa.h"

// Transform int to uint8_t
int int2uc(uint8_t *res, int num)
{
    res[0] = num >> 24;
    res[1] = num >> 16;
    res[2] = num >> 8;
    res[3] = num;
    return 0;
}

// Res:
// 0 => ERROR
// else => Success (msg total len)
int sign(uint8_t *msg_signed, uint8_t *msg_raw, unsigned int msg_len, EVP_PKEY *secret_key)
{
    // Create a context
    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(secret_key, NULL);

    // Initialize signing
    EVP_PKEY_sign_init(ctx);
    uint8_t sigma[SIGN_MAX_LEN]; // Signature var
    size_t siglen = SIGN_MAX_LEN;
    if (!EVP_PKEY_sign(ctx, sigma, &siglen, msg_raw, (size_t)msg_len))
    {
        printf("sign error: %s\n", strerror(errno));
        return 0;
    }
    EVP_PKEY_CTX_free(ctx);

    // * Concatenate sign with message to send (siglen (int) + msg + sigma)
    uint8_t siglen_size[4];
    int2uc(siglen_size, (int)siglen);

    memmove(msg_signed, siglen_size, SIGN_HEADER_SIZE);
    memmove(msg_signed + SIGN_HEADER_SIZE, msg_raw, msg_len);
    memmove(msg_signed + SIGN_HEADER_SIZE + msg_len, (uint8_t *)sigma, siglen);

    return SIGN_HEADER_SIZE + msg_len + siglen;
}
