#ifndef GENHT_HTPI_H
#define GENHT_HTPI_H

typedef void *htpi_key_t;
typedef int htpi_value_t;
#define HT(x) htpi_ ## x
#include "ht.h"
#undef HT

#endif
