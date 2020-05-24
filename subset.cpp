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

// global variables are bad ...
// but they are acceptable for your first mulithreaded program
std::vector<long> a;
long count = 0;

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
void test(long comb)
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
    count ++;
  }
}

// test all combinatsion in range [from, to)
void test_range( long from, long to)
{
  for( long i = from ; i < to ; i ++)
    test(i);
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
  }
  if( nThreads > 1) {
    printf("Sorry, I don't know how to use multiple threads yet.\n");
    nThreads = 1;
  }

  //
  // read all integers one by one and put them into the array
  //
  while(1) {
    long n;
    if( 1 != scanf("%ld", & n)) break;
    a.push_back(n);
  }

  // debug message
  printf( "Using %ld thread(s) on %lu numbers.\n",
	  nThreads, a.size());

  //
  // to convert this into multithreaded code, you should
  // create threads here, and make each thread count different
  // subsets
  //
  // to make the code work without synchronization mechanisms,
  // you should make separate counters for each thread
  //
  count = 0;
  test_range(1, long(1) << a.size()); // range = 1 .. 2^N using bitshift

  //
  // once you join the threads, you can simply sum up the counters
  // to get the total count
  //
  // since this version does not use multiple counters, report the only
  // one we have
  //
  printf("Subsets found: %ld\n", count);

  return 0;
}
