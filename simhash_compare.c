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

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "simple_simhash.h"

char g_usage[] = "Usage: ./simhash_compare [filename 1] [filename 2]\n";

void hash_file(const char* filename, uint64_t* hash_1, uint64_t* hash_2,
  uint64_t* hash_3, uint64_t* hash_4) {

  /* Load, map and hash the first file. */
  FILE* input = fopen(filename, "rb");
  if (!input) {
    fprintf(stderr, "[E] Failure to open input file.");
    exit(-1);
  }

  fseek(input, 0, SEEK_END);
  uint64_t size = ftell(input);
  uint8_t* buffer = malloc(size);
  if (!buffer) {
    fprintf(stderr, "[E] Failure to allocate buffer for file.");
    exit(-1);
  }
  rewind(input);
  fread(buffer, size, 1, input);
  fclose(input);

  simple_simhash(buffer, size, hash_1, hash_2, hash_3, hash_4);
  free(buffer);
}

int main(int argc, char** argv) {
  if (argc != 3) {
    printf(g_usage);
    exit(-1);
  }

  uint64_t hash_1, hash_2, hash_3, hash_4;
  hash_file(argv[1], &hash_1, &hash_2, &hash_3, &hash_4);
  uint64_t hashB_1, hashB_2, hashB_3, hashB_4;
  hash_file(argv[2], &hashB_1, &hashB_2, &hashB_3, &hashB_4);

  fprintf(stdout, "%16.16" PRIx64 "-%16.16" PRIx64 "-%16.16" PRIx64 "-%16.16"
    PRIx64 "\n", hash_1, hash_2, hash_3, hash_4);
  fprintf(stdout, "%16.16" PRIx64 "-%16.16" PRIx64 "-%16.16" PRIx64 "-%16.16"
    PRIx64 "\n", hashB_1, hashB_2, hashB_3, hashB_4);

  uint64_t hamming_distance = __builtin_popcountll(hash_1 ^ hashB_1) +
    __builtin_popcountll(hash_2 ^ hashB_2) +
    __builtin_popcountll(hash_3 ^ hashB_3) +
    __builtin_popcountll(hash_4 ^ hashB_4);
  fprintf(stdout, "Hamming Distance: %d - %02.02f\n", hamming_distance,
    hamming_distance / 256.0);
  exit(1);
}
