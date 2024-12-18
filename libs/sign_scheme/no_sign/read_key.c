#include "no_sign.h"

// load_type:
//   0 => secret key
//   1 => public key
char *read_key(char _load_type, const char *_file_name)
{
    static char value = '1';
    return &value;
}
