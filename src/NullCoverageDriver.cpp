#include "NullCoverageDriver.h"

bool testing::coverage::NullCoverageDriver::isAvailable( std::string & ) const {
  return true;
}

const std::string testing::coverage::NullCoverageDriver::getName() const {
  return "null";
}

void testing::coverage::NullCoverageDriver::startProgram() {
}

void testing::coverage::NullCoverageDriver::beginTest() {
}

void testing::coverage::NullCoverageDriver::endTest( const testing::coverage::TestInfoPtr &, const testing::coverage::CoverageDataPtr & ) {
}

void testing::coverage::NullCoverageDriver::endProgram() {
}
