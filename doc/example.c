/* genht string-to-integer example

   Placed in the Public Domain (2021, Tibor 'Igor2' Palinkas

   This example reads stdin line by line and remembers the line number
   of the last occurance of each unique line content.

   (Note: the code is c89, except for the strdup() call which is used to
   keep boilerplate small)
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* string-to-integer hash table; normally <genht/htsi.h> when genht is installed */
#include "htsi.h"

/* For calculating hashes and comparing keys (we don't want to write our own);
   normally <genht/hash.h> when genht is installed */
#include "hash.h"


int main()
{
	char *s, *end, line[1024];
	int lineno = 0;
	htsi_t ht;
	htsi_entry_t *e;

	/* initialize the hash table in ht */
	htsi_init(&ht, strhash, strkeyeq);

	/* read each line of stdin and remember the last occurance line number for each */
	while((s = fgets(line, sizeof(line), stdin)) != NULL) {
		lineno++;

		/* remove newline from the end of the line */
		end = strpbrk(s, "\r\n");
		if (end != NULL) *end = '\0';

		/* update existing line or enter a new line in the hash */
		if (htsi_has(&ht, s))
			htsi_set(&ht, s, lineno); /* existing entry: just update the number */
		else
			htsi_set(&ht, strdup(s), lineno); /* new entry: need to strdup() because htsi stores the pointer only */
	}

	/* simple lookups */
	printf("Last empty line: %d\n", htsi_get(&ht, ""));
	printf("Last 'hello': %d\n", htsi_get(&ht, "hello"));

	/* print all unique lines */
	printf("Table:\n");
	for(e = htsi_first(&ht); e != NULL; e = htsi_next(&ht, e))
		printf(" '%s' last in line %d\n", e->key, e->value);

	/* Our keys are strdup'd, free them */
	for(e = htsi_first(&ht); e != NULL; e = htsi_next(&ht, e))
		free(e->key);

	/* Discard the table, free all administrative memory allocated in the lib */
	htsi_uninit(&ht);

	return 0;
}
