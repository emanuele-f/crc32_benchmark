.PHONY: clean

main: main.c crc32_slice4_zlib.c crc32_lookup.c crc32_slice4_brumme.c crc32_bitwise.c
	gcc -Wall -O2 -o $@ $^

clean:
	rm -f main
