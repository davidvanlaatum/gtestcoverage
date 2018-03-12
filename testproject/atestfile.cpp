#include "atestfile.h"
#include <iostream>

int atestfile::aFunction( const char *param ) {
  if ( param ) {
    return 0;
  } else {
    return 1;
  }
}

int atestfile::aFunction( int param ) {
  int i;
  i = 1;
  if ( param ) {
    std::cout << "";
  }
  return 0;
}
