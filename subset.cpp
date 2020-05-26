// single threaded program that reads in 32-bit integers from
// standard input, and then counds and prints out how many
// subsets of these integers sum up to 0 (zero).
//
// this program uses a trivial exponential time algorithm, by
// testing every possible subset (there are 2^N-1 subsets to
// test for N integers)
//
// the program uses global variables - to make it a bit easier
// to convert to a multi-threaded code for programmers
// that have not used pthreads before

#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <pthread.h>

struct m_arguments
{
  long from;
  long to;
  long pid;
};


// global variables are bad ...
// but they are acceptable for your first mulithreaded program
std::vector<long> a;
std::vector<long> counts;

// for debugging purposes (if you want to print the combinations)
void print_sol(long comb)
{
  return; // comment this out if you want to see the subsets
  
  for(size_t i = 0 ; i < a.size() ; i ++ ) {
    if( comb & 1)
      printf("%ld ", a[i]);
    comb /= 2;
  }
  printf("\n");
}

// test a combination 'comb'
// the binary representation of comb will be used to decide
// which numbers in a[] will be used to test the sum
void test(long comb, long pid)
{
  long long sum = 0;
  long bits = comb;
  for(size_t i = 0 ; i < a.size() ; i ++ ) {
    if( bits & 1) // check lowest bit
      sum += a[i];
    bits >>= 1; // shift bits to the right
  }
  if( sum == 0) {
    print_sol(comb);
    counts[pid]++;
  }
}

// test all combinatsion in range [from, to)
void test_range( long from, long to, long pid)
{
  for( long i = from ; i < to ; i ++)
    test(i, pid);
}

void * test_range_2( void * args) {
  m_arguments * arguments = (m_arguments *) args; 
  // printf("from %ld, to %ld, pid %ld\n", arguments -> from, arguments -> to, arguments -> pid);
  long from = arguments -> from;
  long to = arguments -> to;
  long pid = arguments -> pid;
  test_range(from, to, pid);
  pthread_exit(0);
}

int main( int argc, char ** argv)
{
  //
  // parse command line arguments
  //
  if( argc > 2) {
    printf("Error: expected at most 1 command line argument.\n");
    return -1;
  }
  long nThreads = 1;
  if( argc > 1) {
    // convert argv[1] to 'nThreads', report error if failed
    char * end = 0;
    errno = 0;
    nThreads = strtol( argv[1], & end, 10);
    if( * end != 0 || errno != 0) {
      printf("Expected integer, but you gave me '%s'\n", argv[1]);
      return -1;
    }

    if (nThreads <= 0) {
      printf("Number of threads must be greater than 0! \n");
      return -1;
    }
  }

  //
  // read all integers one by one and put them into the array
  //
  while(1) {
    long n;
    if( 1 != scanf("%ld", & n)) break;
    a.push_back(n);
  }

  unsigned long n = a.size();
  // debug message
  printf( "Using %ld thread(s) on %lu numbers.\n",
	  nThreads, n);

  //
  // to convert this into multithreaded code, you should
  // create threads here, and make each thread count different
  // subsets
  //
  // to make the code work without synchronization mechanisms,
  // you should make separate counters for each thread
  //

  long max = long(1) << n;
  // printf("max: %ld, ", max);

  if (nThreads > max) {
    printf("Number of threads is too big! \n");
    return -1;
  }

  if (nThreads == 1) {
    counts.push_back(0);
    test_range(1, max, 0); // range = 1 .. 2^N using bitshift
  } else {
    for (long c = 0; c < nThreads; c++) counts.push_back(0);
    unsigned long numsPerThread = max / nThreads;
    // printf("nums per thread: %lu\n", numsPerThread);

    pthread_t threads[nThreads];
    long from = 1;
    struct m_arguments args[nThreads];
    for (long i = 0; i < nThreads; i++) {
      args[i].from = from;
      args[i].pid = i;

      long to = from + numsPerThread;
      if (to > max || (i == nThreads - 1 && i < max)) {
        args[i].to = max;
      } else {
        args[i].to = to;
      }

      long status = pthread_create(&threads[i], NULL, test_range_2, &args[i]);
      if (status != 0) {
        printf("Oops, pthread_create returned error code %ld\n", status);
        exit(-1);
      }
      from += numsPerThread;
    }

    for (long j = 0; j < nThreads; j++) {
      pthread_join(threads[j], NULL);
    }
  }

  //
  // once you join the threads, you can simply sum up the counters
  // to get the total count
  //
  // since this version does not use multiple counters, report the only
  // one we have
  //
  long count = 0;
  for (long t = 0; t < nThreads; t++) {
    count += counts[t];
  }
  printf("Subsets found: %ld\n", count);

  return 0;
}
