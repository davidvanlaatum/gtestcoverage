#include "FunctionInfo.h"
#include "CoverageData.h"
#include "TestCaseInfo.h"  // for TestCaseInfo
#include "TestInfo.h"      // for TestInfo
#include <utility>         // for pair
#include <filesystemserializers.h> // IWYU pragma: keep

using namespace testing::coverage;

FunctionInfo::FunctionInfo( const std::string &nName ) : name( nName ) {}

void testing::coverage::from_json( const nlohmann::json &j, testing::coverage::FunctionInfo &data ) {
  data.sourceFiles = j.value<decltype( data.sourceFiles )>( "files", {} );
  data.hits = j.value<decltype( data.hits )>( "hits", 0 );
  for ( const auto &item : j.value<std::set<std::string>>( "tests", {} ) ) {
    data.tests.emplace( item, TestInfoWeakPtr() );
  }
}

void testing::coverage::to_json( nlohmann::json &j, const testing::coverage::FunctionInfo &data ) {
  j["files"] = data.sourceFiles;
  j["hits"] = data.hits;
  for ( const auto &test : data.tests ) {
    j["tests"].emplace_back( test.first );
  }
}

const std::string &FunctionInfo::getName() const {
  return name;
}

const std::set<path> &FunctionInfo::getSourceFiles() const {
  return sourceFiles;
}

void FunctionInfo::addSourceFile( const path &file ) {
  sourceFiles.emplace( file );
}

void FunctionInfo::addHits( uint32_t count ) {
  hits += count;
}

void FunctionInfo::addCoveringTest( const TestInfoPtr &test ) {
  tests.emplace( test->getTestCase()->getName() + "::" + test->getName(), test );
}

void FunctionInfo::setName( const std::string &nName ) {
  name = nName;
}

void FunctionInfo::readResolve( const CoverageDataPtr &data ) {
  for ( auto &item : tests ) {
    item.second = data->getTestByFullName( item.first );
  }
}

bool FunctionInfo::hasCoverage() const {
  return hits > 0;
}
