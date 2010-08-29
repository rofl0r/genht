typedef char *htsi_key_t;
typedef int htsi_value_t;
#define HT(x) htsi_ ## x
#include "ht.h"
#undef HT
