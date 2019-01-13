/* Copyright 2019 optimyze.cloud AG. All Rights Reserved.

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/

/*
 * C implementation of SimHashing two byte streams, using no floating point
 * arithmetic and no memory allocations. This should allow easy embedding into
 * kernel mode libraries and minimal footprint.
 *
 * The SimHash will reflect the cosine distance between the sets of 4-byte-grams
 * including multiplicities.
 *
 * The fuzzy hash will have 256 bits and be returned by writing to 4 uint64_ts.
 */

#include <stdint.h>
#include <string.h>
#include "trivial_hash.h"
#include "counting_bloom_filter.h"

#include "simple_simhash.h"

uint64_t convert_64_counters_to_64_bit(int32_t* counters) {
  uint64_t result = 0;
  for (uint64_t index = 0; index < 64; ++index) {
    if( counters[index] > 0) {
      result = result | (1LL << index);
    }
  }
  return result;
}

void increment_if_bit_set(int32_t* counters, uint64_t index, uint64_t value) {
  if (value) {
    counters[index]++;
  } else {
    counters[index]--;
  }
}

int simple_simhash(uint8_t* input, size_t number_of_bytes, uint64_t* result_1,
  uint64_t* result_2, uint64_t* result_3, uint64_t* result_4) {
  int32_t counters_1[64];
  int32_t counters_2[64];
  int32_t counters_3[64];
  int32_t counters_4[64];
  uint16_t bloom_filter_buffer[BLOOM_FILTER_SIZE];

  memset(counters_1, 0, sizeof(int32_t) * 64);
  memset(counters_2, 0, sizeof(int32_t) * 64);
  memset(counters_3, 0, sizeof(int32_t) * 64);
  memset(counters_4, 0, sizeof(int32_t) * 64);

  initialize_counting_bloom_filter(bloom_filter_buffer);

  if (number_of_bytes <= 4) {
    /* Return failure, not enough data to reasonably hash. */
    return -1;
  }

  for (uint64_t index = 0; index < number_of_bytes - 4; ++index) {
    /* Read the 4-byte tuple */
    uint32_t value = input[index] | input[index+1] << 8 | input[index+2] << 16 |
      input[index+3] << 24;
    /* Increment the counter in the bloom filter */
    uint16_t count = increment_count(bloom_filter_buffer, value);

    /* If a value has occurred more than 0xFFFF times, ignore it from now on. */
    if (count == 0xFFFF) {
      continue;
    }

    uint64_t value_with_count = count;
    value_with_count << 32;
    value_with_count |= value;

    /* Generate a 256-bit hash from this value */
    uint64_t hash_1 = hash_value(value_with_count, 1);
    uint64_t hash_2 = hash_value(value_with_count, 2);
    uint64_t hash_3 = hash_value(value_with_count, 3);
    uint64_t hash_4 = hash_value(value_with_count, 4);

    /* Increment counters for each value. */
    for (uint64_t test_bit_index = 0; test_bit_index < 64; ++test_bit_index) {
      uint64_t test_bit = 1ULL << test_bit_index;

      increment_if_bit_set(counters_1, test_bit_index, test_bit & hash_1);
      increment_if_bit_set(counters_2, test_bit_index, test_bit & hash_2);
      increment_if_bit_set(counters_3, test_bit_index, test_bit & hash_3);
      increment_if_bit_set(counters_4, test_bit_index, test_bit & hash_4);
    }
  }
  /* Convert the array of counters into hash values again. */
  *result_1 = convert_64_counters_to_64_bit(counters_1);
  *result_2 = convert_64_counters_to_64_bit(counters_2);
  *result_3 = convert_64_counters_to_64_bit(counters_3);
  *result_4 = convert_64_counters_to_64_bit(counters_4);
  return 0;
}





