#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

typedef unsigned char byte;
typedef int (*function)(void);

int main() {
  int prot = PROT_READ | PROT_WRITE | PROT_EXEC;
  int map = MAP_PRIVATE | MAP_ANON;
  byte* bytes = mmap(0, 6, prot, map, -1, 0);
  
  // 1011 movl 1 width 000 eax
  bytes[0] = 0b10111000;
  // 1129 little endian
  bytes[1] = 0b01101001;
  bytes[2] = 0b00000100;
  bytes[3] = 0b00000000;
  bytes[4] = 0b00000000;
  // ret
  bytes[5] = 0b11000011;
  
  function get1129 = (function)bytes;
  int eleven_twentynine = (*get1129)();

  printf("Working like a charm: %d\n", eleven_twentynine);

  return 0;
}