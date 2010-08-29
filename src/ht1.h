/* open addressing hash table */
/* no malloc checks (out of memory == segfault), max size is 1 << 31 */
/* an entry pointer is valid until the next insertion or resize */
/*
typedef void *HT(key_t);
typedef void *HT(value_t);
*/
typedef unsigned int HT(hash_t);

typedef struct {
	int flag;
	HT(hash_t) hash;
	HT(key_t) key;
	HT(value_t) value;
} HT(entry_t);

typedef struct {
	unsigned int mask;
	unsigned int fill;
	unsigned int used;
	HT(entry_t) *table;

	HT(hash_t) (*keyhash)(HT(key_t));
	int (*keyeq)(HT(key_t), HT(key_t));
} HT(t);

HT(t) *HT(alloc)(HT(hash_t) (*keyhash)(HT(key_t)), int (*keyeq)(HT(key_t), HT(key_t)));
void HT(init)(HT(t) *ht, HT(hash_t) (*keyhash)(HT(key_t)), int (*keyeq)(HT(key_t), HT(key_t)));
void HT(free)(HT(t) *ht);
void HT(uninit)(HT(t) *ht);
void HT(clear)(HT(t) *ht);
HT(t) *HT(copy)(const HT(t) *ht);
/* new size is 2^n >= hint */
void HT(resize)(HT(t) *ht, unsigned int hint);

/* ht[key] is used */
int HT(has)(HT(t) *ht, HT(key_t) key);
/* value of ht[key] or 0 if key is not used */
HT(value_t) HT(get)(HT(t) *ht, HT(key_t) key);
/* entry of ht[key] or NULL if key is not used */
HT(entry_t) *HT(getentry)(HT(t) *ht, HT(key_t) key);
/* ht[key] = value */
void HT(set)(HT(t) *ht, HT(key_t) key, HT(value_t) value);
/* if key is used then return ht[key] else ht[key] = value and return NULL */
/* (the value of the returned used entry can be modified) */
HT(entry_t) *HT(insert)(HT(t) *ht, HT(key_t) key, HT(value_t) value);
/* delete key and return ht[key] or 0 if key is not used */
HT(value_t) HT(pop)(HT(t) *ht, HT(key_t) key);
/* delete key and return ht[key] or NULL if key is not used */
/* (the returned deleted entry can be used to free key,value resources) */
HT(entry_t) *HT(popentry)(HT(t) *ht, HT(key_t) key);
/* delete entry (useful for destructive iteration) */
void HT(delentry)(HT(t) *ht, HT(entry_t) *entry);

/* helper functions */
static inline unsigned int HT(length)(const HT(t) *ht) {return ht->used;}
static inline unsigned int HT(fill)(const HT(t) *ht) {return ht->fill;}
static inline unsigned int HT(size)(const HT(t) *ht) {return ht->mask + 1;}

/* for any entry exactly one returns true */
static inline int HT(isused)(const HT(entry_t) *entry) {return entry->flag > 0;}
static inline int HT(isempty)(const HT(entry_t) *entry) {return entry->flag == 0;}
static inline int HT(isdeleted)(const HT(entry_t) *entry) {return entry->flag < 0;}

/* first used (useful for iteration) */
static inline HT(entry_t) *HT(first)(const HT(t) *ht)
{
	HT(entry_t) *entry = 0;

	if (ht->used)
		for (entry = ht->table; !HT(isused)(entry); entry++);
	return entry;
}

/* next used (useful for iteration) */
static inline HT(entry_t) *HT(next)(const HT(t) *ht, HT(entry_t) *entry)
{
	while (++entry != ht->table + ht->mask + 1)
		if (HT(isused)(entry))
			return entry;
	return 0;
}
