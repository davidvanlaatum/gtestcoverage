#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <GTestCoverageListener.h> // IWYU pragma: keep

int main( int argc, char *argv[] ) {
  testing::InitGoogleMock( &argc, argv );
  ::testing::TestEventListeners &listeners = ::testing::UnitTest::GetInstance()->listeners();
  listeners.Append( new testing::coverage::GTestCoverageListener() );
  int exit = RUN_ALL_TESTS();
  if ( getenv( "IGNORE_EXIT_CODE" ) != nullptr ) {
    return 0;
  }
  return exit;
}
