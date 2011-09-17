typedef long int htip_key_t;
typedef void *htip_value_t;
#define HT(x) htip_ ## x
#include "ht.h"
#undef HT
