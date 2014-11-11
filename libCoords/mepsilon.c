// ================================================================
// Filename:    mepsilon.c
// Description: Calculates macine epsilon.
//              from http://en.wikipedia.org/wiki/Machine_epsilon
// Author:      L.R. McFarland, lrm@starbug.com
// Created:     10 May 2009
// Language:    C
//
// TODO:        use this to set string precision during build.
// ================================================================

#include <stdio.h>
 
int main( int argc, char **argv ) {

  float machEps = 1.0f;

  printf("current Epsilon, 1 + current Epsilon\n");
  do {
     printf( "%G\t%.30f\n", machEps, (1.0f + machEps) );
     machEps /= 2.0f;
     // If next epsilon yields 1, then break, because current
     // epsilon is the machine epsilon.
  }
  while ((float)(1.0 + (machEps/2.0)) != 1.0);

  printf("\nCalculated float Machine epsilon: %G\n", machEps);

  printf("===== double =====");

  double machEpsl = 1.0l;

  printf("current Epsilon, 1 + current Epsilon\n");
  do {
     printf( "%G\t%.60Lf\n", machEpsl, (1.0l + machEpsl) );
     machEpsl /= 2.0l;
     // If next epsilon yields 1, then break, because current
     // epsilon is the machine epsilon.
  }
  while ((double)(1.0 + (machEpsl/2.0)) != 1.0);

  printf("\nCalculated double Machine epsilon: %G\n", machEpsl);

  return 0;
}

