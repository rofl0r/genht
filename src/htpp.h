#ifndef GENHT_HTPP_H
#define GENHT_HTPP_H

typedef void *htpp_key_t;
typedef void *htpp_value_t;
#define HT(x) htpp_ ## x
#include "ht.h"
#undef HT

#endif