/* genht vector-to-double example

   Placed in the Public Domain (2021, Tibor 'Igor2' Palinkas)

   This example implements a local vector-to-integer hash table for
   two dimensional vectors. It demonstrates how to instantiate a custom
   key-value hash table.

   (Note: to keep the example small and focused on hash tables, floating
   point accuracy problems are not handled.)
*/

#include <stdio.h>
#include <math.h>

typedef struct {
	double x, y;
} vect_t;

/* create a htvi instance: API (this could be in a header, see src/htsi.h) */
typedef vect_t htvi_key_t;
typedef int htvi_value_t;
#define HT(x) htvi_ ## x
#include "ht.h"

/* implementation (could be a compilation unit, see src/htsi.c) */
#include "ht.c"

#undef HT

/* Calculate (not very efficient) hash value for doubles and vectors */
static unsigned dbl_hash(double val)
{
	unsigned a = floor(val), b = (val - a) * 100000000;
	return a ^ b;
}

static unsigned vect_hash(vect_t v)
{
	return dbl_hash(v.x) ^ dbl_hash(v.y);
}

/* compare two vectors and return 1 if they are the same */
static int vect_eq(vect_t a, vect_t b)
{
	return (a.x == b.x) && (a.y == b.y);
}


/* Helper function: increase the value of a vector in the hash */
static void vect_inc(htvi_t *ht, vect_t v)
{
	int curr = htvi_get(ht, v); /* returns 0 if not in the table */
	curr++;
	htvi_set(ht, v, curr);
}

/* Helper function: print the table */
static void print_all(htvi_t *ht, const char *title)
{
	htvi_entry_t *e;

	printf("%s\n", title);
	for(e = htvi_first(ht); e != NULL; e = htvi_next(ht, e))
		printf(" %f;%f -> %d\n", e->key.x, e->key.y, e->value);
	printf("\n");
}

int main()
{
	htvi_t ht;
	vect_t v1 = { 3.14, 42.42 };
	vect_t v2 = { 1.00, 666 };

	/* initialize the hash table in ht */
	htvi_init(&ht, vect_hash, vect_eq);


	vect_inc(&ht, v1);
	print_all(&ht, "After adding v1");

	vect_inc(&ht, v1);
	vect_inc(&ht, v1);
	print_all(&ht, "After adding more v1's");

	vect_inc(&ht, v2);
	print_all(&ht, "After adding v1");

	printf("final v1: %d\n", htvi_pop(&ht, v1));
	print_all(&ht, "After removing v1");


	/* Discard the table, free all administrative memory allocated in the lib */
	htvi_uninit(&ht);

	return 0;
}
