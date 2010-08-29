#include <stdlib.h>
#include <assert.h>

#define HT_MINSIZE 8
#define HT_MAXSIZE (1U << 31)

#define JUMP(i, j) i += j++
#define JUMP_FIRST(i, j) j = 1, i += j++

/* generic functions, useful if ht_entry_t changes */

static inline void setused(HT(entry_t) *entry) {
	entry->flag = 1;
}

static inline void setdeleted(HT(entry_t) *entry) {
	entry->flag = -1;
}

static inline HT(hash_t) entryhash(const HT(entry_t) *entry) {
	return entry->hash;
}

void HT(init)(HT(t) *ht, HT(hash_t) (*keyhash)(HT(key_t)), int (*keyeq)(HT(key_t), HT(key_t))) {
	ht->mask = HT_MINSIZE - 1;
	ht->fill = 0;
	ht->used = 0;
	ht->table = calloc(ht->mask + 1, sizeof(HT(entry_t)));
	assert(ht->table);
	ht->keyhash = keyhash;
	ht->keyeq = keyeq;
}

void HT(uninit)(HT(t) *ht) {
	free(ht->table);
	ht->table = NULL;
}

HT(t) *HT(alloc)(HT(hash_t) (*keyhash)(HT(key_t)), int (*keyeq)(HT(key_t), HT(key_t))) {
	HT(t) *ht = malloc(sizeof(HT(t)));

	assert(ht);
	HT(init)(ht, keyhash, keyeq);
	return ht;
}

void HT(clear)(HT(t) *ht) {
	HT(uninit)(ht);
	HT(init)(ht, ht->keyhash, ht->keyeq);
}

void HT(free)(HT(t) *ht) {
	HT(uninit)(ht);
	free(ht);
}

/* one lookup function to rule them all */
static HT(entry_t) *HT(lookup)(HT(t) *ht, HT(key_t) key, HT(hash_t) hash) {
	unsigned int mask = ht->mask;
	unsigned int i = hash;
	unsigned int j;
	HT(entry_t) *table = ht->table;
	HT(entry_t) *entry = table + (i & mask);
	HT(entry_t) *free_entry; /* first deleted entry for insert */

	if (HT(isempty)(entry))
		return entry;
	else if (HT(isdeleted)(entry))
		free_entry = entry;
	else if (entryhash(entry) == hash && ht->keyeq(entry->key, key))
		return entry;
	else
		free_entry = NULL;
	for (JUMP_FIRST(i, j); ; JUMP(i, j)) {
		entry = table + (i & mask);
		if (HT(isempty)(entry))
			return (free_entry == NULL) ? entry : free_entry;
		else if (HT(isdeleted)(entry)) {
			if (free_entry == NULL)
				free_entry = entry;
		} else if (entryhash(entry) == hash && ht->keyeq(entry->key, key))
			return entry;
	}
}

/* for copy and resize: no deleted entries in ht, entry->key is not in ht */
static void cleaninsert(HT(t) *ht, const HT(entry_t) *entry) {
	unsigned int mask = ht->mask;
	unsigned int i = entryhash(entry);
	unsigned int j;
	HT(entry_t) *table = ht->table;
	HT(entry_t) *newentry = table + (i & mask);

	if (!HT(isempty)(newentry))
		for (JUMP_FIRST(i, j); !HT(isempty)(newentry); JUMP(i, j))
			newentry = table + (i & mask);
	ht->fill++;
	ht->used++;
	*newentry = *entry;
	setused(newentry);
}

HT(t) *HT(copy)(const HT(t) *ht) {
	HT(t) *newht;
	HT(entry_t) *entry;
	unsigned int used;

	newht = calloc(1, sizeof(HT(t)));
	assert(newht);
	newht->mask = ht->mask;
	newht->table = calloc(newht->mask + 1, sizeof(HT(entry_t)));
	assert(newht->table);
	for (entry = ht->table, used = ht->used; used > 0; entry++)
		if (HT(isused)(entry)) {
			used--;
			cleaninsert(newht, entry);
		}
	return newht;
}

void HT(resize)(HT(t) *ht, unsigned int hint) {
	unsigned int newsize;
	unsigned int oldused = ht->used;
	HT(entry_t) *oldtable = ht->table;
	HT(entry_t) *entry;

	if (hint < oldused << 1)
		hint = oldused << 1;
	if (hint > HT_MAXSIZE)
		hint = HT_MAXSIZE;
	for (newsize = HT_MINSIZE; newsize < hint; newsize <<= 1);
	ht->table = calloc(newsize, sizeof(HT(entry_t)));
	assert(ht->table);
	ht->mask = newsize - 1;
	ht->fill = 0;
	ht->used = 0;
	for (entry = oldtable; oldused > 0; entry++)
		if (HT(isused)(entry)) {
			oldused--;
			cleaninsert(ht, entry);
		}
	free(oldtable);
}

int HT(has)(HT(t) *ht, HT(key_t) key) {
	HT(entry_t) *entry = HT(lookup)(ht, key, ht->keyhash(key));

	return HT(isused)(entry);
}

HT(value_t) HT(get)(HT(t) *ht, HT(key_t) key) {
	HT(entry_t) *entry = HT(lookup)(ht, key, ht->keyhash(key));

	return HT(isused)(entry) ? entry->value : 0;
}

HT(entry_t) *HT(getentry)(HT(t) *ht, HT(key_t) key) {
	HT(entry_t) *entry = HT(lookup)(ht, key, ht->keyhash(key));

	return HT(isused)(entry) ? entry : NULL;
}

/* fill threshold = 3/4 */
static inline void checkfill(HT(t) *ht) {
	if (ht->fill > ht->mask - (ht->mask >> 2) || ht->fill > ht->used << 2)
		HT(resize)(ht, ht->used << (ht->used > 1 << 16 ? 1 : 2));
}

HT(entry_t) *HT(insert)(HT(t) *ht, HT(key_t) key, HT(value_t) value) {
	HT(hash_t) hash = ht->keyhash(key);
	HT(entry_t) *entry = HT(lookup)(ht, key, hash);

	if (HT(isused)(entry))
		return entry;
	if (HT(isempty)(entry))
		ht->fill++;
	ht->used++;
	entry->hash = hash;
	entry->key = key;
	entry->value = value;
	setused(entry);
	checkfill(ht);
	return NULL;
}

void HT(set)(HT(t) *ht, HT(key_t) key, HT(value_t) value) {
	HT(entry_t) *entry = HT(insert)(ht, key, value);

	if (entry)
		entry->value = value;
}

HT(value_t) HT(pop)(HT(t) *ht, HT(key_t) key) {
	HT(entry_t) *entry = HT(lookup)(ht, key, ht->keyhash(key));
	HT(value_t) v;

	if (!HT(isused)(entry))
		return 0;
	ht->used--;
	v = entry->value;
	setdeleted(entry);
	return v;
}

HT(entry_t) *HT(popentry)(HT(t) *ht, HT(key_t) key) {
	HT(entry_t) *entry = HT(lookup)(ht, key, ht->keyhash(key));

	if (HT(isused)(entry)) {
		ht->used--;
		setdeleted(entry);
		return entry;
	}
	return NULL;
}

void HT(delentry)(HT(t) *ht, HT(entry_t) *entry) {
	if (!HT(isused)(entry))
		return;
	ht->used--;
	setdeleted(entry);
}
