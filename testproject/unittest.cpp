#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "atestfile.h"

using namespace testing;

class unittest : public ::testing::Test {
  void SetUp() override {
    RecordProperty( "COVERS", "atestfile.cpp:atestfile.h" );
  }
};

TEST_F( unittest, Test1 ) {
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
