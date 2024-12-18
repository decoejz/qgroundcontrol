#include "no_sign.h"

// Res:
// 0 => ERROR
// else => Success (msg total len)
int sign(uint8_t *msg_signed, uint8_t *msg_raw, unsigned int msg_len, char *_secret_key)
{
    // ! Verificar como otimizar isso e evitar processamento extra de memoria
    memmove(msg_signed, msg_raw, msg_len);
    return msg_len;
}