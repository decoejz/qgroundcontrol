#include "ecdsa.h"

// load_type:
//   0 => secret key
//   1 => public key
EVP_PKEY *read_key(char load_type, const char *file_name)
{
    EVP_PKEY *key = NULL;
    FILE *fp = fopen(file_name, "r");
    if (fp == NULL)
    {
        fprintf(stderr, "Read key error: Cannot open the file");
        return NULL;
    }

    if (load_type == PRIVATE_KEY)
    {

        key = PEM_read_PrivateKey(fp, NULL, NULL, NULL);
    }
    else if (load_type == PUBLIC_KEY)
    {
        key = PEM_read_PUBKEY(fp, NULL, NULL, NULL);
    }
    else
    {
        fprintf(stderr, "Read key error: Invalid input");
    }

    if (!key)
    {
        fprintf(stderr, "Read key error: Unable to load key from file %s\n", file_name);
    }

    fclose(fp);
    return key;
}
