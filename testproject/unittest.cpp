#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "atestfile.h"
#include <GTestCoverageCovers.h>

using namespace testing;

class unittest : public ::testing::Test {
  void SetUp() override {
    COVERS_FILES( "atestfile.cpp:atestfile.h" );
  }
};


TEST_F( unittest, Test1 ) {
  COVERS_BEGIN_FUNCTIONS
    COVERS_CLASS_FUNCTION( atestfile, aFunction, int(
        const char *) );
    COVERS_CLASS_FUNCTION_T( atestfile, aTestFunction, void( int ), int );
    COVERS_CLASS_FUNCTION_T( atestfile, aTestFunction, void(
        const char* ), const char* );
  COVERS_END_FUNCTIONS

  atestfile a;
  a.aFunction( "abc123" );
  a.aTestFunction( "abc123" );
  a.aTestFunction( 123 );
}

TEST_F( unittest, Test2 ) {
  atestfile a;
  a.aFunction( nullptr );
  a.aTestFunction( 0 );
}

TEST_F( unittest, Test3 ) {
  FUNCTION
#if 0
  abc
#endif
}
