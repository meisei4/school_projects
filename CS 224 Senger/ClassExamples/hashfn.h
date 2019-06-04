#ifndef HASH_FN_H
#define HASH_FN_H

// simply sum char values and mod
unsigned int
sumHash(const char * key, unsigned int modulus);

// FNV hash function found at
// http://www.isthe.com/chongo/tech/comp/fnv/index.html#history

// use u_int32_t ?
#define FNV_32_PRIME ((unsigned int)0x01000193)
#define FNV1_32_INIT ((unsigned int)0x811c9dc5)

// FNV_1a hash using FNV1_32_INIT as start value
unsigned int
FNV_1a(const char *str, unsigned int modulus);

// FNV_1a hash using seed as start value
unsigned int
FNV_1a_seed(const char *str, unsigned int seed, unsigned int modulus);

// FNV_1a hash but fold into single byte before mod
unsigned int
FNV_1a_byte(const char *str, unsigned int modulus);


#endif
