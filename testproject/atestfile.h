#pragma once
#ifndef GTESTCOVERAGE_ATESTFILE_H
#define GTESTCOVERAGE_ATESTFILE_H

class atestfile {
public:
  int aFunction( const char *param );
  int aFunction( int param );

  template<typename T> void aTestFunction( T value ) {
    aFunction( value );
  }
};

#define FUNCTION atestfile a; \
a.aFunction( nullptr );

#endif //GTESTCOVERAGE_ATESTFILE_H
