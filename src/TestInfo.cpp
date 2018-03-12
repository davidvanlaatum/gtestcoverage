#include "TestInfo.h"
#include "FileInfo.h"
#include "LineInfo.h"
#include <iostream>
#include <utility>

using namespace testing::coverage;

TestInfo::TestInfo( std::string nSuite, std::string nName, coveredFilesType nCoveredFiles, bool nPassed ) :
    suite( std::move( nSuite ) ), name( std::move( nName ) ), coveredFiles( std::move( nCoveredFiles ) ), passed( nPassed ) {}

const std::string &TestInfo::getName() const {
  return name;
}

const std::string &TestInfo::getSuite() const {
  return suite;
}

void TestInfo::addLine( const LineInfoPtr &line ) {
  covered[line->getFile()->getSource()] += 1;
}

std::ostream &testing::coverage::operator<<( std::ostream &os, const TestInfo &test ) {
  os << test.getSuite() << "::" << test.getName() << " covers:";
  for ( const auto &it : test.covered ) {
    os << std::endl << it.first << ": " << it.second;
  }
  return os;
}
