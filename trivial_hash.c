#include <stdint.h>
#include "trivial_hash.h"

/* A list of random 64-bit primes. */
uint64_t g_primes[] = {
 538404559390855907ULL,
 3805450411483076243ULL,
 14708983389484784737ULL,
 17195658974718141529ULL,
 13245550289197498483ULL,
 941416883125616461ULL,
 5612982853315651507ULL,
 4425188701120728451ULL,
 1243719584255937337ULL,
 13226440087791218581ULL,
 5457430076438826907ULL,
 4252286348012823599ULL,
 12192003767930684759ULL,
 6352987505094611413ULL,
 7524584683149350093ULL,
 15024612497464916197ULL,
 5330929197429877007ULL,
 12085737423030207737ULL,
 13047231448031613839ULL,
 17893104092742596429ULL,
 3665815096395488329ULL,
 5164402960129843169ULL,
 2076469291307250377ULL,
 8494890768520301029ULL,
 10484743222427856449ULL,
 11655765399976318403ULL,
 17620121305513338553ULL
};

inline uint64_t rotl64 ( uint64_t x, int8_t r )
{
  return (x << r) | (x >> (64 - r));
}

/* Takes a 64-bit value and a hash function index, and then permutes the value
 * that should be hashed with the hash function of a given index. 
 *
 * On the scale of "hash function sophistication", this is clearly in the quick
 * and dirty realm of quality. Some analysis should be done; it is quite
 * possible that this can be had much faster.
 *
 * */
uint64_t hash_value(uint64_t value_to_hash, uint64_t hash_function_index) {
  uint64_t prime_to_use = g_primes[hash_function_index %
    (sizeof(g_primes)/sizeof(uint64_t))];
  uint64_t prime2_to_use = g_primes[(hash_function_index + 1) %
    (sizeof(g_primes)/sizeof(uint64_t))];

  uint64_t temp = 0;

  temp = value_to_hash + prime_to_use + rotl64(value_to_hash, 7);
  for (int i = 0; i < 16; ++i) {
    temp = temp + prime_to_use + rotl64(temp, 7);
    temp = temp + prime2_to_use + rotl64(temp, 7);
  }
  return temp;
}


