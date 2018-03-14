#include "GCCCoverageDriver.h"

bool testing::coverage::gcc::GCCCoverageDriver::isAvailable( std::string & ) const {
  return true;
}

const std::string testing::coverage::gcc::GCCCoverageDriver::getName() const {
  return "gcc";
}

void testing::coverage::gcc::GCCCoverageDriver::startProgram() {
}

void testing::coverage::gcc::GCCCoverageDriver::beginTest() {
}

void testing::coverage::gcc::GCCCoverageDriver::endTest( const testing::coverage::TestInfoPtr &, const testing::coverage::CoverageDataPtr & ) {
}

void testing::coverage::gcc::GCCCoverageDriver::endProgram() {
}
