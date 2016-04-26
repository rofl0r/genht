#ifndef GENHT_HTSS_H
#define GENHT_HTSS_H

typedef char *htss_key_t;
typedef char *htss_value_t;
#define HT(x) htss_ ## x
#include "ht.h"
#undef HT

#endif
