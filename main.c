#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

uint32_t crc32_slice4_zlib(u_char *buf, size_t len);
uint32_t crc32_lookup(u_char *buf, size_t len);
uint32_t crc32_slice4_brumme(u_char *buf, size_t len);
uint32_t crc32_bitwise(u_char *buf, size_t len);

#if defined(__i386__)||defined(__X86_64__)||defined(__x86_64__)||defined(__ia64__)

__inline__ uint64_t rdtsc(void) {
  uint32_t lo, hi;
  __asm__ __volatile__ (      // serialize
  "xorl %%eax,%%eax \n        cpuid"
  ::: "%rax", "%rbx", "%rcx", "%rdx");
  /* We cannot use "=A", since this would use %rax on x86_64 and return only the lower 32bits of the TSC */
  __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
  return (uint64_t)hi << 32 | lo;
}

#elif defined(__aarch64__)

// https://www.mail-archive.com/linux-kernel@vger.kernel.org/msg2305555.html
__inline__ uint64_t rdtsc(void)
{
    uint64_t val;

    /*
     * According to ARM DDI 0487F.c, from Armv8.0 to Armv8.5 inclusive, the
     * system counter is at least 56 bits wide; from Armv8.6, the counter
     * must be 64 bits wide.  So the system counter could be less than 64
     * bits wide and it is attributed with the flag 'cap_user_time_short'
     * is true.
     */
    asm volatile("mrs %0, cntvct_el0" : "=r" (val));

    return val;
}

#endif

uint64_t measure(uint32_t(*cb)(u_char*, size_t), u_char *data, size_t len) {
  uint64_t start, stop;
  const int num_iter = 1024;

  start = rdtsc();

  for(int i=0; i<num_iter; i++)
    cb(data, len);

  stop = rdtsc();

  return((stop - start) / num_iter);
}

int main() {
  uint8_t data[1500];
  int len = sizeof data;

  printf("crc_bitwise:\t\t%lu ticks\n", measure(crc32_bitwise, data, len));
  printf("crc32_lookup:\t\t%lu ticks\n", measure(crc32_lookup, data, len));
  printf("crc32_slice4_zlib:\t%lu ticks\n", measure(crc32_slice4_zlib, data, len));
  printf("crc32_slice4_brumme:\t%lu ticks\n", measure(crc32_slice4_brumme, data, len));
}
