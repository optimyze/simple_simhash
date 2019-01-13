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
#ifndef __COUNTING_BLOOM_FILTER_H
#define __COUNTING_BLOOM_FILTER_H

#include <stdint.h>

#define BLOOM_FILTER_SIZE 2048
#define NUMBER_OF_BLOOM_HASHES 10

uint64_t hash_value(uint64_t value_to_hash, uint64_t hash_function_index);

/* Initialize a 2048-element buffer of unsigned 8-bit integers to be all-zero */
void initialize_counting_bloom_filter(uint16_t* filter_buffer);

/* Increment the counters in the bloom filter for 'element_to_count'. Returns
 * the current count for 'element_to_count' after increment. */
uint16_t increment_count(uint16_t* filter_buffer, uint64_t element_to_count);

/* Read the counter for 'element_to_count' without updating the filter. */
uint16_t read_count(uint16_t* filter_buffer, uint64_t element_to_read_count);

#endif /* __COUNTING_BLOOM_FILTER_H */
