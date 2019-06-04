#include <stdlib.h>
#include <string.h>


unsigned int
sumHash(const char * key, unsigned int modulus) {
  unsigned int sum = 0;
  for (int i = 0; i < strlen(key); i++) {
    sum += (unsigned int) key[i];
  }
  return sum % modulus;
}

// FNV hash function found at
// http://www.isthe.com/chongo/tech/comp/fnv/index.html#history

// use u_int32_t ?
#define FNV_32_PRIME ((unsigned int)0x01000193)
#define FNV1_32_INIT ((unsigned int)0x811c9dc5)

unsigned int
FNV_1a(const char *str, unsigned int modulus) {
  unsigned int hval = FNV1_32_INIT;
  while (*str) {
    hval ^= (unsigned int)*str++;
    hval *= FNV_32_PRIME;
  }
  return hval % modulus;
}

unsigned int
FNV_1a_seed(const char *str, unsigned int seed, unsigned int modulus) {
  unsigned int hval = seed;
  while (*str) {
    hval ^= (unsigned int)*str++;
    hval *= FNV_32_PRIME;
  }
  return hval % modulus;
}

unsigned int
FNV_1a_byte(const char *str, unsigned int modulus) {
  unsigned int hval = FNV1_32_INIT;
  while (*str) {
    hval ^= (unsigned int)*str++;
    hval *= FNV_32_PRIME;
  }
  hval = (hval >> 16) ^ (hval & 0xffff);
  hval = (hval >> 8) ^ (hval & 0xff);
  return hval % modulus;
}

