#include "no_sign.h"

// Res:
// Msg len
int sign(uint8_t *msg_signed, uint8_t *msg_raw, unsigned int msg_len, char *_secret_key)
{
    // TODO: Improve this function to eliminate the memmove
    memmove(msg_signed, msg_raw, msg_len);
    return msg_len;
}