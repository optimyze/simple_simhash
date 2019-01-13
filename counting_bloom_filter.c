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
 * C implementation of a very simple counting bloom filter that counts the
 * number of times a given 64-bit value occurred, saturating at 255. Constant
 * memory usage, zero allocations.
 */

#include <stdint.h>
#include <string.h>
#include "trivial_hash.h"

#include "counting_bloom_filter.h"

/* Expects filter_buffer to point to an array of unsigned chars of
 * BLOOM_FILTER_SIZE elements and initializes it to zeroes. */
void initialize_counting_bloom_filter(uint16_t* filter_buffer) {
  memset(filter_buffer, 0, BLOOM_FILTER_SIZE*sizeof(uint16_t));
}

/* Increments the count for the element to count. Also returns the value. */
uint16_t increment_count(uint16_t* filter_buffer, uint64_t element_to_count) {
  uint16_t result = 0xFFFF;
  for (uint64_t hash_index = 0; hash_index < NUMBER_OF_BLOOM_HASHES;
    ++hash_index) {
    uint64_t hashed_value = hash_value(element_to_count, hash_index) %
      BLOOM_FILTER_SIZE;
    if (filter_buffer[hashed_value] != 0xFFFF) {
      ++filter_buffer[hashed_value];
    }
    if (filter_buffer[hashed_value] < result) {
      result = filter_buffer[hashed_value];
    }
  }
  return result;
}

/* Read the current count for a given element. */
uint16_t read_count(uint16_t* filter_buffer, uint64_t element_to_read_count) {
  uint16_t result = 0xFFFF;
  for (uint64_t hash_index = 0; hash_index < NUMBER_OF_BLOOM_HASHES; 
    ++hash_index) {
    uint64_t hashed_value = hash_value(element_to_read_count, hash_index) %
      BLOOM_FILTER_SIZE;
    if (filter_buffer[hashed_value] < result) {
      result = filter_buffer[hashed_value];
    }
  }
  return result;
}


