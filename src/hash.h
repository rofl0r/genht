/* assumes sizeof(unsigned)==4 */

/* not for strings: does unaligned access and reads past the end of key */
/* bob jenkins: lookup 3 */
unsigned jenhash(const void *key, unsigned len);
unsigned jenhash32(unsigned k);

/* austin appleby: murmur 2 */
unsigned murmurhash(const void *key, unsigned len);
unsigned murmurhash32(unsigned k);


/* simple hash for aligned pointers */
unsigned ptrhash(const void *k);

/* simple string hash - case sensitive and case-insensitive */
unsigned strhash(const char *k);
unsigned strhash_case(const char *key);

/* string keyeq functions - case sensitive and case-insensitive */
int strkeyeq(const char *a, const char *b);
int strkeyeq_case(const char *a, const char *b);

/* pointer match for htp*_t */
int ptrkeyeq(const void *a, const void *b);


/* long (int) */
long int longhash(long int l);
int longkeyeq(long a, long b);
