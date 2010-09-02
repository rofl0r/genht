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
