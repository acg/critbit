#include <string>
#include <set>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "critbit.h"
#include "benchmark-data.h"

using namespace std;


int test_critbit0( critbit0_tree *crit, const char **words, int times )
{
  int i;
  int count = 0;
  const char **w;

  for (i=0; i<times; i++)
    for (w=words, count=0; *w; w++) {
      if (critbit0_contains( crit, *w ))
        count++;
    }

  return count;
}


int test_stdset( set<string>& stdset, const char **words, int times )
{
  int i;
  int count = 0;
  const char **w;

  for (i=0; i<times; i++)
    for (w=words, count=0; *w; w++) {
      if (stdset.find( *w ) == stdset.end())
        count++;
    }

  return count;
}


const char *stopwords[] = {
  "a", "able", "about", "across", "after", "all", "almost", "also", "am",
  "among", "an", "and", "any", "are", "as", "at", "be", "because",
  "been", "but", "by", "can", "cannot", "could", "dear", "did", "do",
  "does", "either", "else", "ever", "every", "for", "from", "get", "got",
  "had", "has", "have", "he", "her", "hers", "him", "his", "how",
  "however", "i", "if", "in", "into", "is", "it", "its", "just", "least",
  "let", "like", "likely", "may", "me", "might", "most", "must", "my",
  "neither", "no", "nor", "not", "of", "off", "often", "on", "only",
  "or", "other", "our", "own", "rather", "said", "say", "says", "she",
  "should", "since", "so", "some", "than", "that", "the", "their",
  "them", "then", "there", "these", "they", "this", "tis", "to", "too",
  "twas", "us", "wants", "was", "we", "were", "what", "when", "where",
  "which", "while", "who", "whom", "why", "will", "with", "would", "yet",
  "you", "your",
  0
};


int testnum;
int times;

critbit0_tree crit;
set<string> stdset;

int test_critbit0() { return test_critbit0( &crit, testdata_article, times ); }
int test_stdset() { return test_stdset( stdset, testdata_article, times ); }

typedef struct
{
  const char *name;
  int (*fn)(void);
}
benchmark_test;

benchmark_test tests[] =
{
  { "critbit0", test_critbit0 },
  { "std::set", test_stdset },
};


int main( int argc, char **argv )
{
  if (argc != 3) {
    fprintf( stderr, "usage: %s testnum times\n", argv[0] );
    exit(1);
  }

  testnum = atoi( argv[1] );
  times = atoi( argv[2] );

  if (testnum < 0 || testnum >= (int)(sizeof(tests)/sizeof(tests[0]))) {
    fprintf( stderr, "bad testnum %d\n", testnum );
    exit(1);
  }

  const char **w;

  for (w=stopwords; *w; w++) {
    critbit0_insert( &crit, *w );
    stdset.insert( *w );
  }

  fprintf( stdout, "test %d: \"%s\" x %d ... ", testnum, tests[testnum].name, times );
  fflush( stdout );

  timeval t1;
  timeval t2;
  timeval elapsed;

  gettimeofday( &t1, 0 );
  tests[testnum].fn();
  gettimeofday( &t2, 0 );

  timersub( &t2, &t1, &elapsed );

  float secs = (float)elapsed.tv_sec + (float)elapsed.tv_usec / 1000000;
  fprintf( stdout, "%.06f\n", secs );

  return 0;
}

