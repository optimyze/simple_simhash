#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "simple_simhash.h"

char g_usage[] = "Usage: ./simhash_fingerprint [filename]\n";

int main(int argc, char** argv) {
  if (argc != 2) {
    printf(g_usage);
    exit(-1);
  }

  FILE* input = fopen(argv[1], "rb");
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

  uint64_t hash_1, hash_2, hash_3, hash_4;

  simple_simhash(buffer, size, &hash_1, &hash_2, &hash_3, &hash_4);
  free(buffer);

  fprintf(stdout, "%16.16" PRIx64 "-%16.16" PRIx64 "-%16.16" PRIx64 "-%16.16"
    PRIx64 "\n", hash_1, hash_2, hash_3, hash_4);
  exit(1);
}
