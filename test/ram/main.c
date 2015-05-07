#include <stdio.h>
#include <iostream>

extern size_t getMemorySize();

int main(int argc, char** argv) {
  size_t memory = getMemorySize() / 2;
  size_t values = memory / sizeof(int);
  printf("memory: %zu\nbits",memory);
  printf("array length: %zu\nbits",values);
  printf("Integer Range: [%zu,%zu]",(size_t)1,memory*8*2);
  return 0;
}
