#include "ecdsa.h"

int uc2int(uint8_t *res)
{
    return (res[0] << 24) | (res[1] << 16) | (res[2] << 8) | res[3];
}

// RES:
// <0 => Invalid Signature | Error
// msg_size => Signature Correct
// else => Error
int verify(uint8_t *msg_raw, uint8_t *msg_signed, int total_len, EVP_PKEY *public_key)
{
    if (public_key == NULL)
    {
        fprintf(stderr, "Validation ERROR - key is NULL\n");
        return -1;
    }

    int sigma_len = uc2int(msg_signed);
    int msg_len = total_len - SIGN_HEADER_SIZE - sigma_len;
    unsigned char sigma[sigma_len];

    // (siglen (int) + msg + sigma)
    memcpy(msg_raw, msg_signed + SIGN_HEADER_SIZE, msg_len);
    memcpy(sigma, msg_signed + SIGN_HEADER_SIZE + msg_len, sigma_len);

    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(public_key, NULL);
    if (!ctx)
    {
        fprintf(stderr, "Validation ERROR - Failed to create EVP_PKEY_CTX\n");
        return -1;
    }

    if (EVP_PKEY_verify_init(ctx) <= 0)
    {
        fprintf(stderr, "Validation ERROR - Failed to initialize EVP_PKEY_CTX\n");
        EVP_PKEY_CTX_free(ctx);
        return -1;
    }

    int res = EVP_PKEY_verify(ctx, sigma, sigma_len, (unsigned char *)msg_raw, msg_len);
    EVP_PKEY_CTX_free(ctx);
    if (res == 1)
    {
        return msg_len;
    }
    else
    {
        fprintf(stderr, "Validation ERROR - code %d\n", res);
        return res;
    }
}
