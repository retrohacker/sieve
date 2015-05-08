#include <cstdio>
#include <cstdlib>

// Use MEM_FRAC_NUMERATOR / MEM_FRAC_DENOMINATOR worth of total RAM
#define MEM_FRAC_NUMERATOR 1
#define MEM_FRAC_DENOMINATOR 2

extern size_t getMemorySize();
static inline void loadBar(int,int,int,int);
void allocateMemory(int**,size_t*);
void identifyComposites(int**,size_t,int);

int main(int argc, char** argv) {
  int *integers;
  // Use enough memory to hold integer length provided,
  // else consume 1/2 system memory.
  size_t integer_length=(argc>1)?strtoul(argv[1],NULL,10)/2-1:0;
  int threads=(argc>2)?atoi(argv[2]):1;

  allocateMemory(&integers,&integer_length);
  identifyComposites(&integers,integer_length,threads);

  //Free up our arrays
  free(integers);
}

void identifyComposites(int **integers,size_t size,int threads) {
  size_t i;//,last_prime;
  // Since we don't handle even numbers, we multiple size*2. Since we don't
  // handle 1, we add 1.
  size_t loop_end=size*2;
  size_t seek=sizeof(int)*8;
  //loadBar(0,loop_end,loop_end,50);
  for(i=3;i<loop_end;i+=2) {
    //loadBar(i,loop_end,100,50);
    // We need to check if i is prime. floor(i/2)-1 is the index of current bit.
    size_t bit=i/2-1,
           integer=bit/seek, //in array of integers
           offset=bit%seek;  //in bit of integer
    // Check that bit is still 0
    if(!(((*integers)[integer]>>offset)&1U)) {
      //printf("%zu is prime\n",i);
      //last_prime=i; //keep track of highest prime number we have encountered
      // Set all factors of i > i^2 to 1
      // All factors if i < i^2 are handled by other primes
      // Ignore multiples of 2.
      size_t jstart = (i*i%2==0)?i*i+i:i*i;
# pragma omp parallel for num_threads(threads) schedule(static)
      for(size_t j=jstart;j<loop_end;j+=i*2) {
        size_t jbit=j/2-1,
               jinteger=jbit/seek, //in array of integers
               joffset=jbit%seek;  //in bit of integer
        (*integers)[jinteger]|=1U<<joffset;
      }
    }
  }
  //printf("The highest prime number we computed was: %zu\n",last_prime);
}

void allocateMemory(int **integers,size_t *size) {
  size_t available_memory,
         array_bytes,
         int_array_len;

  // Compute size of our array
  available_memory=getMemorySize();
  if(*size==0) { //size not supplied
    array_bytes=(available_memory*MEM_FRAC_NUMERATOR)/MEM_FRAC_DENOMINATOR;
  } else {
    array_bytes=*size/8;
  }
  int_array_len=array_bytes/sizeof(int)+1; //forces to match integer length

  //Create our array
  *integers=(int*)calloc(int_array_len,sizeof(int));
  *size=(*size==0)?array_bytes*8:*size;

  //Print out statistics about our array
  //printf("Available System Memory: %zu bits\n",available_memory*8);
  //printf("Creating array of %zu bits\n",int_array_len*sizeof(int));
  //printf("Exploring values in range [%zu,%zu]\n",(size_t)1,*size*2+2);

}

// Taken from www.ross.click
// Process has done i out of n rounds,
// and we want a bar of width w and resolution r.
static inline void loadBar(int x, int n, int r, int w) {
    // Only update r times.
    if (x%(n/r+1)!=0) return;
    // Calculuate the ratio of complete-to-incomplete.
    float ratio=x/(float)n;
    int c=ratio*w;
    // Show the percentage complete.
    printf("%3d%% [",(int)(ratio*100));
    // Show the load bar.
    for (int x=0;x<c;x++)
      printf("=");
    for (int x=c;x<w;x++)
      printf(" ");
    // ANSI Control codes to go back to the
    // previous line and clear it.
    printf("]\n\033[F\033[J");
}
