#include "FunctionInfo.h"
#include "TestInfo.h"
#include "TestCaseInfo.h"
#include <filesystemserializers.h>

using namespace testing::coverage;

FunctionInfo::FunctionInfo( const std::string &nName ) : name( nName ) {}

//void testing::coverage::from_json( const nlohmann::json &j, testing::coverage::FunctionInfo &data ) {
//}

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

const std::set<boost::filesystem::path> &FunctionInfo::getSourceFiles() const {
  return sourceFiles;
}

void FunctionInfo::addSourceFile( const boost::filesystem::path &file ) {
  sourceFiles.emplace( file );
}

void FunctionInfo::addHits( uint32_t count ) {
  hits += count;
}

void FunctionInfo::addCoveringTest( const TestInfoPtr &test ) {
  tests.emplace( test->getTestCase()->getName() + "::" + test->getName(), test );
}
