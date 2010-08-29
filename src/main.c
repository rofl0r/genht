#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "htsi1.h"

static unsigned int keyhash(char *key) {
	unsigned char *p = (unsigned char *)key;
	unsigned int hash = 0;

	while (*p)
		hash += (hash << 2) + *p++;
	return hash;
}

static int keyeq(char *a, char *b) {
	char *pa = (char *)a;
	char *pb = (char *)b;

	for (; *pa == *pb; pa++, pb++)
		if (*pa == '\0')
			return 1;
	return 0;
}

int main() {
	ht_t *ht;
	ht_entry_t *e;

	ht = ht_alloc(keyhash, keyeq);
	ht_set(ht, "a", 1);
	ht_set(ht, "b", 2);
	ht_set(ht, "asdf", -3);
	ht_set(ht, "qw", 4);
	ht_set(ht, "v", 5);
	ht_set(ht, "df", 6);
	ht_set(ht, "x", 7);
	if (!ht_has(ht, "a"))
		puts("ERR: has a");
	if (ht_has(ht, "1"))
		puts("ERR: has 1");
	if (ht_insert(ht, "y", 8))
		puts("ERR: insert y");
	if (ht_insert(ht, "x", 9)->value != 7)
		puts("ERR: insert x");
	if (ht_pop(ht, "b") != 2 || ht_getentry(ht, "b"))
		puts("ERR: pop b");
	if (ht_popentry(ht, "b"))
		puts("ERR: pope b");
	if (ht_popentry(ht, "c"))
		puts("ERR: pope c");
	for (e = ht_first(ht); e; e = ht_next(ht, e)) {
		if (ht_get(ht, e->key) != e->value)
			printf("ERR %s %d\n", e->key, e->value);
		printf("%s %d\n", e->key, e->value);
	}
	ht_clear(ht);
	for (e = ht_first(ht); e; e = ht_next(ht, e))
		puts("ERR: clear");
	ht_free(ht);
	return 0;
}
