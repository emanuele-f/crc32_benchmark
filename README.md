This repository contains different implementations of the CRC-32 error-detecting code.
All the implementations are adapted to remove external references so that they can be contained into a single file.

The rationale about this repo is to determine a dependency-free implementation of CRC-32 with a good tradeoff between performance, complexy and memory consumption.

CRC-32 should not be confused with the CRC-32C.
CRC-32 is the one used to store the Ethernet FCS field and uses the `0xEDB88320` polynomial.
CRC-32C, on the other hand, uses the `0x82F63B78` polynomial, for which exist the x86/x64 and ARM64 optimizations.

The implementations tested are:
  - `crc32_bitwise`: the simplest, but slowest algorithm
  - `crc32_lookup`: enhanced CRC32 calculation by zlib which uses a 1-byte lookup table
  - `crc32_slice4_zlib`: enhanced CRC32 calculation via the Slicing-by-4 algorithm
  - `crc32_slice4_brumme`: Slicing-by-4 algorithm variant by Stephan Brumme

The `main` program performs the performance test of the different implementations:

```
crc_bitwise:		85766 ticks
crc32_lookup:		20658 ticks
crc32_slice4_zlib:	7034 ticks
crc32_slice4_brumme:	7131 ticks
```

The slicing-by-4 algorithmn is, as expected, the winner. The Brumme and zlib implementations are mostly equivalent, however the Brumme implementation is better in these regards:

- uses a lookup table of `4 KB` in contrast of the `8 KB` for the zlib implementation
- the code is compact and clean

*Note*: as benchmarked in https://create.stephan-brumme.com/crc32, the slicing-by-8 (not benchmarked here) algorithm is slighly more performant of slicing-by-4. However it is more complex and requires more memory.
