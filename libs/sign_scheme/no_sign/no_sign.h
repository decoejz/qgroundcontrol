#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define PRIVATE_KEY 0
#define PUBLIC_KEY 1
#define SIGMA_LEN 0
#define SIGN_MAX_LEN 0

    // ! Define unique interface
    int key_gen(const char *secret_name, const char *public_name);
    char *read_key(char key_type, const char *file_name);

    int verify(uint8_t *_msg_signed, int total_len, char *_public_key);
    int sign(uint8_t *msg_signed, uint8_t *msg_raw, unsigned int msg_len, char *_secret_key);

#ifdef __cplusplus
}
#endif
