#include <string>
#include <set>
#include <hash_set>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "critbit.h"
#include "benchmark-data.h"

/* https://github.com/j0sh/radixtree */
#define HAVE_RADIXTREE 0

#if HAVE_RADIXTREE
#  include "radix.h"
#endif

using namespace std;
using namespace __gnu_cxx;


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
      if (stdset.find( *w ) != stdset.end())
        count++;
    }

  return count;
}


int test_stdhashset( hash_set<const char*>& stdhashset, const char **words, int times )
{
  int i;
  int count = 0;
  const char **w;

  for (i=0; i<times; i++)
    for (w=words, count=0; *w; w++) {
      if (stdhashset.find( *w ) != stdhashset.end())
        count++;
    }

  return count;
}


#if HAVE_RADIXTREE
int test_rxtree( rxt_node *rxtree, const char **words, int times )
{
  int i;
  int count = 0;
  const char **w;

  for (i=0; i<times; i++)
    for (w=words, count=0; *w; w++) {
      if (rxt_get( (char*)*w, rxtree ))
        count++;
    }

  return count;
}
#endif // HAVE_RADIXTREE


int testnum;
int times;

critbit0_tree crit = {0};
set<string> stdset;
hash_set<const char*> stdhashset;
#if HAVE_RADIXTREE
rxt_node *rxtree = 0;
#endif

int test_critbit0() { return test_critbit0( &crit, testdata_article, times ); }
int test_stdset() { return test_stdset( stdset, testdata_article, times ); }
int test_stdhashset() { return test_stdhashset( stdhashset, testdata_article, times ); }
#if HAVE_RADIXTREE
int test_rxtree() { return test_rxtree( rxtree, testdata_article, times ); }
#endif

typedef struct
{
  const char *name;
  int (*fn)(void);
}
benchmark_test;

benchmark_test tests[] =
{
  { "critbit0",  test_critbit0 },
  { "std::set",  test_stdset },
  { "__gnu_cxx::hash_set",  test_stdhashset },
#if HAVE_RADIXTREE
  { "radixtree", test_rxtree },
#endif
};


int main( int argc, char **argv )
{
  /* Argument processing */

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

  /* Initialize tests */

#if HAVE_RADIXTREE
  rxtree = rxt_init();
#endif

  const char **w;

  for (w=testdata_stopwords; *w; w++)
  {
    critbit0_insert( &crit, *w );
    stdset.insert( *w );
    stdhashset.insert( *w );
#if HAVE_RADIXTREE
    rxt_put( (char*)*w, (char*)*w, rxtree );
#endif
  }

  /* Run test */

  fprintf( stdout, "test %d: \"%s\" x %d ... ", testnum, tests[testnum].name, times );
  fflush( stdout );

  timeval t1;
  timeval t2;
  timeval elapsed;

  gettimeofday( &t1, 0 );
  int rv = tests[testnum].fn();
  gettimeofday( &t2, 0 );

  timersub( &t2, &t1, &elapsed );

  float secs = (float)elapsed.tv_sec + (float)elapsed.tv_usec / 1000000;
  fprintf( stdout, "= %d %.06f\n", rv, secs );

  /* Free tests */

  critbit0_clear( &crit );
#if HAVE_RADIXTREE
  // rxt_free( rxtree ); // FIXME invalid free()
#endif

  return 0;
}

