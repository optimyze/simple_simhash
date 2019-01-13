# simple_simhash
A pure ANSI-C implementation of calculating a SimHash over 4-byte tuples
(including multiplicities) for a given byte stream. Simple and reasonably fast,
no dynamic memory allocations (outside of some stack usage).

Calculates a 256-bit hash for the input stream. Two input streams can be
compared to each other by calculating the hamming distance between the hashes;
this is just a matter of XOR and popcount.

The output approximates the cosine similarity --
S = |W1 \cap W2| / sqrt(|W1||W2|). The sets W1 and W2 in this scenario are the
4-byte sequences encountered in the input buffers, where the n-th occurrence of
the same sequence is treated as distinct from the n+1-th occurrence.

Let's calculate through an example:

```
Buffer1: ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz
Buffer2: ABCDEFGHIJKLMNOPQRSTUVWXYZ____abcdefghijklmnopqrstuvwxyz
```

There are 52 4-tuples in the first buffer, and 56 4-tuples in the second buffer.

Buffer2 does not contain the tuples ```XYZa, YZab, Zabc``` which are present in
Buffer1, and Buffer1 does not contain the tuples ```XYZ_, YZ__, Z___, ____,
___a, __ab, _abc``` which are present in Buffer2.

This means that the cosine similarity is: 49 / sqrt(53 * 56) = 49 / sqrt(2968) =
49 / 54 ~= 0.899. The distance is hence a bit more than 0.1.

Let's have a look:
```
$ ./simhash_compare ./testfile1.txt ./testfile2.txt 
95e24d0f855f73aa-563488dc5716e32f-0095f987c0616a19-0be46dc9599106b8
d5c04d0e865f73aa-deb4885d5636ea27-1691f983c0616a19-1bc1edea59938679
Hamming Distance: 34 - 0.13
```

Seems to work.

The code itself is well-suited to be embedded into kernel drivers etc. - the
runtime is predictable in terms of the number of bytes that need to be processed,
outside of some stack use no dynamic allocations can happen, there is no
recursion and no other unwanted surprises.

