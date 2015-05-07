#include <cstdio>
#include <cstdlib>

// Use MEM_FRAC_NUMERATOR / MEM_FRAC_DENOMINATOR worth of total RAM
#define MEM_FRAC_NUMERATOR 1
#define MEM_FRAC_DENOMINATOR 2

extern size_t getMemorySize();
void allocateMemory(int**,size_t*);

int main(int argc, char** argv) {
  int *integers;
  size_t integer_length;

  allocateMemory(&integers,&integer_length);
  int i;
  for(i=0;integer_length>(size_t)i;i++) {
    printf("%d",integers[i]);
  }

  //Free up our arrays
  free(integers);
}

void allocateMemory(int **integers,size_t *size) {
  size_t available_memory,
         int_array_len;

  // Compute size of our array
  available_memory=(getMemorySize()*MEM_FRAC_NUMERATOR)/MEM_FRAC_DENOMINATOR;
  int_array_len=available_memory/sizeof(int); //forces to match integer length

  //Create our array
  *integers=(int*)calloc(int_array_len,sizeof(int));

  //Print out statistics about our array
  printf("Available System Memory: %zu bits\n",available_memory*8);
  printf("Creating array of %zu bits\n",available_memory*8);
  printf("Exploring values in range [%zu,%zu]\n",(size_t)1,available_memory*16+2);

  *size=int_array_len;
}
