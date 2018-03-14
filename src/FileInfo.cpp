#include "FileInfo.h"
#include "TestInfo.h"
#include "TestCaseInfo.h"

using namespace testing::coverage;

FileInfo::FileInfo( const boost::filesystem::path &nName ) : name( nName ) {}

//void testing::coverage::from_json( const nlohmann::json &j, testing::coverage::FileInfo &data ) {
//}

void testing::coverage::to_json( nlohmann::json &j, const testing::coverage::FileInfo &data ) {
  for ( const auto &item : data.tests ) {
    j["tests"].emplace_back( item.first );
  }
}

void FileInfo::addCoveringTest( const TestInfoPtr &test ) {
  tests.emplace( test->getTestCase()->getName() + "::" + test->getName(), test );
}
