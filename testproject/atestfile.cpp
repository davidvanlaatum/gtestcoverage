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
  if ( param ) {
    std::cout << "";
  }
  return 0;
}

int atestfile::unusedFunction( int param ) {
  if ( param == 1 ) {
    std::cout << "Hi all";
  } else if ( param == 2 ) {
    std::cout << "Bye all";
  } else {
    std::cout << "Hi all";
  }
  return 0;
}
