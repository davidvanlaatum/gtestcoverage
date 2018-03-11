#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "atestfile.h"

using namespace testing;

class unittest : public ::testing::Test {
  void SetUp() override {
    RecordProperty( "COVERS", "atestfile.cpp:atestfile.h" );
  }
};

TEST_F( unittest, Test ) {
  atestfile a;
  a.aFunction( "abc123" );
}
