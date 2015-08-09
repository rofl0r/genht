/* assumes sizeof(unsigned)==4 */

/* not for strings: does unaligned access and reads past the end of key */
/* bob jenkins: lookup 3 */
unsigned jenhash(const void *key, unsigned len);
unsigned jenhash32(unsigned k);

/* austin appleby: murmur 2 */
unsigned murmurhash(const void *key, unsigned len);
unsigned murmurhash32(unsigned k);


/* simple hash for aligned pointers */
unsigned ptrhash(void *k);

/* simple string hash */
unsigned strhash(char *k);

