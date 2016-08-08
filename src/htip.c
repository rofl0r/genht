#include "htip.h"
#define HT(x) htip_ ## x
#include "ht.c"

int keyeq_int(htip_key_t a, htip_key_t b)
{
	return a == b;
}

#undef HT
