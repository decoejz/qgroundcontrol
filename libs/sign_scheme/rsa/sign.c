#include "rsa.h"

// Res:
// 0 => ERROR
// else => Success (msg total len)
int sign(uint8_t *msg_signed, uint8_t *msg_raw, unsigned int msg_len, EVP_PKEY *secret_key)
{
    // Create a context
    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(secret_key, NULL);

    // Initialize signing
    EVP_PKEY_sign_init(ctx);
    uint8_t sigma[SIGMA_LEN]; // Signature var
    size_t siglen = SIGN_MAX_LEN;
    if (!EVP_PKEY_sign(ctx, sigma, &siglen, msg_raw, (size_t)msg_len))
    {
        printf("sign error: %s\n", strerror(errno));
        return 0;
    }
    EVP_PKEY_CTX_free(ctx);

    // * Concatenate sign with message to send (msg + sigma)
    memmove(msg_signed, msg_raw, msg_len);
    memmove(msg_signed + msg_len, (uint8_t *)sigma, siglen);

    return msg_len + siglen;
}