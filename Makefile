all: simhash_fingerprint simhash_compare

simhash_fingerprint:
	$(CC) -O3 -ggdb simhash_fingerprint.c simple_simhash.c counting_bloom_filter.c trivial_hash.c -fsanitize=address -o simhash_fingerprint

simhash_compare:
	$(CC) -O3 -ggdb simhash_compare.c simple_simhash.c counting_bloom_filter.c trivial_hash.c -o simhash_compare

clean:
	$(RM) simple_simhash.o counting_bloom_filter.o trivial_hash.o simhash_fingerprint simhash_compare



