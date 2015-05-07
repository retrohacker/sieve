#include <cstdio>
#include <cstdlib>

// Use MEM_FRAC_NUMERATOR / MEM_FRAC_DENOMINATOR worth of total RAM
#define MEM_FRAC_NUMERATOR 1
#define MEM_FRAC_DENOMINATOR 2

extern size_t getMemorySize();
void allocateMemory(int**,size_t*);
void identifyComposites(int**,size_t);

int main(int argc, char** argv) {
  int *integers;
  size_t integer_length;

  allocateMemory(&integers,&integer_length);
  identifyComposites(&integers,integer_length);

  //Free up our arrays
  free(integers);
}

void identifyComposites(int **integers,size_t size) {
  size_t i,j,last_prime;
  // Since we don't handle even numbers, we multiple size*2. Since we don't
  // handle 1, we add 1.
  // size_t loop_end=size*2+1;
  size_t loop_end=1000;
  size_t seek=sizeof(int)*8;
  for(i=3;i<loop_end;i+=2) {
    // We need to check if i is prime. floor(i/2)-1 is the index of current bit.
    size_t bit=i/2-1,
           integer=bit/seek, //in array of integers
           offset=bit%seek;  //in bit of integer
    // Check that bit is still 0
    if(!(((*integers)[integer]>>offset)&1U)) {
      printf("%zu is prime\n",i);
      last_prime=i; //keep track of highest prime number we have encountered
      // Set all factors of i > i^2 to 1
      // All factors if i < i^2 are handled by other primes
      // Ignore multiples of 2.
      size_t jstart = (i*i%2==0)?i*i+i:i*i;
# pragma omp parallel for schedule(static) private(j)
      for(j=jstart;j<loop_end;j+=i*2) {
        size_t jbit=j/2-1,
               jinteger=jbit/seek, //in array of integers
               joffset=jbit%seek;  //in bit of integer
        (*integers)[jinteger]|=1U<<joffset;
      }
    }
  }
  printf("The highest prime number we computed was: %zu\n",last_prime);
}

void allocateMemory(int **integers,size_t *size) {
  size_t available_memory,
         array_bytes,
         int_array_len;

  // Compute size of our array
  available_memory=getMemorySize();
  array_bytes=(available_memory*MEM_FRAC_NUMERATOR)/MEM_FRAC_DENOMINATOR;
  int_array_len=array_bytes/sizeof(int); //forces to match integer length

  //Create our array
  *integers=(int*)calloc(int_array_len,sizeof(int));

  //Print out statistics about our array
  printf("Available System Memory: %zu bits\n",available_memory*8);
  printf("Creating array of %zu bits\n",array_bytes*8);
  printf("Exploring values in range [%zu,%zu]\n",(size_t)1,array_bytes*16+2);

  *size=array_bytes*8;
}
